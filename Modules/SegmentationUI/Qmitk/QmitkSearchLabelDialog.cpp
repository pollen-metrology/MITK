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

#include "QmitkSearchLabelDialog.h"

#include "mitkOrganTypeProperty.h"

#include <itkRGBPixel.h>

#include <QAbstractItemModel>
#include <QColorDialog>
#include <QStringListModel>
#include <qlabel.h>
#include <qlayout.h>
#include <qlineedit.h>
#include <qpushbutton.h>

QmitkSearchLabelDialog::QmitkSearchLabelDialog(QWidget *parent, Qt::WindowFlags f) : QDialog(parent, f)
{
  m_Controls = new Ui::QmitkSearchLabelDialogGUI();
  m_Controls->setupUi(this);

  m_LabelIndex = -1;

  QStringList completionList;
  completionList << "";
  m_Completer = new QCompleter(completionList);
  m_Completer->setCaseSensitivity(Qt::CaseInsensitive);
  m_Controls->m_LabelName->setCompleter(m_Completer);

  connect(m_Completer, SIGNAL(activated(const QString &)), this, SLOT(OnLabelCompleterChanged(const QString &)));
  connect(m_Controls->m_pbCancel, SIGNAL(clicked()), this, SLOT(reject()));
  connect(m_Controls->m_pbAccept, SIGNAL(clicked()), this, SLOT(accept()));

  m_Controls->m_LabelName->setFocus();
}

QmitkSearchLabelDialog::~QmitkSearchLabelDialog()
{
}

int QmitkSearchLabelDialog::GetLabelSetWidgetTableIndex()
{
  return m_LabelIndex;
}

QString QmitkSearchLabelDialog::GetLabelSetWidgetTableCompleteWord()
{
  return m_CompleteWord;
}

void QmitkSearchLabelDialog::SetLabelSuggestionList(QStringList stringList)
{
  m_LabelList = stringList;
  QStringListModel *completeModel = static_cast<QStringListModel *>(m_Completer->model());
  completeModel->setStringList(m_LabelList);
}

void QmitkSearchLabelDialog::OnLabelCompleterChanged(const QString &completedWord)
{
  if (m_LabelList.contains(completedWord))
  {
    m_LabelIndex = m_LabelList.indexOf(completedWord);
    m_CompleteWord = completedWord;
    //    emit(goToLabel(m_LabelIndex));
  }
}
