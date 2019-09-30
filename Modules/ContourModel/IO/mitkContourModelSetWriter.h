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

#ifndef _MITK_CONTOURMODELSET_WRITER__H_
#define _MITK_CONTOURMODELSET_WRITER__H_

#include <mitkAbstractFileWriter.h>
#include <mitkContourModel.h>
#include <mitkContourModelSet.h>

namespace mitk
{
  /**
  * @brief XML-based writer for mitk::ContourModelSet
  *
  * Uses the regular ContourModel writer to write each contour of the ContourModelSet to a single file.
  *
  * @ingroup MitkContourModelModule
  */
  class ContourModelSetWriter : public mitk::AbstractFileWriter
  {
  public:
    ContourModelSetWriter();
    ~ContourModelSetWriter() override;

    using AbstractFileWriter::Write;
    void Write() override;

  protected:
    ContourModelSetWriter(const ContourModelSetWriter &other);

    mitk::ContourModelSetWriter *Clone() const override;
  };
}

#endif
