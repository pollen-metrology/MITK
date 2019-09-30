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

#ifndef QMITKDATANODEADDTOSEMANTICRELATIONSACTION_H
#define QMITKDATANODEADDTOSEMANTICRELATIONSACTION_H

#include <org_mitk_gui_qt_semanticrelations_Export.h>

// mitk gui qt application plugin
#include <QmitkAbstractDataNodeAction.h>

// qt
#include <QAction>

namespace AddToSemanticRelationsAction
{
  /**
  * @brief The function checks whether the given node is an image or a segmentation and calls the corresponding add function.
  *        The corresponding add functions will add the data node to the semantic relations storage.
  *        If an image is added, the 'AddImage' function will check if another image at the fitting control-point - information type cell
  *        already exists. If so, the user is prompted to overwrite the existing image or abort the process.
  *        If the user wants to overwrite the existing image, the image and it's corresponding segmentation nodes will be removed from the semantic relations storage.
  *        If a segmentation is added, the parent image node will also be to the semantic relations storage. If the segmentation does not contain the required DICOM information,
  *        the DICOM information of the parent data will be transfered to the segmentation data.
  *
  * @pre    The given dataStorage has to be valid (!nullptr).
  * @pre    The given dataNode has to be valid (!nullptr).
  *         The function simply returns if the preconditions are not met.
  *
  * @throw  SemanticRelationException re-thrown.
  *
  * @param dataStorage                    The data storage to use for to remove the existing image and to check for the parent image node of a segmentation.
  * @param dataNode                       The data node to add.
  */
  MITK_GUI_SEMANTICRELATIONS_EXPORT void Run(mitk::DataStorage* dataStorage, const mitk::DataNode* image);
  
  void AddImage(mitk::DataStorage* dataStorage, const mitk::DataNode* image);
  void AddSegmentation(mitk::DataStorage* dataStorage, const mitk::DataNode* segmentation);
}

class MITK_GUI_SEMANTICRELATIONS_EXPORT QmitkDataNodeAddToSemanticRelationsAction : public QAction, public QmitkAbstractDataNodeAction
{
  Q_OBJECT

public:

  QmitkDataNodeAddToSemanticRelationsAction(QWidget* parent, berry::IWorkbenchPartSite::Pointer workbenchPartSite);
  QmitkDataNodeAddToSemanticRelationsAction(QWidget* parent, berry::IWorkbenchPartSite* workbenchPartSite);

private Q_SLOTS:

  void OnActionTriggered(bool);

protected:

  virtual void InitializeAction() override;

};

#endif // QMITKDATANODEADDTOSEMANTICRELATIONSACTION_H
