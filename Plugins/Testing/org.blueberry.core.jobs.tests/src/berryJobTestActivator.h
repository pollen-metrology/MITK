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
#ifndef BERRY_JOBTESTACTIVATOR_H
#define BERRY_JOBTESTACTIVATOR_H

#include <ctkPluginActivator.h>
#include <ctkServiceRegistration.h>

namespace berry
{
  class JobTestActivator : public QObject, public ctkPluginActivator
  {
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "org_blueberry_core_jobs_tests")
    Q_INTERFACES(ctkPluginActivator)

  public:
    void start(ctkPluginContext *context) override;
    void stop(ctkPluginContext *context) override;

    static ctkPluginContext *GetContext();

  private:
    static ctkPluginContext *pluginContext;
  };

  typedef JobTestActivator PluginActivator;
}

#endif // BERRY_JOBTESTACTIVATOR_H
