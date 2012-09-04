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

#ifndef _mitkImageLiveWireContourModelFilter_h__
#define _mitkImageLiveWireContourModelFilter_h__

#include "mitkCommon.h"
#include "SegmentationExports.h"
#include "mitkContourModel.h"
#include "mitkContourModelSource.h"

#include <mitkImage.h>
#include <mitkImageAccessByItk.h>
#include <mitkImageCast.h>

#include <itkShortestPathCostFunctionLiveWire.h>
#include <itkShortestPathImageFilter.h>


namespace mitk {

  /**
  *
  * \brief 
  *
  * 
  *
  * \ingroup ContourModelFilters
  * \ingroup Process
  */
  class Segmentation_EXPORT ImageLiveWireContourModelFilter : public ContourModelSource
  {

  public:

    mitkClassMacro(ImageLiveWireContourModelFilter, ContourModelSource);
    itkNewMacro(Self);


    typedef ContourModel OutputType;
    typedef OutputType::Pointer OutputTypePointer;
    typedef mitk::Image InputType;

    typedef itk::Image< float,  2 > FloatImageType;
    typedef itk::ShortestPathImageFilter< FloatImageType, FloatImageType > ShortestPathImageFilterType;
    typedef itk::ShortestPathCostFunctionLiveWire< FloatImageType >        CostFunctionType;


    itkSetMacro(StartPoint, mitk::Point3D);
    itkGetMacro(StartPoint, mitk::Point3D);

    itkSetMacro(EndPoint, mitk::Point3D);
    itkGetMacro(EndPoint, mitk::Point3D);



    virtual void SetInput( const InputType *input);

    virtual void SetInput( unsigned int idx, const InputType * input);

    const InputType* GetInput(void);

    const InputType* GetInput(unsigned int idx);

    virtual OutputType* GetOutput();

  protected:
    ImageLiveWireContourModelFilter();

    virtual ~ImageLiveWireContourModelFilter();

    void GenerateOutputInformation() {};

    void GenerateData();

    mitk::Point3D m_StartPoint;
    mitk::Point3D m_EndPoint;

    mitk::Point3D m_StartPointInIndex;
    mitk::Point3D m_EndPointInIndex;

    CostFunctionType::Pointer m_CostFunction;
    ShortestPathImageFilterType::Pointer m_ShortestPathFilter;

  private:

    template<typename TPixel, unsigned int VImageDimension>
    void ItkProcessImage (itk::Image<TPixel, VImageDimension>* inputImage);
  };

}

#endif