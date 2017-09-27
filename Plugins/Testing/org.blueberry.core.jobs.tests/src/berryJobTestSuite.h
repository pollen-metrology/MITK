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
#ifndef BERRY_JOBTESTSUITE_H
#define BERRY_JOBTESTSUITE_H

#include <cppunit/TestSuite.h>

#include <QObject>

Q_DECLARE_INTERFACE(CppUnit::Test, "CppUnit.Test")

namespace berry
{
  class JobTestSuite : public QObject, public CppUnit::TestSuite
  {
    Q_OBJECT
    Q_INTERFACES(CppUnit::Test)

  public:
    JobTestSuite();
  };
}

#endif // BERRY_JOBTESTSUITE_H
