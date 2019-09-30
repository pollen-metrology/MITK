#ifndef MITKNUMERICTWOCOMPARTMENTEXCHANGEMODEL_H
#define MITKNUMERICTWOCOMPARTMENTEXCHANGEMODEL_H

#include "mitkAIFBasedModelBase.h"
#include "MitkPharmacokineticsExports.h"


namespace mitk
{
  /** @class NumericTwoCompartmentExchangeModel
   * @brief Implementation of the numeric model function of the 2 Compartment Exchange model, using an Aterial Input Function
   * The Model calculates the measured Concentration-Time-Curve from the mass balance equations of the 2-tissue compartent Model
   *
   * vp * dCp(t)/dt =  F * (CA(t) - Cp(t)) - PS * (Cp(t) - Ci(t))
   * ve * dCi(t)/dt = PS * (Cp(t) - Ci(t))
   *
   * with concentration curve Cp(t) of the Blood Plasma p and Ce(t) of the Extracellular Extravascular Space(EES)(interstitial volume). CA(t) is the aterial concentration, i.e. the AIF
   * Cp(t) and Ce(t) are found numerical via Runge-Kutta methode, implemented in Boosts numeric library ODEINT. Here we use a runge_kutta_cash_karp54 stepper with
   * adaptive step size and error controll.
   * From the resulting curves Cp(t) and Ce(t) the measured concentration Ctotal(t) is found vial
   *
   * Ctotal(t) = vp * Cp(t) + ve * Ce(t)
   *
   * where vp=Vp/VT and ve=Ve/VT are the portion of Plasma/EES volume Vp/Ve of the total volume VT respectively.
   * The parameters PS, F,  vp and ve are subject to the fitting routine*/

  class MITKPHARMACOKINETICS_EXPORT NumericTwoCompartmentExchangeModel : public AIFBasedModelBase
  {

  public:
    typedef NumericTwoCompartmentExchangeModel Self;
    typedef AIFBasedModelBase Superclass;
    typedef itk::SmartPointer< Self >                            Pointer;
    typedef itk::SmartPointer< const Self >                      ConstPointer;


    /** Method for creation through the object factory. */
    itkFactorylessNewMacro(Self)
    itkCloneMacro(Self)

    /** Run-time type information (and related methods). */
    itkTypeMacro(NumericTwoCompartmentExchangeModel, ModelBase)

    typedef std::vector<double> state_type;


    static const std::string MODEL_DISPLAY_NAME;

    static const std::string NAME_PARAMETER_F;
    static const std::string NAME_PARAMETER_PS;
    static const std::string NAME_PARAMETER_ve;
    static const std::string NAME_PARAMETER_vp;
    static const std::string NAME_STATIC_PARAMETER_ODEINTStepSize;

    static const std::string UNIT_PARAMETER_F;
    static const std::string UNIT_PARAMETER_PS;
    static const std::string UNIT_PARAMETER_ve;
    static const std::string UNIT_PARAMETER_vp;

    static const unsigned int POSITION_PARAMETER_F;
    static const unsigned int POSITION_PARAMETER_PS;
    static const unsigned int POSITION_PARAMETER_ve;
    static const unsigned int POSITION_PARAMETER_vp;

    static const unsigned int NUMBER_OF_PARAMETERS;

    virtual std::string GetModelDisplayName() const override;

    virtual std::string GetModelType() const override;

    itkGetConstReferenceMacro(ODEINTStepSize, double);
    itkSetMacro(ODEINTStepSize, double);


    virtual ParameterNamesType GetParameterNames() const override;
    virtual ParametersSizeType  GetNumberOfParameters() const override;

    virtual ParamterUnitMapType GetParameterUnits() const override;

    virtual ParameterNamesType GetStaticParameterNames() const;
    virtual ParametersSizeType GetNumberOfStaticParameters() const;


  protected:
    NumericTwoCompartmentExchangeModel();
    virtual ~NumericTwoCompartmentExchangeModel();

    /**
     * Actual implementation of the clone method. This method should be reimplemeted
     * in subclasses to clone the extra required parameters.
     */
    virtual itk::LightObject::Pointer InternalClone() const;

    virtual ModelResultType ComputeModelfunction(const ParametersType& parameters) const override;

    virtual void SetStaticParameter(const ParameterNameType& name, const StaticParameterValuesType& values);
    virtual StaticParameterValuesType GetStaticParameterValue(const ParameterNameType& name) const;

    virtual void PrintSelf(std::ostream& os, ::itk::Indent indent) const;

  private:



    //No copy constructor allowed
    NumericTwoCompartmentExchangeModel(const Self& source);
    void operator=(const Self&);  //purposely not implemented

    double m_ODEINTStepSize;



  };
}

#endif // MITKNUMERICTWOCOMPARTMENTEXCHANGEMODEL_H
