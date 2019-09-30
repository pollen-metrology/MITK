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

#ifndef FitGeneratorDemoView_h
#define FitGeneratorDemoView_h

#include <QString>

#include <QmitkAbstractView.h>

#include <itkCommand.h>

#include "ui_FitGeneratorDemoView.h"
#include <mitkModelBase.h>
#include <mitkModelFactoryBase.h>
#include <QmitkParameterFitBackgroundJob.h>
#include <mitkModelFitResultHelper.h>

/*!
*	@brief Test Plugin for generation of model parameter images
*/
class FitGeneratorDemoView : public QmitkAbstractView
{
  Q_OBJECT

public:

  /*! @brief The view's unique ID - required by MITK */
  static const std::string VIEW_ID;

  FitGeneratorDemoView();

  protected slots:

    void OnModellingButtonClicked();
    void OnGenerateTestDataButtonClicked();
    void OnJobFinished();
    void OnJobError(QString err);
    void OnJobResultsAreAvailable(mitk::modelFit::ModelFitResultNodeVectorType results, const ParameterFitBackgroundJob* pJob);
    void OnJobProgress(double progress);
    void OnJobStatusChanged(QString info);

protected:

  // Overridden base class functions

  /*!
  *	@brief					Sets up the UI controls and connects the slots and signals. Gets
  *							called by the framework to create the GUI at the right time.
  *	@param[in,out] parent	The parent QWidget, as this class itself is not a QWidget
  *							subclass.
  */
  void CreateQtPartControl(QWidget* parent);

  /*!
  *	@brief	Sets the focus to the plot curve button. Gets called by the framework to set the
  *			focus on the right widget.
  */
  void SetFocus();

  void Generate();

  /*! \brief called by QmitkFunctionality when DataManager's selection has changed
  */
  virtual void OnSelectionChanged( berry::IWorkbenchPart::Pointer source,
    const QList<mitk::DataNode::Pointer>& nodes);

  Ui::FitGeneratorDemoViewControls m_Controls;

  mitk::DataNode::Pointer m_selectedNode;
  mitk::Image::Pointer m_selectedImage;

  mitk::DataNode::Pointer m_selectedMaskNode;
  mitk::Image::Pointer m_selectedMask;

  mitk::ModelFactoryBase::Pointer m_ModelFactory;

};

#endif
