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

#include "berryCoreTestApplication.h"

#include <berryPlatform.h>
#include <berryLog.h>

#include "berryBlueBerryTestDriver.h"

namespace berry {

CoreTestApplication::CoreTestApplication()
{

}

CoreTestApplication::CoreTestApplication(const CoreTestApplication& other)
  : QObject()
{
  Q_UNUSED(other)
}

int CoreTestApplication::Start() {

  QString testPlugin = Platform::GetDebugOption(Platform::PROP_TESTPLUGIN).toString();
  if (QString::null != testPlugin) {
    return BlueBerryTestDriver::Run(testPlugin);
  }
  else
  {
    BERRY_ERROR << "You must specify a test plug-in id via " << Platform::PROP_TESTPLUGIN << "=<id>";
    return 1;
  }
}

void CoreTestApplication::Stop() {

}

}
