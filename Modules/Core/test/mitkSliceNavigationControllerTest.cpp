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

#include <mitkGeometry3D.h>
#include <mitkPlaneGeometry.h>
#include <mitkSlicedGeometry3D.h>
#include <mitkSliceNavigationController.h>

#include <mitkTestFixture.h>
#include <mitkTestingMacros.h>

// T22254

class mitkSliceNavigationControllerTestSuite : public mitk::TestFixture
{
  CPPUNIT_TEST_SUITE(mitkSliceNavigationControllerTestSuite);
  CPPUNIT_TEST(validateAxialViewDirection);
  CPPUNIT_TEST(validateCoronalViewDirection);
  CPPUNIT_TEST(validateSagittalViewDirection);
  CPPUNIT_TEST_SUITE_END();

public:
  void setUp() override
  {
    mitk::Point3D origin;
    mitk::FillVector3D(origin, 10.0, 20.0, 30.0);

    mitk::Vector3D firstAxisVector;
    mitk::FillVector3D(firstAxisVector, 100.0, 0.0, 0.0);

    mitk::Vector3D secondAxisVector;
    mitk::FillVector3D(secondAxisVector, 0.0, 50.0, 0.0);

    mitk::Vector3D spacing;
    mitk::FillVector3D(spacing, 1.0, 1.0, 2.0);

    auto planeGeometry = mitk::PlaneGeometry::New();
    planeGeometry->InitializeStandardPlane(firstAxisVector, secondAxisVector, &spacing);
    planeGeometry->SetOrigin(origin);

    unsigned int numberOfSlices = 100U;

    auto slicedGeometry3D = mitk::SlicedGeometry3D::New();
    slicedGeometry3D->InitializeEvenlySpaced(planeGeometry, numberOfSlices);

    m_Geometry3D = mitk::Geometry3D::New();
    m_Geometry3D->SetBounds(slicedGeometry3D->GetBounds());
    m_Geometry3D->SetIndexToWorldTransform(slicedGeometry3D->GetIndexToWorldTransform());
  }

  void tearDown() override
  {
  }

  void validateAxialViewDirection()
  {
    auto sliceNavigationController = mitk::SliceNavigationController::New();

    sliceNavigationController->SetInputWorldGeometry3D(m_Geometry3D);
    sliceNavigationController->SetViewDirection(mitk::SliceNavigationController::Axial);
    sliceNavigationController->Update();

int testReorientPlanes()
{
  // Create PlaneGeometry
  mitk::PlaneGeometry::Pointer planegeometry = mitk::PlaneGeometry::New();

  mitk::Point3D origin;
  mitk::Vector3D right, bottom, normal;
  mitk::ScalarType width, height;
  mitk::ScalarType widthInMM, heightInMM, thicknessInMM;

  width = 100;
  widthInMM = width;
  height = 200;
  heightInMM = height;
  thicknessInMM = 1.5;

  mitk::FillVector3D(origin, 4.5, 7.3, 11.2);
  mitk::FillVector3D(right, widthInMM, 0, 0);
  mitk::FillVector3D(bottom, 0, heightInMM, 0);
  mitk::FillVector3D(normal, 0, 0, thicknessInMM);

  mitk::Vector3D spacing;
  normal.Normalize();
  normal *= thicknessInMM;
  mitk::FillVector3D(spacing, 1.0, 1.0, thicknessInMM);
  planegeometry->InitializeStandardPlane(right.GetVnlVector(), bottom.GetVnlVector(), &spacing);
  planegeometry->SetOrigin(origin);

  // Create SlicedGeometry3D out of planeGeometry
  mitk::SlicedGeometry3D::Pointer slicedgeometry1 = mitk::SlicedGeometry3D::New();
  unsigned int numSlices = 20;
  slicedgeometry1->InitializeEvenlySpaced(planegeometry, thicknessInMM, numSlices);

  // Create another slicedgeo which will be rotated
  mitk::SlicedGeometry3D::Pointer slicedgeometry2 = mitk::SlicedGeometry3D::New();
  slicedgeometry2->InitializeEvenlySpaced(planegeometry, thicknessInMM, numSlices);

  // Create  geo3D as reference
  mitk::Geometry3D::Pointer geometry = mitk::Geometry3D::New();
  geometry->SetBounds(slicedgeometry1->GetBounds());
  geometry->SetIndexToWorldTransform(slicedgeometry1->GetIndexToWorldTransform());

  // Initialize planes
  for (int i = 0; i < (int)numSlices; i++)
  {
    mitk::PlaneGeometry::Pointer geo2d = mitk::PlaneGeometry::New();
    geo2d->Initialize();
    geo2d->SetReferenceGeometry(geometry);
    slicedgeometry1->SetPlaneGeometry(geo2d, i);
  }

    mitk::Vector3D firstAxisVector;
    mitk::FillVector3D(firstAxisVector, 100.0, 0.0, 0.0);

    mitk::Vector3D secondAxisVector;
    mitk::FillVector3D(secondAxisVector, 0.0, -50.0, 0.0);

    mitk::Vector3D thirdAxisVector;
    mitk::FillVector3D(thirdAxisVector, 0.0, 0.0, -200.0);

    std::cout << "Axial view direction" << std::endl;
    CPPUNIT_ASSERT(this->validateGeometry(sliceNavigationController->GetCurrentGeometry3D(), origin, firstAxisVector, secondAxisVector, thirdAxisVector));
  }

  void validateCoronalViewDirection()
  {
    auto sliceNavigationController = mitk::SliceNavigationController::New();

    sliceNavigationController->SetInputWorldGeometry3D(m_Geometry3D);
    sliceNavigationController->SetViewDirection(mitk::SliceNavigationController::Frontal);
    sliceNavigationController->Update();

    mitk::Point3D origin;
    mitk::FillVector3D(origin, 10.0, 69.5, 30.0);

    mitk::Vector3D firstAxisVector;
    mitk::FillVector3D(firstAxisVector, 100.0, 0.0, 0.0);

    mitk::Vector3D secondAxisVector;
    mitk::FillVector3D(secondAxisVector, 0.0, 0.0, 200.0);

    mitk::Vector3D thirdAxisVector;
    mitk::FillVector3D(thirdAxisVector, 0.0, -50.0, 0.0);

    std::cout << "Coronal view direction" << std::endl;
    CPPUNIT_ASSERT(this->validateGeometry(sliceNavigationController->GetCurrentGeometry3D(), origin, firstAxisVector, secondAxisVector, thirdAxisVector));
  }

  void validateSagittalViewDirection()
  {
    auto sliceNavigationController = mitk::SliceNavigationController::New();

    sliceNavigationController->SetInputWorldGeometry3D(m_Geometry3D);
    sliceNavigationController->SetViewDirection(mitk::SliceNavigationController::Sagittal);
    sliceNavigationController->Update();

int testRestorePlanePostionOperation()
{
  // Create PlaneGeometry
  mitk::PlaneGeometry::Pointer planegeometry = mitk::PlaneGeometry::New();

  mitk::Point3D origin;
  mitk::Vector3D right, bottom, normal;
  mitk::ScalarType width, height;
  mitk::ScalarType widthInMM, heightInMM, thicknessInMM;

  width = 100;
  widthInMM = width;
  height = 200;
  heightInMM = height;
  thicknessInMM = 1.5;

  mitk::FillVector3D(origin, 4.5, 7.3, 11.2);
  mitk::FillVector3D(right, widthInMM, 0, 0);
  mitk::FillVector3D(bottom, 0, heightInMM, 0);
  mitk::FillVector3D(normal, 0, 0, thicknessInMM);

  mitk::Vector3D spacing;
  normal.Normalize();
  normal *= thicknessInMM;
  mitk::FillVector3D(spacing, 1.0, 1.0, thicknessInMM);
  planegeometry->InitializeStandardPlane(right.GetVnlVector(), bottom.GetVnlVector(), &spacing);
  planegeometry->SetOrigin(origin);

  // Create SlicedGeometry3D out of planeGeometry
  mitk::SlicedGeometry3D::Pointer slicedgeometry1 = mitk::SlicedGeometry3D::New();
  unsigned int numSlices = 300;
  slicedgeometry1->InitializeEvenlySpaced(planegeometry, thicknessInMM, numSlices);

  // Create another slicedgeo which will be rotated
  mitk::SlicedGeometry3D::Pointer slicedgeometry2 = mitk::SlicedGeometry3D::New();
  slicedgeometry2->InitializeEvenlySpaced(planegeometry, thicknessInMM, numSlices);

  // Create  geo3D as reference
  mitk::Geometry3D::Pointer geometry = mitk::Geometry3D::New();
  geometry->SetBounds(slicedgeometry1->GetBounds());
  geometry->SetIndexToWorldTransform(slicedgeometry1->GetIndexToWorldTransform());

  // Initialize planes
  for (int i = 0; i < (int)numSlices; i++)
  {
    mitk::PlaneGeometry::Pointer geo2d = mitk::PlaneGeometry::New();
    geo2d->Initialize();
    geo2d->SetReferenceGeometry(geometry);
    slicedgeometry1->SetPlaneGeometry(geo2d, i);
  }

    mitk::Vector3D firstAxisVector;
    mitk::FillVector3D(firstAxisVector, 0.0, 50.0, 0.0);

    mitk::Vector3D secondAxisVector;
    mitk::FillVector3D(secondAxisVector, 0.0, 0.0, 200.0);

    mitk::Vector3D thirdAxisVector;
    mitk::FillVector3D(thirdAxisVector, 100.0, 0.0, 0.0);

    std::cout << "Sagittal view direction" << std::endl;
    CPPUNIT_ASSERT(this->validateGeometry(sliceNavigationController->GetCurrentGeometry3D(), origin, firstAxisVector, secondAxisVector, thirdAxisVector));
  }

private:
  bool validateGeometry(mitk::BaseGeometry::ConstPointer geometry, const mitk::Point3D &origin, const mitk::Vector3D &firstAxisVector, const mitk::Vector3D &secondAxisVector, const mitk::Vector3D &thirdAxisVector)
  {
    bool result = true;

    std::cout << "  Origin" << std::endl;

int mitkSliceNavigationControllerTest(int /*argc*/, char * /*argv*/ [])
{
  int result = EXIT_FAILURE;

  std::cout << "Creating and initializing a PlaneGeometry: ";
  mitk::PlaneGeometry::Pointer planegeometry = mitk::PlaneGeometry::New();

  mitk::Point3D origin;
  mitk::Vector3D right, bottom, normal;
  mitk::ScalarType width, height;
  mitk::ScalarType widthInMM, heightInMM, thicknessInMM;

  width = 100;
  widthInMM = width;
  height = 200;
  heightInMM = height;
  thicknessInMM = 1.5;
  //  mitk::FillVector3D(origin,         0,          0, thicknessInMM*0.5);
  mitk::FillVector3D(origin, 4.5, 7.3, 11.2);
  mitk::FillVector3D(right, widthInMM, 0, 0);
  mitk::FillVector3D(bottom, 0, heightInMM, 0);
  mitk::FillVector3D(normal, 0, 0, thicknessInMM);

  mitk::Vector3D spacing;
  normal.Normalize();
  normal *= thicknessInMM;
  mitk::FillVector3D(spacing, 1.0, 1.0, thicknessInMM);
  planegeometry->InitializeStandardPlane(right.GetVnlVector(), bottom.GetVnlVector(), &spacing);
  planegeometry->SetOrigin(origin);
  std::cout << "[PASSED]" << std::endl;

  std::cout << "Creating and initializing a SlicedGeometry3D with the PlaneGeometry: ";
  mitk::SlicedGeometry3D::Pointer slicedgeometry = mitk::SlicedGeometry3D::New();
  unsigned int numSlices = 5;
  slicedgeometry->InitializeEvenlySpaced(planegeometry, thicknessInMM, numSlices);
  std::cout << "[PASSED]" << std::endl;

  std::cout << "Creating a Geometry3D with the same extent as the SlicedGeometry3D: ";
  mitk::Geometry3D::Pointer geometry = mitk::Geometry3D::New();
  geometry->SetBounds(slicedgeometry->GetBounds());
  geometry->SetIndexToWorldTransform(slicedgeometry->GetIndexToWorldTransform());
  std::cout << "[PASSED]" << std::endl;

  mitk::Point3D cornerpoint0;
  cornerpoint0 = geometry->GetCornerPoint(0);

  result = testGeometry(
    geometry, width, height, numSlices, widthInMM, heightInMM, thicknessInMM, cornerpoint0, right, bottom, normal);
  if (result != EXIT_SUCCESS)
    return result;

    std::cout << "  First axis vector" << std::endl;

    if (!mitk::Equal(geometry->GetAxisVector(0), firstAxisVector, mitk::eps, true))
      result = false;

    std::cout << "  Second axis vector" << std::endl;

    if (!mitk::Equal(geometry->GetAxisVector(1), secondAxisVector, mitk::eps, true))
      result = false;

    std::cout << "  Third axis vector" << std::endl;

    if (!mitk::Equal(geometry->GetAxisVector(2), thirdAxisVector, mitk::eps, true))
      result = false;

    return result;
  }

  mitk::Geometry3D::Pointer m_Geometry3D;
};

MITK_TEST_SUITE_REGISTRATION(mitkSliceNavigationController)
