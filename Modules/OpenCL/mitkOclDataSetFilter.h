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

#ifndef __mitkOclDataSetFilter_h
#define __mitkOclDataSetFilter_h

#include "mitkOclFilter.h"
#include "mitkOclDataSet.h"

#define FILTER_UCHAR 0
#define FILTER_SHORT 1

namespace mitk
{
class OclFilter;
class OclDataSetFilter;

   /**
    * \brief The OclDataSetFilter is the topmost class for all filter which take DataSets as input.
    *
    * The input DataSet can be intialized via an oclDataSet or a pointer to the data
    * This makes it possible to create a filter pipeline of GPU-based filters
    * and to bind this part into the CPU (ITK) filter pipeline.
    */
class MITKOPENCL_EXPORT OclDataSetFilter: public OclFilter
{
public:
  /**
   * @brief SetInput SetInput Set the input DataSet (as mitk::OclDataSet).
   * @param DataSet The DataSet in mitk::OclDataSet.
   */
  void SetInput(mitk::OclDataSet::Pointer DataSet);

  /**
   * @brief SetInput Set the input DataSet (as a pointer to the data).
   * @param DataSet The DataSet in mitk::OclDataSet.
   */
  void SetInput(void* DataSet, unsigned int size, unsigned int BpE);

  /**
  * @brief SetInput Set the input DataSet (as mitk::Image).
  * @param DataSet The DataSet in mitk::OclDataSet.
  */
  void SetInput(mitk::Image::Pointer image);

protected:
  OclDataSetFilter();

  virtual ~OclDataSetFilter();

  /** The input DataSet */
  mitk::OclDataSet::Pointer m_Input;
  unsigned int m_CurrentSize;
};
}
#endif // __mitkOclDataSetFilter_h
