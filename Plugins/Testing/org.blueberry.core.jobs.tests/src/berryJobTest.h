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
#ifndef BERRY_JOBTEST_H_
#define BERRY_JOBTEST_H_



// bluleberry
#include "berryTestCase.h"
#include <berryJob.h>


// Qt
#include <QString>

namespace berry
{
  class FakeJobChangedListener : public berry::IJobChangeListener
  {
    unsigned int m_ExecutionDoneCount = 0;
  public:
    virtual void Done(const berry::IJobChangeEvent::ConstPointer& event) override
    {
      m_ExecutionDoneCount++;
    }

    virtual berry::IJobChangeListener::Events::Types GetEventTypes() override
    {
      return berry::IJobChangeListener::Events::Types(berry::IJobChangeListener::Events::Type::DONE);
    }

    unsigned int GetExecutionDoneCount() { return m_ExecutionDoneCount; }
  };


  class FakeJob : public berry::Job
  {
    unsigned int m_ExecutionCount = 0;
  public :
    FakeJob(const QString& name) : berry::Job(name)
    {

    }
    virtual berry::IStatus::Pointer Run(berry::IProgressMonitor::Pointer myProgressMonitor) override
    {
      berry::Status::SourceLocation location("this file","this method");
      berry::IStatus::Pointer myStatus = berry::Status::OK_STATUS(location);
      m_ExecutionCount++;
      return myStatus;
    }
    unsigned int GetExecutionCount() { return m_ExecutionCount; }
  };


  class JobTest : public berry::TestCase
  {
  public:
    JobTest();

    static CppUnit::Test *Suite();

    void setUp() override;
    void tearDown() override;

    void TestOneJobSchedule();
    void TestScheduleDuration();
    void TestMultipleJobSchedule();
    void TestJobReSchedule();
    void TestJobCancel();
    void TestJobRun();

  private:
    bool WaitForJobEnd(Poco::Timestamp::TimeVal maxWaitingTimeS, unsigned int expectedExecutionDone);

    FakeJobChangedListener* m_jobListener;
  };
}

#endif // BERRY_JOBTEST_H_
