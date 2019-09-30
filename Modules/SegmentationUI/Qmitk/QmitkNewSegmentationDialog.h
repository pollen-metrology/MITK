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

#ifndef QmitkNewSegmentationDialog_h_Included
#define QmitkNewSegmentationDialog_h_Included

#include "mitkColorProperty.h"
#include <MitkSegmentationUIExports.h>

#include <qdialog.h>

#include <QCompleter>

class QLabel;
class QLineEdit;
class Q3ListBox;
class QPushButton;

#include <mitkCommon.h>

/**
  \brief Dialog for QmitkInteractiveSegmentation.

  \ingroup ToolManagerEtAl
  \ingroup Widgets

  This dialog is used to ask a user about the type of a newly created segmentation and a name for it.
*/
class MITKSEGMENTATIONUI_EXPORT QmitkNewSegmentationDialog : public QDialog
{
  Q_OBJECT

public:
  QmitkNewSegmentationDialog(QWidget *parent = nullptr);

  ~QmitkNewSegmentationDialog() override;

  const QString GetSegmentationName();
  mitk::Color GetColor();
  const char *GetOrganType();

  void SetSegmentationName(const QString &segmentationName);
  void SetColor(const mitk::Color &color);
  void SetSuggestionList(QStringList organColorList);

signals:

public slots:

  void setPrompt(const QString &prompt);

protected slots:

  void onAcceptClicked();
  void onNewOrganNameChanged(const QString &);
  void onColorBtnClicked();
  void onColorChange(const QString &completedWord);

protected:

  QLabel *lblPrompt;
  Q3ListBox *lstOrgans;
  QLineEdit *lineEditName;

  QPushButton *btnColor;
  QPushButton *btnOk;

  QLineEdit *edtNewOrgan;

  QString selectedOrgan;

  bool newOrganEntry;

  QColor m_Color;

  QCompleter *completer;

  QString m_SegmentationName;

  QStringList organList;
  QList<QColor> colorList;
};

#endif
