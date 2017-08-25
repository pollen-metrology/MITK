/*===================================================================

The Medical Imaging Interaction Toolkit (MITK)

Copyright (c) German Cancer Research Center,
Division of Medical and Biological Informatics.
All rights reserved.

This software is distributed WITHOUT ANY WARRANTY; without
even the implied warranty of MERCHANTABILITY or FITNESS FOR
A PARTICULAR PURPOSE.

See LICENSE.txt or http://www.mitk.org for details.

===================================================================*/

#include "mitkWindowsRealTimeClock.h"

#include "windows.h"
#include <chrono>
#include <ctime>


/**
* \brief basic contructor
*/
mitk::WindowsRealTimeClock::WindowsRealTimeClock()
{
  SetFrequency();
}

/**
* \brief basic contructor
*/
mitk::WindowsRealTimeClock::~WindowsRealTimeClock()
{

}

void mitk::WindowsRealTimeClock::SetFrequency()
{
  if ( !QueryPerformanceFrequency(&m_Frequency) )
  {
    m_Frequency.QuadPart = 0;
  }
}

/**
* \brief returns the current time in milliseconds as a double
*/
double mitk::WindowsRealTimeClock::GetCurrentStamp()
{
  return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
}

/**
* \brief returns the QueryPerformanceFrequency, needed for acquiring the time from ticks
*/
LARGE_INTEGER mitk::WindowsRealTimeClock::GetFrequency()
{
  return this->m_Frequency;
}
