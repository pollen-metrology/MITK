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

#ifndef MITKTWOTISSUECOMPARTMENTMODELPARAMETERIZER_H
#define MITKTWOTISSUECOMPARTMENTMODELPARAMETERIZER_H

#include "mitkAIFBasedModelParameterizerBase.h"
#include "mitkTwoTissueCompartmentModel.h"

#include "MitkPharmacokineticsExports.h"

namespace mitk
{

  class MITKPHARMACOKINETICS_EXPORT TwoTissueCompartmentModelParameterizer : public
    mitk::AIFBasedModelParameterizerBase<mitk::TwoTissueCompartmentModel>
  {
  public:
    typedef TwoTissueCompartmentModelParameterizer Self;
    typedef mitk::AIFBasedModelParameterizerBase<mitk::TwoTissueCompartmentModel> Superclass;
    typedef itk::SmartPointer< Self >                            Pointer;
    typedef itk::SmartPointer< const Self >                      ConstPointer;

    itkTypeMacro(TwoTissueCompartmentModelParameterizer,
                 mitk::AIFBasedModelParameterizerBase<mitk::TwoTissueCompartmentModel>);
    itkFactorylessNewMacro(Self);

    typedef Superclass::ModelBaseType ModelBaseType;
    typedef Superclass::ModelBasePointer ModelBasePointer;

    typedef Superclass::ModelType ModelType;
    typedef ModelType::Pointer ModelPointer;

    typedef Superclass::StaticParameterValueType StaticParameterValueType;
    typedef Superclass::StaticParameterValuesType StaticParameterValuesType;
    typedef  Superclass::StaticParameterMapType StaticParameterMapType;

    typedef Superclass::IndexType IndexType;

    /** This function returns the default parameterization (e.g. initial parametrization for fitting)
     defined by the model developer for  for the given model.*/
    virtual ParametersType GetDefaultInitialParameterization() const;

  protected:
    TwoTissueCompartmentModelParameterizer();

    virtual ~TwoTissueCompartmentModelParameterizer();

  private:

    //No copy constructor allowed
    TwoTissueCompartmentModelParameterizer(const Self& source);
    void operator=(const Self&);  //purposely not implemented
  };
}
#endif
