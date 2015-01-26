#include "DesignEditor.h"

#include <iostream>

#include <QWizardPage>
#include <QGridLayout>
#include <QVBoxLayout>

#include "qcustomplot.h"

#include "RtDesignMatrix.h"

using std::cout;
using std::endl;

DesignEditor::DesignEditor(QWidget *parent, RtDesignMatrix *design)
  : QWizard(parent)
  , design(design)
  , edit_plot(new QCustomPlot)
  , condition_names(NULL)
  , selected_column(-1)
{
  edit_plot->xAxis->setRange(0, design->getNumRows());
  edit_plot->yAxis->setRange(-1, 1);
  edit_plot->legend->setVisible(true);

  connect(edit_plot, SIGNAL(mousePress(QMouseEvent*)),
          this, SLOT(handleConditionClick(QMouseEvent*)));

  addPage(createMeasPage());
  addPage(createEditPage());
  setWindowTitle("Design editor");
}

DesignEditor::~DesignEditor() {}

void DesignEditor::addCondition(QString name) {
  selected_column = design->getNumInputConditions();

  if (selected_column == 0) {
    condition_names->removeItem(1);
  }

  design->addCondition(name.toStdString(), true);

  condition_names->addItem(name);

  edit_plot->addGraph();
  edit_plot->graph(selected_column)->setName(name);

  // TODO set the color

  edit_plot->replot();

  condition_names->setCurrentIndex(selected_column + 1);
}

void DesignEditor::handleConditionClick(QMouseEvent *event) {
  if (selected_column < 0) {
    return;
  }

  double x = edit_plot->xAxis->pixelToCoord(event->pos().x());
  double y = edit_plot->yAxis->pixelToCoord(event->pos().y());

  int tr = rint(x);
  design->setConditionValueAtTR(tr, selected_column, y);

}

QWizardPage* DesignEditor::createMeasPage() {
  QWizardPage *page = new QWizardPage;
  page->setTitle("Setup measurement details");

  QLabel *rep_time_label = new QLabel("Repetition time (ms):");
  QSpinBox *rep_time = new QSpinBox;
  rep_time->setRange(1, 10000);
  rep_time->setValue(2000);

  QLabel *num_meas_label = new QLabel("Number of measurements:");
  QSpinBox *num_meas = new QSpinBox;
  num_meas->setRange(1, 10000);
  num_meas->setValue(100);

  QGridLayout *layout = new QGridLayout;
  layout->addWidget(rep_time_label, 0, 0);
  layout->addWidget(rep_time, 0, 1);
  layout->addWidget(num_meas_label, 1, 0);
  layout->addWidget(num_meas, 1, 1);

  page->setLayout(layout);
  return page;
}

QWizardPage* DesignEditor::createEditPage() {
  QWizardPage *page = new QWizardPage;
  page->setTitle("Edit condition vectors");

  condition_names = new QComboBox;
  condition_names->setDuplicatesEnabled(false);

  condition_names->addItem("Add new condition");

  if (design->getNumColumns() == 0) {
    condition_names->addItem("No conditions");
    condition_names->setCurrentIndex(1);
  }

  connect(condition_names, SIGNAL(currentIndexChanged(int)),
          this, SLOT(setSelectedColumn(int)));

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addWidget(condition_names, 1);
  layout->addWidget(edit_plot, 10);

  page->setLayout(layout);
  return page;
}

void DesignEditor::setSelectedColumn(int col) {
  if (col == 0) {
    bool ok;
    QString text = QInputDialog::getText(
      this, "Enter condition name", "Condition name: ",
      QLineEdit::Normal, "", &ok);

    if (ok && !text.isEmpty()) {
      addCondition(text);
    }
  }
  else {
    selected_column = col - 1;
  }
}
