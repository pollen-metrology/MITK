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
#include "berryJobTestSuite.h"

#include "berryJobTest.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestCase.h>
#include <cppunit/TestSuite.h>

namespace berry
{
  JobTestSuite::JobTestSuite() : CppUnit::TestSuite("BlueBerry JobTestSuite")
  {
    addTest(JobTest::Suite());
  }
}
