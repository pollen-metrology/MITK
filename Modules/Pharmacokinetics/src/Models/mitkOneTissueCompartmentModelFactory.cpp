#include "mitkOneTissueCompartmentModelFactory.h"

#include <mitkImageTimeSelector.h>
#include <mitkImageCast.h>
#include <mitkSimpleBarrierConstraintChecker.h>

mitk::ConstraintCheckerBase::Pointer
mitk::OneTissueCompartmentModelFactory::CreateDefaultConstraints() const
{
  SimpleBarrierConstraintChecker::Pointer constraints = SimpleBarrierConstraintChecker::New();

  constraints->SetLowerBarrier(ModelType::POSITION_PARAMETER_k1, 0.0);
  constraints->SetLowerBarrier(ModelType::POSITION_PARAMETER_k2, 0.0);

  constraints->SetUpperBarrier (ModelType::POSITION_PARAMETER_k1, 1.0);
  constraints->SetUpperBarrier(ModelType::POSITION_PARAMETER_k2, 1.0);


  return constraints.GetPointer();
};

mitk::ModelParameterizerBase::ParametersType
mitk::OneTissueCompartmentModelFactory::GetDefaultInitialParameterization() const
{
  return OneTissueCompartmentModelParameterizer::New()->GetDefaultInitialParameterization();
};

mitk::OneTissueCompartmentModelFactory::OneTissueCompartmentModelFactory()
{
};

mitk::OneTissueCompartmentModelFactory::~OneTissueCompartmentModelFactory()
{
};
