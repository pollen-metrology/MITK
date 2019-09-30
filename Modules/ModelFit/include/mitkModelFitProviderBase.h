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

#ifndef __MODEL_FIT_PROVIDER_BASE_H
#define __MODEL_FIT_PROVIDER_BASE_H

#include <memory>

// MITK
#include <mitkIModelFitProvider.h>

// Microservices
#include <usGetModuleContext.h>
#include <usServiceProperties.h>
#include <usServiceRegistration.h>

namespace mitk
{
  /**
   * @brief Base class for model fit provider.
   */
  template<class TModelFactory>
  class ModelFitProviderBase : public mitk::IModelFitProvider
  {
  public:
    /** Returns an instance of the model factory that is represented by the provider.*/
    virtual itk::SmartPointer<ModelFactoryBase> GenerateFactory() const override;

    /** Returns the grid of the model variable extracted from the fit info. The default implementation
     returns a time grid extracted from the time geometry of the fitInfo->inputImage. Reimplement for
     other models/fits to generate other variable grids.
     @pre fitInfo is a valid instance for the model fit.
    */
    virtual ModelBase::TimeGridType GetVariableGrid(const modelFit::ModelFitInfo* fitInfo) const override;

    us::ServiceRegistration<IModelFitProvider> RegisterService(us::ModuleContext *context = us::GetModuleContext());
    void UnregisterService();

    ModelFitProviderBase();
    virtual ~ModelFitProviderBase();

  protected:

    ModelFitProviderBase(const ModelFitProviderBase &other);

    virtual us::ServiceProperties GetServiceProperties() const;

    /**
     * \brief Set the service ranking for this file reader.
     *
     * Default is zero and should only be chosen differently for a reason.
     * The ranking is used to determine which reader to use if several
     * equivalent readers have been found.
     * It may be used to replace a default reader from MITK in your own project.
     * E.g. if you want to use your own reader for nrrd files instead of the default,
     * implement it and give it a higher ranking than zero.
     */
    void SetRanking(int ranking);
    int GetRanking() const;

  private:
    ModelFitProviderBase &operator=(const ModelFitProviderBase &other);

    class Impl;
    std::unique_ptr< Impl > d;
  };

} // namespace mitk


#ifndef ITK_MANUAL_INSTANTIATION
#include "mitkModelFitProviderBase.tpp"
#endif

#endif /* __MODEL_FIT_PROVIDER_BASE_H */
