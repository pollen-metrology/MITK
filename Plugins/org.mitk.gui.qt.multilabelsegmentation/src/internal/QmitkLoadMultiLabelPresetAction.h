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
#ifndef QMITK_QmitkLoadMultiLabelPresetAction_H
#define QMITK_QmitkLoadMultiLabelPresetAction_H

#include "mitkIContextMenuAction.h"

#include "org_mitk_gui_qt_multilabelsegmentation_Export.h"

#include "vector"
#include "mitkDataNode.h"

class MITK_QT_SEGMENTATION QmitkLoadMultiLabelPresetAction : public QObject, public mitk::IContextMenuAction
{
  Q_OBJECT
  Q_INTERFACES(mitk::IContextMenuAction)

public:

  QmitkLoadMultiLabelPresetAction();
  virtual ~QmitkLoadMultiLabelPresetAction();

  //interface methods
  virtual void Run( const QList<mitk::DataNode::Pointer>& selectedNodes );
  virtual void SetDataStorage(mitk::DataStorage* dataStorage);
  virtual void SetFunctionality(berry::QtViewPart* functionality);
  virtual void SetSmoothed(bool smoothed);
  virtual void SetDecimated(bool decimated);

private:

  typedef QList<mitk::DataNode::Pointer> NodeList;

  mitk::DataStorage::Pointer m_DataStorage;
};

#endif // QMITK_CreateMultiLabelSegmentation_H
