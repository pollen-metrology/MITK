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
#include "berryJobTestActivator.h"

#include "berryJobTestSuite.h"

#include <berryMacros.h>

namespace berry
{
  ctkPluginContext *JobTestActivator::pluginContext = nullptr;

  void JobTestActivator::start(ctkPluginContext *context)
  {
    pluginContext = context;
    BERRY_REGISTER_EXTENSION_CLASS(JobTestSuite, context)
  }

  void JobTestActivator::stop(ctkPluginContext *context){Q_UNUSED(context)}

  ctkPluginContext *JobTestActivator::GetContext()
  {
    return pluginContext;
  }
}
