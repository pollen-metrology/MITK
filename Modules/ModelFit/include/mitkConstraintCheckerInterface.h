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

#ifndef CONSTRAINTCHECKER_INTERFACE_H
#define CONSTRAINTCHECKER_INTERFACE_H

#include "mitkModelBase.h"

#include "MitkModelFitExports.h"

namespace mitk
{
  /** Interface class that is implemented by all constraint checkers that are used in the context of model fitting.
   * @remark All functions of the interface must be implemented thread save because it will be used in a multi threaded
   * environment.
  */
  class MITKMODELFIT_EXPORT ConstraintCheckerInterface
  {
  public:

    typedef ConstraintCheckerInterface Self;

    typedef double PenaltyValueType;
    typedef itk::Array<PenaltyValueType> PenaltyArrayType;
    typedef itk::Array<double> SignalType;
    typedef ModelBase::ParametersType ParametersType;

    /** Returns the penalties for all defined constraints.
    * @pre defined constraints must address valid parameters in the passed parameters set.
    * @post the result vector has the size given by GetNumberOfConstraints().
    */
    virtual PenaltyArrayType GetPenalties(const ParametersType &parameters) const = 0;

    /** Returns the sum of all penalties.
    * @pre defined constraints must address valid parameters in the passed parameters set.
    */
    virtual PenaltyValueType GetPenaltySum(const ParametersType &parameters) const = 0;

    /** Returns the number of defined constraints, that will be checked. Thus also the size of the penalty vector.*/
    virtual unsigned int GetNumberOfConstraints() const = 0;

    /** Returns the penalty value that indicates at least one failed constrained.*/
    virtual PenaltyValueType GetFailedConstraintValue() const = 0;

  protected:

    ConstraintCheckerInterface()
    {}

    virtual ~ConstraintCheckerInterface()
    {}

  private:
    ConstraintCheckerInterface(const ConstraintCheckerInterface& source);
    void operator=(const ConstraintCheckerInterface&);  //purposely not implemented
  };

}

#endif // MODELFITCOSTFUNCTION_H
