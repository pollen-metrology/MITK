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
#include "berryIApplicationContext.h"
#include "internal/berryPluginActivator.h"

#include "berryBlueBerryTestDriver.h"

#include <QString>

namespace berry {

CoreTestApplication::CoreTestApplication()
{

}

CoreTestApplication::CoreTestApplication(const CoreTestApplication& other)
  : QObject()
{
  Q_UNUSED(other)
}

QVariant CoreTestApplication::Start(IApplicationContext* context) {

  ctkPluginContext* pluginContext = org_blueberry_test_Activator::GetInstance()->GetContext();

  QString testPlugin = pluginContext->getProperty(Platform::PROP_TESTPLUGIN).toString();
  if (!testPlugin.isNull()) {
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
