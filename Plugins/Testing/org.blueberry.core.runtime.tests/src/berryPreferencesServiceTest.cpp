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

#include "berryPreferencesServiceTest.h"
#include <berryIBerryPreferencesService.h>
#include <berryIPreferences.h>
#include <berryPlatform.h>

#include "berryLog.h"

#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>

#include "Poco/File.h"
#include "Poco/Path.h"
#include "Poco/AutoPtr.h"

#include <cstdlib>
#include <iostream>
#include <sstream>

#include <QString>

using namespace std;

namespace berry
{

  PreferencesServiceTest::PreferencesServiceTest()
  : berry::TestCase(QString("PreferencesServiceTest"))
  {
  
  }

  CppUnit::Test* PreferencesServiceTest::Suite()
  {
    CppUnit::TestSuite* suite = new CppUnit::TestSuite("PreferencesServiceTest");
    
    suite->addTest(new CppUnit::TestCaller<PreferencesServiceTest>("TestAll", &PreferencesServiceTest::TestAll));

    return suite;
  }


  void PreferencesServiceTest::TestAll()
  {
    try
    {
      IPreferencesService *prefService = Platform::GetPreferencesService();
      CPPUNIT_ASSERT(nullptr != prefService);

      /// Test for: IPreferences::GetSystemPreferences()
      IPreferences::Pointer sysPrefs = prefService->GetSystemPreferences();
      CPPUNIT_ASSERT(sysPrefs.IsNotNull());

      /// Test for: IPreferences::GetUserPreferences(std::string name)
      IPreferences::Pointer testUserPrefs = prefService->GetUserPreferences("testUser");
      CPPUNIT_ASSERT(testUserPrefs.IsNotNull());

      /// Test for: IPreferences::GetUsers()
      QStringList userList = prefService->GetUsers();

      // userList should now contain "testUser"
      bool userListContainsTestUser = false;
      for (QList<QString>::iterator it = userList.begin()
        ; it != userList.end(); it++)
      {
        if(*it == "testUser")
        {
          userListContainsTestUser = true;
          break;
        }
      }
      CPPUNIT_ASSERT(userListContainsTestUser);

      IBerryPreferencesService *berryPrefService = dynamic_cast<IBerryPreferencesService*>(prefService);
      // optional test for IBerryPreferencesService
      if(NULL != berryPrefService)
      {
        /// Test for: IBerryPreferencesService::ExportPreferences(Poco::File f, std::string name="")

        // write general prefs
        std::string sysPrefsExportFilePath = Poco::Path::temp() + Poco::Path::separator() + "systemBerryPreferences";
        Poco::File sysPrefsExportFile(sysPrefsExportFilePath);
        sysPrefs->PutInt("testNumber", 1);
        berryPrefService->ExportPreferences(QString::fromStdString(sysPrefsExportFilePath));
        // assert somethings was written
        CPPUNIT_ASSERT(sysPrefsExportFile.getSize() > 0);

        // write testUser prefs
        std::string testUserPrefsExportFilePath = Poco::Path::temp() + Poco::Path::separator() + "testUserBerryPreferences";
        Poco::File testUserPrefsExportFile(testUserPrefsExportFilePath);
        testUserPrefs->PutInt("testNumber", 2);
        berryPrefService->ExportPreferences(QString::fromStdString(testUserPrefsExportFilePath), "testUser");
        CPPUNIT_ASSERT(testUserPrefsExportFile.getSize() > 0);

        /// Test for: IBerryPreferencesService::ImportPreferences(Poco::File f, std::string name="")

        // import general prefs
        // change testNumber value
        sysPrefs->PutInt("testNumber", 3);
        berryPrefService->ImportPreferences(QString::fromStdString(sysPrefsExportFilePath));
        // "testNumber" preference should now again be overwritten with its old value 1
        CPPUNIT_ASSERT(sysPrefs->GetInt("testNumber", 3) == 1);

        // import testUser prefs
        // change testNumber value
        testUserPrefs->PutInt("testNumber", 4);
        berryPrefService->ImportPreferences(QString::fromStdString(testUserPrefsExportFilePath), QString("testUser"));
        // "testNumber" preference should now again be overwritten with its old value 2
        CPPUNIT_ASSERT(testUserPrefs->GetInt("testNumber", 4) == 2);

        // delete files again
        sysPrefsExportFile.remove();
        testUserPrefsExportFile.remove();
      }
    }
    catch (Poco::CreateFileException& e)
    {
      std::string msg = "Failed to create preferences file: ";
      msg.append(e.what());
      CPPUNIT_FAIL( msg );
    }
    catch (std::exception& e)
    {
      CPPUNIT_FAIL( e.what() );
    }
    catch (...)
    {
      CPPUNIT_FAIL("unknown exception occured");
    }
  }
}
