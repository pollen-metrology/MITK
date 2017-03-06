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

#include "mitkWatershedTool.h"

#include "mitkIOUtil.h"
#include "mitkITKImageImport.h"
#include "mitkImage.h"
#include "mitkLabelSetImage.h"
#include "mitkImageAccessByItk.h"
#include "mitkImageCast.h"
#include "mitkImageStatisticsHolder.h"
#include "mitkLevelWindowManager.h"
#include "mitkLookupTable.h"
#include "mitkLookupTableProperty.h"
#include "mitkProgressBar.h"
#include "mitkRenderingManager.h"
#include "mitkRenderingModeProperty.h"
#include "mitkToolCommand.h"
#include "mitkToolManager.h"
#include <mitkSliceNavigationController.h>

#include <usGetModuleContext.h>
#include <usModule.h>
#include <usModuleContext.h>
#include <usModuleResource.h>

#include <vtkLookupTable.h>

#include <itkExceptionObject.h>
#include <itkGradientMagnitudeRecursiveGaussianImageFilter.h>
#include <itkWatershedImageFilter.h>

namespace mitk
{
  MITK_TOOL_MACRO(MITKSEGMENTATION_EXPORT, WatershedTool, "Watershed tool");
}

mitk::WatershedTool::WatershedTool() : m_Threshold(0.0), m_Level(0.0)
{
}

mitk::WatershedTool::~WatershedTool()
{
}

void mitk::WatershedTool::Activated()
{
  Superclass::Activated();
}

void mitk::WatershedTool::Deactivated()
{
  Superclass::Deactivated();
}

us::ModuleResource mitk::WatershedTool::GetIconResource() const
{
  us::Module *module = us::GetModuleContext()->GetModule();
  us::ModuleResource resource = module->GetResource("Watershed_48x48.png");
  return resource;
}

const char **mitk::WatershedTool::GetXPM() const
{
  return nullptr;
}

const char *mitk::WatershedTool::GetName() const
{
  return "Watershed";
}

void mitk::WatershedTool::DoIt()
{
  // get image from tool manager
  mitk::DataNode::Pointer referenceData = m_ToolManager->GetReferenceData(0);
  mitk::Image::Pointer input = dynamic_cast<mitk::Image *>(referenceData->GetData());
  if (input.IsNull())
    return;

  unsigned int timestep = mitk::RenderingManager::GetInstance()->GetTimeNavigationController()->GetTime()->GetPos();
  input = Get3DImage(input, timestep);

  mitk::Image::Pointer output;

  try
  {
    // create and run itk filter pipeline
    AccessByItk_1(input.GetPointer(), ITKWatershed, output);

    mitk::LabelSetImage::Pointer labelSetOutput = mitk::LabelSetImage::New();
    labelSetOutput->InitializeByLabeledImage(output);

    // create a new datanode for output
    mitk::DataNode::Pointer dataNode = mitk::DataNode::New();
    dataNode->SetData(labelSetOutput);

    // set name of data node
    std::string name = referenceData->GetName() + "_Watershed";
    dataNode->SetName(name);

    // look, if there is already a node with this name
    mitk::DataStorage::SetOfObjects::ConstPointer children =
      m_ToolManager->GetDataStorage()->GetDerivations(referenceData);
    mitk::DataStorage::SetOfObjects::ConstIterator currentNode = children->Begin();
    mitk::DataNode::Pointer removeNode;
    while (currentNode != children->End())
    {
      if (dataNode->GetName().compare(currentNode->Value()->GetName()) == 0)
      {
        removeNode = currentNode->Value();
      }
      currentNode++;
    }
    // remove node with same name
    if (removeNode.IsNotNull())
      m_ToolManager->GetDataStorage()->Remove(removeNode);

    // add output to the data storage
    m_ToolManager->GetDataStorage()->Add(dataNode, referenceData);
  }
  catch (itk::ExceptionObject &e)
  {
    MITK_ERROR << "Watershed Filter Error: " << e.GetDescription();
  }

  RenderingManager::GetInstance()->RequestUpdateAll();
}

template <typename TPixel, unsigned int VImageDimension>
void mitk::WatershedTool::ITKWatershed(itk::Image<TPixel, VImageDimension> *originalImage,
                                       mitk::Image::Pointer &segmentation)
{
  typedef itk::WatershedImageFilter<itk::Image<float, VImageDimension>> WatershedFilter;
  typedef itk::GradientMagnitudeRecursiveGaussianImageFilter<itk::Image<TPixel, VImageDimension>,
                                                             itk::Image<float, VImageDimension>>
    MagnitudeFilter;

  // at first add a gradient magnitude filter
  typename MagnitudeFilter::Pointer magnitude = MagnitudeFilter::New();
  magnitude->SetInput(originalImage);
  magnitude->SetSigma(1.0);

  // use the progress bar
  mitk::ToolCommand::Pointer command = mitk::ToolCommand::New();
  command->AddStepsToDo(60);

  // then add the watershed filter to the pipeline
  typename WatershedFilter::Pointer watershed = WatershedFilter::New();
  watershed->SetInput(magnitude->GetOutput());
  watershed->SetThreshold(m_Threshold);
  watershed->SetLevel(m_Level);
  watershed->AddObserver(itk::ProgressEvent(), command);
  watershed->Update();

  // then make sure, that the output has the desired pixel type
  typedef itk::CastImageFilter<typename WatershedFilter::OutputImageType,
                               itk::Image<Tool::DefaultSegmentationDataType, VImageDimension>>
    CastFilter;
  typename CastFilter::Pointer cast = CastFilter::New();
  cast->SetInput(watershed->GetOutput());

  // start the whole pipeline
  cast->Update();

  // reset the progress bar by setting progress
  command->SetProgress(10);

  // since we obtain a new image from our pipeline, we have to make sure, that our mitk::Image::Pointer
  // is responsible for the memory management of the output image
  segmentation = mitk::GrabItkImageMemory(cast->GetOutput());
}
