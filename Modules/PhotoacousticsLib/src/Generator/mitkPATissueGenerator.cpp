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

#include "mitkPATissueGenerator.h"
#include "mitkPAVector.h"
#include "mitkPAVolumeManipulator.h"

mitk::pa::InSilicoTissueVolume::Pointer mitk::pa::InSilicoTissueGenerator::GenerateInSilicoData(
  TissueGeneratorParameters::Pointer parameters)
{
  MITK_DEBUG << "Initializing Empty Volume";

  const double RESAMPLING_FACTOR = 2;

  if (parameters->GetDoPartialVolume())
  {
    parameters->SetXDim(parameters->GetXDim() * RESAMPLING_FACTOR);
    parameters->SetYDim(parameters->GetYDim() * RESAMPLING_FACTOR);
    parameters->SetZDim(parameters->GetZDim() * RESAMPLING_FACTOR);
    parameters->SetVesselBifurcationFrequency(parameters->GetVesselBifurcationFrequency() * RESAMPLING_FACTOR);
    parameters->SetVoxelSpacingInCentimeters(parameters->GetVoxelSpacingInCentimeters() / RESAMPLING_FACTOR);
  }

  std::mt19937 randomNumberGenerator;
  std::random_device randomDevice;
  if (parameters->GetUseRngSeed())
  {
    randomNumberGenerator.seed(parameters->GetRngSeed());
  }
  else
  {
    if (randomDevice.entropy() > 0.1)
    {
      randomNumberGenerator.seed(randomDevice());
    }
    else
    {
      randomNumberGenerator.seed(std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count());
    }
  }

  auto generatedVolume = mitk::pa::InSilicoTissueVolume::New(parameters, &randomNumberGenerator);

  double DIRECTION_VECTOR_INITIAL_VARIANCE = 0.2;

  if(parameters->GetForceVesselsMoveAlongYDirection())
      DIRECTION_VECTOR_INITIAL_VARIANCE = 0;

  std::uniform_int_distribution<int> randomNumVesselDistribution(
              parameters->GetMinNumberOfVessels(), parameters->GetMaxNumberOfVessels());
  std::uniform_real_distribution<double> randomBendingDistribution(
              parameters->GetMinVesselBending(), parameters->GetMaxVesselBending());
  std::uniform_real_distribution<double> randomAbsorptionDistribution(
              parameters->GetMinVesselAbsorption(), parameters->GetMaxVesselAbsorption());
  std::uniform_real_distribution<double> randomRadiusDistribution(
              parameters->GetMinVesselRadiusInMillimeters(), parameters->GetMaxVesselRadiusInMillimeters());
  std::uniform_real_distribution<double> randomScatteringDistribution(
              parameters->GetMinVesselScattering(), parameters->GetMaxVesselScattering());
  std::uniform_real_distribution<double> randomAnisotropyDistribution(
              parameters->GetMinVesselAnisotropy(), parameters->GetMaxVesselAnisotropy());
  std::uniform_int_distribution<int> borderTypeDistribution(0, 3);

  int numberOfBloodVessels = randomNumVesselDistribution(randomNumberGenerator);

  generatedVolume->AddIntProperty("numberOfVessels", numberOfBloodVessels);
  generatedVolume->AddIntProperty("bifurcationFrequency", parameters->GetVesselBifurcationFrequency());

  MITK_INFO << "Simulating " << numberOfBloodVessels << " vessel structures";
  for (int vesselNumber = 0; vesselNumber < numberOfBloodVessels; vesselNumber++)
  {
    Vector::Pointer initialPosition = Vector::New();
    Vector::Pointer initialDirection = Vector::New();

    double initialRadius = randomRadiusDistribution(randomNumberGenerator) / parameters->GetVoxelSpacingInCentimeters() / 10;
    std::stringstream radiusString;
    radiusString << "vessel_" << vesselNumber + 1 << "_radius";
    generatedVolume->AddDoubleProperty(radiusString.str(), initialRadius);

    double absorptionCoefficient = randomAbsorptionDistribution(randomNumberGenerator);
    std::stringstream absorptionString;
    absorptionString << "vessel_" << vesselNumber + 1 << "_absorption";
    generatedVolume->AddDoubleProperty(absorptionString.str(), absorptionCoefficient);

    double bendingFactor = randomBendingDistribution(randomNumberGenerator);
    std::stringstream bendingString;
    bendingString << "vessel_" << vesselNumber + 1 << "_bendingFactor";
    generatedVolume->AddDoubleProperty(bendingString.str(), bendingFactor);

    double vesselScattering = randomScatteringDistribution(randomNumberGenerator);
    std::stringstream scatteringString;
    scatteringString << "vessel_" << vesselNumber + 1 << "_scattering";
    generatedVolume->AddDoubleProperty(scatteringString.str(), vesselScattering);

    double vesselAnisotropy = randomAnisotropyDistribution(randomNumberGenerator);
    std::stringstream anisotropyString;
    anisotropyString << "vessel_" << vesselNumber + 1 << "_anisotropy";
    generatedVolume->AddDoubleProperty(anisotropyString.str(), vesselAnisotropy);

    /*The vessel tree shall start at one of the 4 sides of the volume.
    * The vessels will always be completely contained in the volume
    * when starting to meander.
    * They will meander in a direction perpendicular to the
    * plane they started from (within the limits of the
    * DIRECTION_VECTOR_INITIAL_VARIANCE)
    */
    int borderType = borderTypeDistribution(randomNumberGenerator);

    if(parameters->GetForceVesselsMoveAlongYDirection())
        borderType = 2;

    switch (borderType)
    {
    case 0:
      initialPosition->Randomize(0, 0, initialRadius, parameters->GetYDim() - initialRadius,
                                 parameters->GetMinVesselZOrigin() / parameters->GetVoxelSpacingInCentimeters(),
        parameters->GetMaxVesselZOrigin() / parameters->GetVoxelSpacingInCentimeters(), &randomNumberGenerator);
      initialDirection->Randomize(1, 2, -DIRECTION_VECTOR_INITIAL_VARIANCE, DIRECTION_VECTOR_INITIAL_VARIANCE,
        -DIRECTION_VECTOR_INITIAL_VARIANCE, DIRECTION_VECTOR_INITIAL_VARIANCE, &randomNumberGenerator);
      break;
    case 1:
      initialPosition->Randomize(parameters->GetXDim() - 1, parameters->GetXDim() - 1, initialRadius, parameters->GetYDim() - initialRadius,
                                 parameters->GetMinVesselZOrigin() / parameters->GetVoxelSpacingInCentimeters(),
        parameters->GetMaxVesselZOrigin() / parameters->GetVoxelSpacingInCentimeters(), &randomNumberGenerator);
      initialDirection->Randomize(-2, -1, -DIRECTION_VECTOR_INITIAL_VARIANCE, DIRECTION_VECTOR_INITIAL_VARIANCE,
        -DIRECTION_VECTOR_INITIAL_VARIANCE, DIRECTION_VECTOR_INITIAL_VARIANCE, &randomNumberGenerator);
      break;
    case 2:
      initialPosition->Randomize(initialRadius, parameters->GetXDim() - initialRadius, 0, 0,
                                 parameters->GetMinVesselZOrigin() / parameters->GetVoxelSpacingInCentimeters(),
        parameters->GetMaxVesselZOrigin() / parameters->GetVoxelSpacingInCentimeters(), &randomNumberGenerator);
      initialDirection->Randomize(-DIRECTION_VECTOR_INITIAL_VARIANCE, DIRECTION_VECTOR_INITIAL_VARIANCE, 1, 2,
        -DIRECTION_VECTOR_INITIAL_VARIANCE, DIRECTION_VECTOR_INITIAL_VARIANCE, &randomNumberGenerator);
      break;
    case 3:
      initialPosition->Randomize(initialRadius, parameters->GetXDim() - initialRadius, parameters->GetYDim() - 1, parameters->GetYDim() - 1,
                                 parameters->GetMinVesselZOrigin() / parameters->GetVoxelSpacingInCentimeters(),
        parameters->GetMaxVesselZOrigin() / parameters->GetVoxelSpacingInCentimeters(), &randomNumberGenerator);
      initialDirection->Randomize(-DIRECTION_VECTOR_INITIAL_VARIANCE, DIRECTION_VECTOR_INITIAL_VARIANCE, -2, -1,
        -DIRECTION_VECTOR_INITIAL_VARIANCE, DIRECTION_VECTOR_INITIAL_VARIANCE, &randomNumberGenerator);
      break;
    }

    initialDirection->Normalize();
    MITK_INFO << initialPosition->GetElement(0) << " | " << initialPosition->GetElement(1) << " | " << initialPosition->GetElement(2);
    MITK_INFO << initialDirection->GetElement(0) << " | " << initialDirection->GetElement(1) << " | " << initialDirection->GetElement(2);

    VesselProperties::Pointer vesselParams = VesselProperties::New();
    vesselParams->SetDirectionVector(initialDirection);
    vesselParams->SetPositionVector(initialPosition);
    vesselParams->SetRadiusInVoxel(initialRadius);
    vesselParams->SetAbsorptionCoefficient(absorptionCoefficient);
    vesselParams->SetScatteringCoefficient(vesselScattering);
    vesselParams->SetAnisotopyCoefficient(vesselAnisotropy);
    vesselParams->SetBifurcationFrequency(parameters->GetVesselBifurcationFrequency());
    vesselParams->SetDoPartialVolume(parameters->GetDoPartialVolume());

    VesselTree::Pointer vesselTree = VesselTree::New(vesselParams);

    while (!vesselTree->IsFinished())
    {
      vesselTree->Step(generatedVolume, parameters->GetCalculateNewVesselPositionCallback(), bendingFactor, &randomNumberGenerator);
    }
  }

  if (parameters->GetDoPartialVolume())
  {
    VolumeManipulator::RescaleImage(generatedVolume, (1.0 / RESAMPLING_FACTOR));
    parameters->SetXDim(parameters->GetXDim() / RESAMPLING_FACTOR);
    parameters->SetYDim(parameters->GetYDim() / RESAMPLING_FACTOR);
    parameters->SetZDim(parameters->GetZDim() / RESAMPLING_FACTOR);
    parameters->SetVesselBifurcationFrequency(parameters->GetVesselBifurcationFrequency() / RESAMPLING_FACTOR);
    parameters->SetVoxelSpacingInCentimeters(parameters->GetVoxelSpacingInCentimeters() * RESAMPLING_FACTOR);
  }

  generatedVolume->FinalizeVolume();

  return generatedVolume;
}

mitk::pa::InSilicoTissueGenerator::InSilicoTissueGenerator()
{
}

mitk::pa::InSilicoTissueGenerator::~InSilicoTissueGenerator()
{
}
