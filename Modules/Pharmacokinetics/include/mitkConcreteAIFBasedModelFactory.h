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
#ifndef MITK_CONCRETE_AIF_BASED_MODEL_FACTORY_H
#define MITK_CONCRETE_AIF_BASED_MODEL_FACTORY_H

#include "mitkConcreteModelFactoryBase.h"
#include "mitkAIFParametrizerHelper.h"

namespace mitk
{

  template <class TModelParameterizer>
  class ConcreteAIFBasedModelFactory : public
    mitk::ConcreteModelFactoryBase<typename TModelParameterizer::ModelType>
  {
  public:
    mitkClassMacro(ConcreteAIFBasedModelFactory,
                   ConcreteModelFactoryBase<typename TModelParameterizer::ModelType>);

    typedef typename Superclass::ModelType ModelType;
    typedef TModelParameterizer ModelParameterizerType;

  protected:
    virtual ModelParameterizerBase::Pointer DoCreateParameterizer(const modelFit::ModelFitInfo* fit)
    const
    {
      mitk::ModelParameterizerBase::Pointer result;

      typename ModelParameterizerType::Pointer modelParameterizer =
        ModelParameterizerType::New();

      modelFit::StaticParameterMap::ValueType aif = fit->staticParamMap.Get(
            ModelType::NAME_STATIC_PARAMETER_AIF);

      modelParameterizer->SetAIF(mitk::convertParameterToArray(aif));

      modelFit::StaticParameterMap::ValueType aifGrid = fit->staticParamMap.Get(
            ModelType::NAME_STATIC_PARAMETER_AIFTimeGrid);
      modelParameterizer->SetAIFTimeGrid(mitk::convertParameterToArray(aifGrid));

      result = modelParameterizer.GetPointer();

      return result;
    };

    ConcreteAIFBasedModelFactory()
    {
    };

    virtual ~ConcreteAIFBasedModelFactory()
    {
    };

  private:

    //No copy constructor allowed
    ConcreteAIFBasedModelFactory(const Self& source);
    void operator=(const Self&);  //purposely not implemented

  };

}
#endif // MITKTWOCOMPARTMENTEXCHANGEMODELFACTORY_H
