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

#ifndef __mitkDiffusionFunctionCollection_h_
#define __mitkDiffusionFunctionCollection_h_


#include <MitkDiffusionCoreExports.h>
#include <vnl/vnl_vector.h>
#include <vnl/vnl_vector_fixed.h>
#include <itkVectorContainer.h>
#include <itkImage.h>
#include <itkLinearInterpolateImageFunction.h>
#include <mitkImage.h>

namespace mitk{

class imv
{
public:

  template< class TPixelType, class TOutPixelType=TPixelType >
  static TOutPixelType GetImageValue(const itk::Point<float, 3>& itkP, itk::Image<TPixelType, 3>* image, bool interpolate)
  {
    itk::ContinuousIndex< float, 3> cIdx;
    image->TransformPhysicalPointToContinuousIndex(itkP, cIdx);

    typename itk::LinearInterpolateImageFunction< itk::Image< TPixelType, 3 >, float >::Pointer interpolator = itk::LinearInterpolateImageFunction< itk::Image< TPixelType, 3 >, float >::New();
    interpolator->SetInputImage(image);

    if (interpolator->IsInsideBuffer(cIdx))
    {
      if (interpolate)
        return interpolator->EvaluateAtContinuousIndex(cIdx);
      else
      {
        itk::Index<3> idx;
        image->TransformPhysicalPointToIndex(itkP, idx);
        return image->GetPixel(idx);
      }
    }
    else
      return 0.0;
  }

  template< class TPixelType=unsigned char >
  static bool IsInsideMask(const itk::Point<float, 3>& itkP, itk::Image<TPixelType, 3>* image, bool interpolate)
  {
    itk::ContinuousIndex< float, 3> cIdx;
    image->TransformPhysicalPointToContinuousIndex(itkP, cIdx);

    typename itk::LinearInterpolateImageFunction< itk::Image< TPixelType, 3 >, float >::Pointer interpolator = itk::LinearInterpolateImageFunction< itk::Image< TPixelType, 3 >, float >::New();
    interpolator->SetInputImage(image);

    if (interpolator->IsInsideBuffer(cIdx))
    {
      double value = 0.0;
      if (interpolate)
        value = interpolator->EvaluateAtContinuousIndex(cIdx);
      else
      {
        itk::Index<3> idx;
        image->TransformPhysicalPointToIndex(itkP, idx);
        value = image->GetPixel(idx);
      }

      if (value>=0.5)
        return true;
    }
    return false;
  }

};

class MITKDIFFUSIONCORE_EXPORT sh
{
public:
  static double factorial(int number);
  static void Cart2Sph(double x, double y, double z, double* cart);
  static double legendre0(int l);
  static double spherical_harmonic(int m,int l,double theta,double phi, bool complexPart);
  static double Yj(int m, int k, double theta, double phi);
};

class MITKDIFFUSIONCORE_EXPORT gradients
{
private:
  typedef std::vector<unsigned int> IndiciesVector;
  typedef std::map<unsigned int, IndiciesVector > BValueMap;
  typedef itk::VectorContainer< unsigned int, vnl_vector_fixed< double, 3 > > GradientDirectionContainerType;
  typedef vnl_vector_fixed<double , 3 > GradientDirectionType;

public:
  static std::vector<unsigned int> GetAllUniqueDirections(const BValueMap &bValueMap, GradientDirectionContainerType *refGradientsContainer );

  static bool CheckForDifferingShellDirections(const BValueMap &bValueMap, GradientDirectionContainerType::ConstPointer refGradientsContainer);
  static vnl_matrix<double> ComputeSphericalHarmonicsBasis(const vnl_matrix<double> & QBallReference, const unsigned int & LOrder);
  static vnl_matrix<double> ComputeSphericalFromCartesian(const IndiciesVector  & refShell, const GradientDirectionContainerType * refGradientsContainer);
  static mitk::gradients::GradientDirectionContainerType::Pointer CreateNormalizedUniqueGradientDirectionContainer(const BValueMap &bValueMap, const GradientDirectionContainerType * origninalGradentcontainer);
};

}

#endif //__mitkDiffusionFunctionCollection_h_

