#include "DesignEditor.h"

#include <QWizardPage>
//#include <QHorizontalLayout>

#include "qcustomplot.h"

#include "RtDesignMatrix.h"

DesignEditor::DesignEditor(QWidget *parent, RtDesignMatrix *design)
  : QWizard(parent)
  , design(design)
  , edit_plot(new QCustomPlot)
  , selected_column(0)
{
  edit_plot->xAxis->setRange(0, design->getNumRows());
  edit_plot->yAxis->setRange(-1, 1);
  edit_plot->legend->setVisible(true);
}

DesignEditor::~DesignEditor() {}

void DesignEditor::addCondition(QString name) {
  design->addCondition(name.toStdString(), true);
  selected_column = design->getNumInputConditions();

  edit_plot->addGraph();
  edit_plot->graph(selected_column)->setName(name);

  // TODO set the color

  edit_plot->replot();
}
