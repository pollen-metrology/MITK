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

#include "berryUITestApplication.h"

#include <berryBlueBerryTestDriver.h>
#include <berryPlatformUI.h>
#include <berryPlatform.h>
#include <berryIExtensionRegistry.h>
#include <berryIExtension.h>
#include "berryIApplicationContext.h"

#include "internal/berryUITestWorkbenchAdvisor.h"


namespace berry
{

class WorkbenchCloseRunnable: public Poco::Runnable
{
public:
  WorkbenchCloseRunnable(IWorkbench* workbench) :
    workbench(workbench)
  {
  }

  void run()
  {
    workbench->Close();
  }

private:
  IWorkbench* workbench;
};

UITestApplication::TestRunnable::TestRunnable(UITestApplication* app,
    const QString& testPlugin) :
  app(app), testPlugin(testPlugin)
{
}

void UITestApplication::TestRunnable::run()
{
  try
  {
    app->testDriverResult = BlueBerryTestDriver::Run(testPlugin, true);
  }
  catch (const ctkException& e)
  {
    qWarning() << e.printStackTrace();
  }
  catch (const std::exception& e)
  {
    qWarning() << e.what();
  }
}

UITestApplication::UITestApplication()
{

}

QVariant UITestApplication::Start(IApplicationContext* context)
{
	QString platypusPluginToTestPrefix("platypusPluginToTest=");
	QString testPlugin;
	for (QString unProcessedArg : Platform::GetApplicationArgs())
	{
		if (unProcessedArg.startsWith(platypusPluginToTestPrefix))
		{
			testPlugin = unProcessedArg.mid(platypusPluginToTestPrefix.size());
		}
	}

	if (!testPlugin.isNull())
	{
    BERRY_ERROR << "You must specify a test plug-in id via "
        << Platform::PROP_TESTPLUGIN << "=<id>";
    return 1;
  }
  
  // Get the application to test
  IApplication* application = GetApplication();
  poco_assert(application);
  int result = RunApplication(application);

  if (IApplication::EXIT_OK != result)
  {
    std::cerr << "UITestRunner: Unexpected result from running application " << application << ": " << result << std::endl;
  }
  return testDriverResult;
}

void UITestApplication::Stop()
{
  IWorkbench* workbench = PlatformUI::GetWorkbench();
  if (!workbench)
  return;
  Display* display = workbench->GetDisplay();
  WorkbenchCloseRunnable runnable(workbench);
  display->SyncExec(&runnable);
}

void UITestApplication::RunTests()
{
  TestRunnable runnable(this, testPlugin);
  testableObject->TestingStarting();
  testableObject->RunTest(&runnable);
  testableObject->TestingFinished();
}

IApplication* UITestApplication::GetApplication()
{

  IExtension::Pointer extension;
  /*Platform::GetExtensionPointService()->GetExtension(
      Starter::XP_APPLICATIONS, GetApplicationToRun());*/

  QList<IConfigurationElement::Pointer> extensions(
        Platform::GetExtensionRegistry()->GetConfigurationElementsFor(Platform::PROP_TESTAPPLICATION));

  QString appToRun = GetApplicationToRun();
  QString id;
  foreach (const IConfigurationElement::Pointer& configElem, extensions)
  {
    id = configElem->GetAttribute("id");
    if(id == appToRun)
    {
      extension = configElem->GetDeclaringExtension();
      break;
    }
  }

  IApplication* app = 0;

  if (extension)
  {
    QList<IConfigurationElement::Pointer> elements(
          extension->GetConfigurationElements());
    if (!elements.isEmpty())
    {
      QList<IConfigurationElement::Pointer> runs(
            elements[0]->GetChildren("run"));
      if (!runs.isEmpty())
      {
        app = runs[0]->CreateExecutableExtension<IApplication> ("class");
      }
    }
    return app;
  }

  return this;
}

QString UITestApplication::GetApplicationToRun()
{
  QString testApp = Platform::GetDebugOption(Platform::PROP_TESTAPPLICATION).toString();
  return testApp;
}

int UITestApplication::RunApplication(IApplication* application)
{
  testableObject = PlatformUI::GetTestableObject();
  testableObject->SetTestHarness(ITestHarness::Pointer(this));

  if (application == dynamic_cast<IApplication*>(this))
  return RunUITestWorkbench();

  return application->Start(nullptr).toInt();
}

int UITestApplication::RunUITestWorkbench()
{
  Display* display = PlatformUI::CreateDisplay();
  UITestWorkbenchAdvisor advisor;
  return PlatformUI::CreateAndRunWorkbench(display, &advisor);
}

}
