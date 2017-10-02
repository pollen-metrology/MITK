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
#include "berryJobTest.h"

#include <cppunit/TestCaller.h>
#include <cppunit/TestSuite.h>

#include <Poco/Thread.h>



namespace berry
{
  JobTest::JobTest() : berry::TestCase(QString("JobTest"))
  {
  }

  CppUnit::Test *JobTest::Suite()
  {
    CppUnit::TestSuite *suite = new CppUnit::TestSuite("JobTest");
    suite->addTest(
      new CppUnit::TestCaller<JobTest>("TestJobRun", &JobTest::TestJobRun));
    suite->addTest(
      new CppUnit::TestCaller<JobTest>("TestOneJobSchedule", &JobTest::TestOneJobSchedule));
    suite->addTest(
      new CppUnit::TestCaller<JobTest>("TestScheduleDuration", &JobTest::TestScheduleDuration));
    suite->addTest(
      new CppUnit::TestCaller<JobTest>("TestJobCancel", &JobTest::TestJobCancel));
    suite->addTest(
      new CppUnit::TestCaller<JobTest>("TestMultipleJobSchedule", &JobTest::TestMultipleJobSchedule));
    suite->addTest(
      new CppUnit::TestCaller<JobTest>("TestJobReSchedule", &JobTest::TestJobReSchedule));
    
    return suite;
  }

  void JobTest::setUp()
  {

    m_jobListener = new FakeJobChangedListener();
  }

  void JobTest::tearDown()
  {
    delete m_jobListener;
  }

  void JobTest::TestMultipleJobSchedule()
  {
    unsigned int maxJobs = 100;
    std::vector<berry::SmartPointer<FakeJob>> jobs;

    for (unsigned int iJob = 0; iJob < maxJobs; iJob++)
    {
      berry::SmartPointer<FakeJob> fakeJob(new FakeJob(QString("fake job %1").arg(iJob)));
      jobs.push_back(fakeJob);
      fakeJob->AddJobChangeListener(m_jobListener);
      fakeJob->Schedule();
    }

    bool allThreadDone = false;
    bool jobSuccess = this->WaitForJobEnd(10, maxJobs);

    unsigned int iExecutionCount = 0;
    for (unsigned int iJob = 0; iJob < maxJobs; iJob++)
    {
      iExecutionCount += jobs[iJob]->GetExecutionCount();
      jobs[iJob]->RemoveJobChangeListener(m_jobListener);
    }
    CPPUNIT_ASSERT_MESSAGE("All thread must have been completed", jobSuccess);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("All Jobs should have been executed", maxJobs, iExecutionCount);

  }
  void JobTest::TestJobReSchedule()
  {
    Poco::Timestamp::TimeVal maxTimeS = 10;
    berry::SmartPointer<FakeJob> fakeJob(new FakeJob("fake job"));

    fakeJob->AddJobChangeListener(m_jobListener);
    fakeJob->Schedule();

    berry::Status::SourceLocation location("this file", "this method");
    berry::IStatus::Pointer myStatus = berry::Status::OK_STATUS(location);

    bool jobSuccess = this->WaitForJobEnd(maxTimeS, 1);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Job should have been executed", (unsigned int)1, fakeJob->GetExecutionCount());
    CPPUNIT_ASSERT_MESSAGE("Job should have been executed", jobSuccess);


    //// Rescedule job for a new usage
    fakeJob->Schedule();
    jobSuccess = this->WaitForJobEnd(maxTimeS, 2);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Job should have been executed", (unsigned int)2, fakeJob->GetExecutionCount());
    CPPUNIT_ASSERT_MESSAGE("Job should have been executed", jobSuccess);

    // Test postponing the job
    fakeJob->Schedule(100);
    fakeJob->Schedule(200);
    jobSuccess = this->WaitForJobEnd(maxTimeS, 3);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Job should have been executed", (unsigned int)3, fakeJob->GetExecutionCount());
    CPPUNIT_ASSERT_MESSAGE("Job should have been executed", jobSuccess);
    
    // Test scheduling job while one is executing, only 1 execution should be added
    fakeJob->Schedule();
    fakeJob->Schedule();

    jobSuccess = this->WaitForJobEnd(maxTimeS, 4);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Job should have been executed", (unsigned int)4, fakeJob->GetExecutionCount());
    CPPUNIT_ASSERT_MESSAGE("Job should have been executed", jobSuccess);
    fakeJob->RemoveJobChangeListener(m_jobListener);

  }
  void JobTest::TestJobCancel()
  {
    berry::SmartPointer<FakeJob> fakeJob(new FakeJob("fake job"));

    FakeJobChangedListener* jobListener = new FakeJobChangedListener();
    fakeJob->AddJobChangeListener(jobListener);
    fakeJob->Schedule(500);
    fakeJob->Cancel();
    Poco::Thread::sleep(1000);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Job should be accounted as canceled", (unsigned int)1, jobListener->GetExecutionDoneCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Job shouldn't have run", (unsigned int)0, fakeJob->GetExecutionCount());
    fakeJob->RemoveJobChangeListener(jobListener);
    delete jobListener;
  }
  void JobTest::TestOneJobSchedule()
  {
    berry::SmartPointer<FakeJob> fakeJob(new FakeJob("fake job"));

    fakeJob->AddJobChangeListener(m_jobListener);
    fakeJob->Schedule();

    bool jobSuccess = this->WaitForJobEnd(5, 1);

    CPPUNIT_ASSERT_MESSAGE("Listener should have been informed", jobSuccess);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Job should have been executed", (unsigned int)1, fakeJob->GetExecutionCount());
    fakeJob->RemoveJobChangeListener(m_jobListener);
    
  }
  void JobTest::TestScheduleDuration()
  {
    berry::SmartPointer<FakeJob> fakeJob(new FakeJob("fake job"));

    fakeJob->AddJobChangeListener(m_jobListener);
    // Schedule for long time seconds
    fakeJob->Schedule(3000);
    // wait for shorter time
    bool jobSuccess = this->WaitForJobEnd(1, 1);

    // Job shouldn't have executed
    CPPUNIT_ASSERT_MESSAGE("Listener should have been informed", !jobSuccess);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Job should have been executed", (unsigned int)0, fakeJob->GetExecutionCount());


    jobSuccess = this->WaitForJobEnd(3, 1);

    // Job should have been executed
    CPPUNIT_ASSERT_MESSAGE("Listener should have been informed", jobSuccess);
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Job should have been executed", (unsigned int)1, fakeJob->GetExecutionCount());
    fakeJob->RemoveJobChangeListener(m_jobListener);

  }
  void JobTest::TestJobRun()
  {
    FakeJob fakeJob("fake job");
    berry::IProgressMonitor::Pointer progressMonitor;
    fakeJob.AddJobChangeListener(m_jobListener);
    fakeJob.Run(progressMonitor);

    CPPUNIT_ASSERT_EQUAL_MESSAGE("Job listener should not have been informed because Run methode does not call listener", (unsigned int)0, m_jobListener->GetExecutionDoneCount());
    CPPUNIT_ASSERT_EQUAL_MESSAGE("Job should have been runned one time", (unsigned int)1, fakeJob.GetExecutionCount());
    fakeJob.RemoveJobChangeListener(m_jobListener);
  }
  bool JobTest::WaitForJobEnd(Poco::Timestamp::TimeVal maxWaitingTimeS, unsigned int expectedExecutionDone)
  {
    Poco::Timestamp::TimeVal timeLimitMicroS = maxWaitingTimeS * 1000000;
    Poco::Timestamp startTime;
    Poco::Timestamp currentTime;
    bool allThreadDone = false;
    while (currentTime < (startTime + timeLimitMicroS) && !allThreadDone)
    {
      allThreadDone = (m_jobListener->GetExecutionDoneCount() == expectedExecutionDone);
      currentTime.update();
    }

    return allThreadDone;
  }
}
