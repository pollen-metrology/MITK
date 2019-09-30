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

#include "mitkT2DecayModel.h"

std::string mitk::T2DecayModel::GetModelDisplayName() const
{
  return "T2 Decay Model";
};

std::string mitk::T2DecayModel::GetModelType() const
{
  return "MRSignal";
};

mitk::T2DecayModel::FunctionStringType mitk::T2DecayModel::GetFunctionString() const
{
  return "M0 * exp(-t/T2)";
};

std::string mitk::T2DecayModel::GetXName() const
{
  return "t";
};

mitk::T2DecayModel::ParameterNamesType
mitk::T2DecayModel::GetParameterNames() const
{
  ParameterNamesType result;
  result.push_back("M0");
  result.push_back("T2");
  return result;
};

mitk::T2DecayModel::ParametersSizeType
mitk::T2DecayModel::GetNumberOfParameters() const
{
  return 2;
};

mitk::T2DecayModel::ModelResultType
mitk::T2DecayModel::ComputeModelfunction(const ParametersType& parameters) const
{
  ModelResultType signal(m_TimeGrid.GetSize());

  ModelResultType::iterator signalPos = signal.begin();

  for (const auto& gridPos : m_TimeGrid)
  {
    *signalPos = parameters[0] * exp(-1.0 * gridPos/ parameters[1]);
  }

  return signal;
};

mitk::T2DecayModel::ParameterNamesType mitk::T2DecayModel::GetStaticParameterNames() const
{
  ParameterNamesType result;

  return result;
}

mitk::T2DecayModel::ParametersSizeType  mitk::T2DecayModel::GetNumberOfStaticParameters() const
{
  return 0;
}

void mitk::T2DecayModel::SetStaticParameter(const ParameterNameType& /*name*/,
    const StaticParameterValuesType& /*values*/)
{
  //do nothing
};

mitk::T2DecayModel::StaticParameterValuesType mitk::T2DecayModel::GetStaticParameterValue(
  const ParameterNameType& /*name*/) const
{
  StaticParameterValuesType result;

  //do nothing

  return result;
};

itk::LightObject::Pointer mitk::T2DecayModel::InternalClone() const
{
  T2DecayModel::Pointer newClone = T2DecayModel::New();

  newClone->SetTimeGrid(this->m_TimeGrid);

  return newClone.GetPointer();
};
