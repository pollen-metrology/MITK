/*===================================================================

BlueBerry Platform

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#include "berryPreferencesTest.h"

#include <berryIBerryPreferences.h>
#include <berryIBerryPreferencesService.h>
#include <berryPlatform.h>

#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "berryLog.h"

#include "Poco/File.h"
#include "Poco/Path.h"

#include <cstdlib>
#include <iostream>
#include <limits>

using namespace std;

namespace berry
{

  PreferencesTest::PreferencesTest()
  : berry::TestCase(QString("PreferencesTest"))
  {}

  CppUnit::Test* PreferencesTest::Suite()
  {
    CppUnit::TestSuite* suite = new CppUnit::TestSuite("PreferencesTest");
    suite->addTest(new CppUnit::TestCaller<PreferencesTest>("TestAll", &PreferencesTest::TestAll));
    return suite;
  }

  // simple class for testing berry changed events
  class TestPreferencesChangedListener
  {
  public:
    TestPreferencesChangedListener(IBerryPreferences* _berryPrefNode) : numCalled(0), berryPrefNode(_berryPrefNode)
    {
      berryPrefNode->OnChanged.AddListener(
        berry::MessageDelegate1<TestPreferencesChangedListener, const IBerryPreferences*> (
        this, &TestPreferencesChangedListener::PreferencesChanged )
        );
    };

    ~TestPreferencesChangedListener()
    {
      berryPrefNode->OnChanged.RemoveListener(
        berry::MessageDelegate1<TestPreferencesChangedListener, const IBerryPreferences*> (
        this, &TestPreferencesChangedListener::PreferencesChanged )
        );
    };

    void PreferencesChanged(const IBerryPreferences*)
    {
      ++numCalled;
    }
    int numCalled;
    IBerryPreferences* berryPrefNode;
  };

  void PreferencesTest::TestAll()
  {
    IPreferencesService *prefService = Platform::GetPreferencesService();
    CPPUNIT_ASSERT(nullptr != prefService);

    /// Test for: IPreferences::GetSystemPreferences()
    IPreferences::Pointer root = prefService->GetSystemPreferences();
    CPPUNIT_ASSERT(root.IsNotNull());

    {
      BERRY_INFO << "testing Preferences::Node(), Preferences::NodeExists(), Preferences::Parent(), "
        "Preferences::ChildrenNames(), Preferences::RemoveNode()";

      berry::IPreferences::Pointer editorsNode(0);
      editorsNode = root->Node("/editors");
      CPPUNIT_ASSERT(editorsNode.IsNotNull());

      CPPUNIT_ASSERT(editorsNode->NodeExists("/editors"));

      CPPUNIT_ASSERT(editorsNode->Parent() == root);

      berry::IPreferences::Pointer editorsGeneralNode = root->Node("/editors/general");
      CPPUNIT_ASSERT(editorsNode->NodeExists("/editors/general"));

      berry::IPreferences::Pointer editorsSyntaxNode = root->Node("/editors/syntax");
      CPPUNIT_ASSERT(editorsGeneralNode->NodeExists("/editors/syntax"));

      berry::IPreferences::Pointer editorsFontNode = root->Node("/editors/font");
      CPPUNIT_ASSERT(editorsSyntaxNode->NodeExists("/editors/font"));

      QStringList childrenNames;
      childrenNames.push_back("general");
      childrenNames.push_back("syntax");
      childrenNames.push_back("font");
      CPPUNIT_ASSERT( (editorsNode->ChildrenNames() == childrenNames) );

      editorsFontNode->RemoveNode();
      try {
        editorsFontNode->Parent();
        CPPUNIT_FAIL("this should throw a Poco::IllegalStateException");
      }
      catch (Poco::IllegalStateException)
      {
        // expected
      }
    }

    // testing methods
    // Preferences::put*()
    // Preferences::get*()
    {
      BERRY_INFO << "testing Preferences::put*(), Preferences::get*(), OnChanged";

      CPPUNIT_ASSERT(root->NodeExists("/editors/general"));
      berry::IPreferences::Pointer editorsGeneralNode = root->Node("/editors/general");

      IBerryPreferences::Pointer berryEditorsGeneralNode = editorsGeneralNode.Cast< IBerryPreferences >();
      CPPUNIT_ASSERT(berryEditorsGeneralNode.IsNotNull());

      TestPreferencesChangedListener listener(berryEditorsGeneralNode.GetPointer());

      QString strKey = QString::fromStdString("Bad words");
      QString strValue = QString::fromStdString("badword1 badword2");
      editorsGeneralNode->Put(strKey, strValue);

      CPPUNIT_ASSERT(listener.numCalled == 1);
      CPPUNIT_ASSERT(editorsGeneralNode->Get(strKey, "") == strValue);
      CPPUNIT_ASSERT(editorsGeneralNode->Get("wrong key", "default value") == "default value");

      strKey = "Show Line Numbers";bool bValue = true;
      editorsGeneralNode->PutBool(strKey, bValue);
      CPPUNIT_ASSERT(listener.numCalled == 2);
      CPPUNIT_ASSERT(editorsGeneralNode->GetBool(strKey, !bValue) == bValue);

      strKey = "backgroundcolor"; 
      QByteArray byteArrayValue("#00FF00");
      editorsGeneralNode->PutByteArray(strKey, byteArrayValue);
      CPPUNIT_ASSERT(listener.numCalled == 3);
      CPPUNIT_ASSERT(editorsGeneralNode->GetByteArray(strKey, "") == strValue);

      strKey = "update time"; double dValue = 1.23;
      editorsGeneralNode->PutDouble(strKey, dValue);
      CPPUNIT_ASSERT(editorsGeneralNode->GetDouble(strKey, 0.0) == dValue);

      strKey = "update time float"; float fValue = 1.23f;
      editorsGeneralNode->PutFloat(strKey, fValue);
      CPPUNIT_ASSERT(editorsGeneralNode->GetFloat(strKey, 0.0f) == fValue);

      strKey = "Break on column"; int iValue = 80;
      editorsGeneralNode->PutInt(strKey, iValue);
      CPPUNIT_ASSERT(editorsGeneralNode->GetInt(strKey, 0) == iValue);

      strKey = "Maximum number of words"; long lValue = 11000000;
      editorsGeneralNode->PutLong(strKey, lValue);
      CPPUNIT_ASSERT(editorsGeneralNode->GetLong(strKey, 0) == lValue);
    }

  }

}
