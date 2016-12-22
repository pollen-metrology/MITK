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

#include "mitkImage.h"
#include "mitkPlaneGeometry.h"
#include "mitkSlicedGeometry3D.h"
#include "mitkTestingMacros.h"

#include <vnl/vnl_quaternion.h>
#include <vnl/vnl_quaternion.txx>

#include <fstream>
#include <array>

static const mitk::ScalarType slicedGeometryEps = 1e-9; // Set epsilon to float precision for this test

static mitk::PlaneGeometry::Pointer createPlaneGeometry()
{
  auto planeGeometry = mitk::PlaneGeometry::New();
  planeGeometry->Initialize();
  return planeGeometry;
}

static mitk::SlicedGeometry3D::Pointer createSlicedGeometry(const mitk::Point3D &origin, const mitk::Vector3D &spacing, int numberOfSlices)
{
  auto slicedGeometry = mitk::SlicedGeometry3D::New();
  slicedGeometry->InitializeSlicedGeometry(static_cast<unsigned int>(numberOfSlices));
  slicedGeometry->SetOrigin(origin);
  slicedGeometry->SetSpacing(spacing);

  for (unsigned int i = 0; i < numberOfSlices; ++i)
  {
    auto planeGeometry = createPlaneGeometry();
    slicedGeometry->SetPlaneGeometry(planeGeometry, i);
  }

  return slicedGeometry;
}

static mitk::SlicedGeometry3D::Pointer createEvenlySpacedSlicedGeometry(mitk::PlaneGeometry::Pointer planeGeometry, mitk::ScalarType spacing, int numberOfSlices)
{
  mitk::PlaneGeometry::Pointer planegeometry1 = mitk::PlaneGeometry::New();

  mitk::Point3D origin;
  mitk::Vector3D right, bottom, normal;
  mitk::ScalarType width, height;
  mitk::ScalarType widthInMM, heightInMM, thicknessInMM;

  width = 100;
  widthInMM = width;
  height = 200;
  heightInMM = height;
  thicknessInMM = 3.5;
  mitk::FillVector3D(origin, 4.5, 7.3, 11.2);
  mitk::FillVector3D(right, widthInMM, 0, 0);
  mitk::FillVector3D(bottom, 0, heightInMM, 0);
  mitk::FillVector3D(normal, 0, 0, thicknessInMM);

  std::cout << "Initializing planegeometry1 by InitializeStandardPlane(rightVector, downVector, spacing = NULL): "
            << std::endl;
  planegeometry1->InitializeStandardPlane(right.GetVnlVector(), bottom.GetVnlVector());

  std::cout << "Setting planegeometry2 to a cloned version of planegeometry1: " << std::endl;
  mitk::PlaneGeometry::Pointer planegeometry2;
  planegeometry2 = dynamic_cast<mitk::PlaneGeometry *>(planegeometry1->Clone().GetPointer());
  ;

  std::cout << "Changing the IndexToWorldTransform of planegeometry2 to a rotated version by "
               "SetIndexToWorldTransform() (keep origin): "
            << std::endl;
  mitk::AffineTransform3D::Pointer transform = mitk::AffineTransform3D::New();
  mitk::AffineTransform3D::MatrixType::InternalMatrixType vnlmatrix;
  vnlmatrix = planegeometry2->GetIndexToWorldTransform()->GetMatrix().GetVnlMatrix();
  mitk::VnlVector axis(3);
  mitk::FillVector3D(axis, 1.0, 1.0, 1.0);
  axis.normalize();
  vnl_quaternion<mitk::ScalarType> rotation(axis, 0.123);
  vnlmatrix = rotation.rotation_matrix_transpose() * vnlmatrix;
  mitk::Matrix3D matrix;
  matrix = vnlmatrix;
  transform->SetMatrix(matrix);
  transform->SetOffset(planegeometry2->GetIndexToWorldTransform()->GetOffset());

  right.SetVnlVector(rotation.rotation_matrix_transpose() * right.GetVnlVector());
  bottom.SetVnlVector(rotation.rotation_matrix_transpose() * bottom.GetVnlVector());
  normal.SetVnlVector(rotation.rotation_matrix_transpose() * normal.GetVnlVector());
  planegeometry2->SetIndexToWorldTransform(transform);

  std::cout << "Setting planegeometry3 to the backside of planegeometry2: " << std::endl;
  mitk::PlaneGeometry::Pointer planegeometry3 = mitk::PlaneGeometry::New();
  planegeometry3->InitializeStandardPlane(planegeometry2, mitk::PlaneGeometry::Axial, 0, false);

  std::cout
    << "Testing SlicedGeometry3D::InitializeEvenlySpaced(planegeometry3, zSpacing = 1, slices = 5): "
    << std::endl;
  mitk::SlicedGeometry3D::Pointer slicedWorldGeometry = mitk::SlicedGeometry3D::New();
  unsigned int numSlices = 5;
  slicedWorldGeometry->InitializeEvenlySpaced(planegeometry3, 1, numSlices);

  std::cout << "Testing availability and type (PlaneGeometry) of first geometry in the SlicedGeometry3D: ";
  mitk::PlaneGeometry *accessedplanegeometry3 =
    dynamic_cast<mitk::PlaneGeometry *>(slicedWorldGeometry->GetPlaneGeometry(0));
  if (accessedplanegeometry3 == nullptr)
  {
    std::cout << "[FAILED]" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "[PASSED]" << std::endl;

  std::cout << "Testing whether the first geometry in the SlicedGeometry3D is identical to planegeometry3 by axis "
               "comparison and origin: "
            << std::endl;
  if ((mitk::Equal(accessedplanegeometry3->GetAxisVector(0), planegeometry3->GetAxisVector(0), slicedGeometryEps) ==
       false) ||
      (mitk::Equal(accessedplanegeometry3->GetAxisVector(1), planegeometry3->GetAxisVector(1), slicedGeometryEps) ==
       false) ||
      (mitk::Equal(accessedplanegeometry3->GetAxisVector(2), planegeometry3->GetAxisVector(2), slicedGeometryEps) ==
       false) ||
      (mitk::Equal(accessedplanegeometry3->GetOrigin(), planegeometry3->GetOrigin(), slicedGeometryEps) == false))
  {
    std::cout << "[FAILED]" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "[PASSED]" << std::endl;

  std::cout << "Testing availability and type (PlaneGeometry) of the last geometry in the SlicedGeometry3D: ";
  mitk::PlaneGeometry *accessedplanegeometry3last =
    dynamic_cast<mitk::PlaneGeometry *>(slicedWorldGeometry->GetPlaneGeometry(numSlices - 1));
  mitk::Point3D origin3last;
  origin3last = planegeometry3->GetOrigin() + slicedWorldGeometry->GetDirectionVector() * (numSlices - 1);
  if (accessedplanegeometry3last == nullptr)
  {
    std::cout << "[FAILED]" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "[PASSED]" << std::endl;

  std::cout
    << "Testing whether the last geometry in the SlicedGeometry3D is identical to planegeometry3 by axis comparison: "
    << std::endl;
  if ((mitk::Equal(accessedplanegeometry3last->GetAxisVector(0), planegeometry3->GetAxisVector(0), slicedGeometryEps) ==
       false) ||
      (mitk::Equal(accessedplanegeometry3last->GetAxisVector(1), planegeometry3->GetAxisVector(1), slicedGeometryEps) ==
       false) ||
      (mitk::Equal(accessedplanegeometry3last->GetAxisVector(2), planegeometry3->GetAxisVector(2), slicedGeometryEps) ==
       false) ||
      (mitk::Equal(accessedplanegeometry3last->GetOrigin(), origin3last, slicedGeometryEps) == false) ||
      (mitk::Equal(accessedplanegeometry3last->GetIndexToWorldTransform()->GetOffset(),
                   origin3last.GetVectorFromOrigin(),
                   slicedGeometryEps) == false))
  {
    std::cout << "[FAILED]" << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "[PASSED]" << std::endl;

template<class T> static T createArray(mitk::ScalarType x, mitk::ScalarType y, mitk::ScalarType z)
{
  T array;
  mitk::FillVector3D(array, x, y, z);
  return array;
}

static mitk::Point3D createPoint(mitk::ScalarType x, mitk::ScalarType y, mitk::ScalarType z)
{
  return createArray<mitk::Point3D>(x, y, z);
}

static mitk::Vector3D createVector(mitk::ScalarType x, mitk::ScalarType y, mitk::ScalarType z)
{
  return createArray<mitk::Vector3D>(x, y, z);
}

static mitk::VnlVector createVnlVector(mitk::ScalarType x, mitk::ScalarType y, mitk::ScalarType z)
{
  mitk::VnlVector vector(3);
  mitk::FillVector3D(vector, x, y, z);
  return vector;
}

void mitkSlicedGeometry3D_ChangeImageGeometryConsideringOriginOffset_Test()
{
  MITK_TEST_OUTPUT(<< "====== mitkSlicedGeometry3D_ChangeImageGeometryConsideringOriginOffset_Test() ======");

  auto origin = createPoint(91.3, -13.3, 0);
  auto spacing = createVector(1.0, 0.9, 0.3);
  auto numberOfSlices = 5;
  auto slicedGeometry = createSlicedGeometry(origin, spacing, numberOfSlices);

  MITK_TEST_OUTPUT(<< "SlicedGeometry3D isn't an image geometry by default");
  MITK_TEST_CONDITION_REQUIRED(slicedGeometry->GetImageGeometry() == false, "");

  MITK_TEST_OUTPUT(<< "First and last PlaneGeometry in SlicedGeometry3D are not image geometries");
  auto firstPlaneGeometry = slicedGeometry->GetPlaneGeometry(0);
  auto lastPlaneGeometry = slicedGeometry->GetPlaneGeometry(numberOfSlices - 1);
  MITK_TEST_CONDITION_REQUIRED(firstPlaneGeometry->GetImageGeometry() == false, "");
  MITK_TEST_CONDITION_REQUIRED(lastPlaneGeometry->GetImageGeometry() == false, "");

  auto originOfSlicedGeometry = slicedGeometry->GetOrigin();
  auto originOfFirstPlaneGeometry = firstPlaneGeometry->GetOrigin();
  auto originOfLastPlaneGeometry = lastPlaneGeometry->GetOrigin();
  auto firstCornerPointOfSlicedGeometry = slicedGeometry->GetCornerPoint(0);
  auto secondCornerPointOfFirstPlaneGeometry = firstPlaneGeometry->GetCornerPoint(1);
  auto thirdCornerPointOfLastPlaneGeometry = lastPlaneGeometry->GetCornerPoint(2);

  MITK_TEST_OUTPUT(<< "Calling SlicedGeometry3D::ChangeImageGeometryConsideringOriginOffset(true)");
  slicedGeometry->ChangeImageGeometryConsideringOriginOffset(true);
  MITK_TEST_OUTPUT(<< "SlicedGeometry3D is an image geometry");
  MITK_TEST_CONDITION_REQUIRED(slicedGeometry->GetImageGeometry() == true, "");
  MITK_TEST_OUTPUT(<< "First and last PlaneGeometry in SlicedGeometry3D are image geometries");
  MITK_TEST_CONDITION_REQUIRED(firstPlaneGeometry->GetImageGeometry() == true, "");
  MITK_TEST_CONDITION_REQUIRED(lastPlaneGeometry->GetImageGeometry() == true, "");

  MITK_TEST_OUTPUT(<< "Corner points of geometries didn't change");
  MITK_TEST_CONDITION_REQUIRED(slicedGeometry->GetCornerPoint(0) == firstCornerPointOfSlicedGeometry, "");
  MITK_TEST_CONDITION_REQUIRED(firstPlaneGeometry->GetCornerPoint(1) == secondCornerPointOfFirstPlaneGeometry, "");
  MITK_TEST_CONDITION_REQUIRED(lastPlaneGeometry->GetCornerPoint(2) == thirdCornerPointOfLastPlaneGeometry, "");

  MITK_TEST_OUTPUT(<< "Offsets were added to geometry origins");
  MITK_TEST_CONDITION_REQUIRED(slicedGeometry->GetOrigin() == originOfSlicedGeometry + slicedGeometry->GetSpacing() * 0.5, "");
  MITK_TEST_CONDITION_REQUIRED(firstPlaneGeometry->GetOrigin() == originOfFirstPlaneGeometry + firstPlaneGeometry->GetSpacing() * 0.5, "");
  MITK_TEST_CONDITION_REQUIRED(lastPlaneGeometry->GetOrigin() == originOfLastPlaneGeometry + lastPlaneGeometry->GetSpacing() * 0.5, "");

  MITK_TEST_OUTPUT(<< "Calling SlicedGeometry3D::ChangeImageGeometryConsideringOriginOffset(false)");
  slicedGeometry->ChangeImageGeometryConsideringOriginOffset(false);
  MITK_TEST_OUTPUT(<< "SlicedGeometry3D isn't an image geometry anymore");
  MITK_TEST_CONDITION_REQUIRED(slicedGeometry->GetImageGeometry() == false, "");
  MITK_TEST_OUTPUT(<< "First and last PlaneGeometry in SlicedGeometry3D are no image geometries anymore");
  MITK_TEST_CONDITION_REQUIRED(firstPlaneGeometry->GetImageGeometry() == false, "");
  MITK_TEST_CONDITION_REQUIRED(lastPlaneGeometry->GetImageGeometry() == false, "");

  MITK_TEST_OUTPUT(<< "Corner points of geometries didn't change");
  MITK_TEST_CONDITION_REQUIRED(slicedGeometry->GetCornerPoint(0) == firstCornerPointOfSlicedGeometry, "");
  MITK_TEST_CONDITION_REQUIRED(firstPlaneGeometry->GetCornerPoint(1) == secondCornerPointOfFirstPlaneGeometry, "");
  MITK_TEST_CONDITION_REQUIRED(lastPlaneGeometry->GetCornerPoint(2) == thirdCornerPointOfLastPlaneGeometry, "");

  MITK_TEST_OUTPUT(<< "Offsets were subtracted from geometry origins");
  MITK_TEST_CONDITION_REQUIRED(slicedGeometry->GetOrigin() == originOfSlicedGeometry, "");
  MITK_TEST_CONDITION_REQUIRED(firstPlaneGeometry->GetOrigin() == originOfFirstPlaneGeometry, "");
  MITK_TEST_CONDITION_REQUIRED(lastPlaneGeometry->GetOrigin() == originOfLastPlaneGeometry, "");
}

int mitkSlicedGeometry3DTest(int, char *[])
{
  mitk::ScalarType width = 100.0;
  mitk::ScalarType widthInMM = width;

  mitk::ScalarType height = 200.0;
  mitk::ScalarType heightInMM = height;

  mitk::ScalarType thicknessInMM = 3.0;

  auto right = createVector(widthInMM, 0.0, 0.0);
  auto bottom = createVector(0.0, heightInMM, 0.0);
  auto normal = createVector(0.0, 0.0, thicknessInMM);
  auto spacing = createVector(1.0, 1.0, thicknessInMM);

  auto planeGeometry = mitk::PlaneGeometry::New();
  planeGeometry->InitializeStandardPlane(right, bottom, &spacing);

  auto numberOfSlices = 5;
  auto slicedGeometry = createEvenlySpacedSlicedGeometry(planeGeometry, thicknessInMM, numberOfSlices);
  auto firstPlaneGeometry = slicedGeometry->GetPlaneGeometry(0);

  MITK_TEST_OUTPUT(<< "Check if first PlaneGeometry of evenly spaced SlicedGeometry is valid");
  MITK_TEST_CONDITION_REQUIRED(firstPlaneGeometry != nullptr, "");
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(firstPlaneGeometry->GetAxisVector(0), planeGeometry->GetAxisVector(0), slicedGeometryEps), "");
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(firstPlaneGeometry->GetAxisVector(1), planeGeometry->GetAxisVector(1), slicedGeometryEps), "");
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(firstPlaneGeometry->GetAxisVector(2), planeGeometry->GetAxisVector(2), slicedGeometryEps), "");

  auto lastPlaneGeometry = slicedGeometry->GetPlaneGeometry(numberOfSlices - 1);
  auto expectedOriginOfLastSlice = createPoint(0.0, 0.0, thicknessInMM * (numberOfSlices - 1));

  MITK_TEST_OUTPUT(<< "Check if origin of last PlaneGeometry is at expected location");
  MITK_TEST_CONDITION_REQUIRED(mitk::Equal(lastPlaneGeometry->GetOrigin(), expectedOriginOfLastSlice, slicedGeometryEps), "");

  mitkSlicedGeometry3D_ChangeImageGeometryConsideringOriginOffset_Test();

  std::cout << "[TEST DONE]" << std::endl;
  return EXIT_SUCCESS;
}
