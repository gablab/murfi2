#include "DesignEditor.h"

//
#include <iomanip>
#include <iostream>
#include <sstream>

#include <QWizardPage>
#include <QGridLayout>
#include <QLabel>
#include <QHBoxLayout>
#include <QVBoxLayout>

#include "qcustomplot.h"

#include "RtDesignMatrix.h"

using std::cout;
using std::endl;

namespace {
void plotDesignColumn(RtDesignMatrix *design, int col, QCPGraph *graph) {
  graph->clearData();
  for (int i = 0; i < design->getNumMeas(); i++) {
    graph->addData(i, design->getConditionValueAtTR(i, col));
  }
}
} // anonymous namespace

DesignEditor::DesignEditor(QWidget *parent, RtDesignMatrix *design)
  : QWizard(parent)
  , design(design)
  , edit_plot(new QCustomPlot)
  , condition_colormap(Colormap::ColormapType::DESIGN)
  , condition_names(NULL)
  , selected_column(-1)
  , mouse_pos_label(NULL)
  , condition_pos_label(NULL)
  , condition_increment(0.05)
  , max_y(1.0)
  , min_y(0.0)
  , mouse_down(false)
{
  edit_plot->xAxis->setRange(0, design->getNumRows());
  edit_plot->yAxis->setRange(min_y - 0.2, max_y + 0.2);
  edit_plot->legend->setVisible(true);

  connect(edit_plot, SIGNAL(mousePress(QMouseEvent*)),
          this, SLOT(handleMouseDown(QMouseEvent*)));

  connect(edit_plot, SIGNAL(mouseRelease(QMouseEvent*)),
          this, SLOT(handleMouseUp(QMouseEvent*)));

  connect(edit_plot, SIGNAL(mouseMove(QMouseEvent*)),
          this, SLOT(handleMouseMove(QMouseEvent*)));

  addPage(createMeasPage());
  addPage(createEditPage());
  setWindowTitle("Design editor");

  connect(this->button(QWizard::FinishButton), SIGNAL(clicked()),
          this, SLOT(finish()));
}

DesignEditor::~DesignEditor() {}

void DesignEditor::addCondition(QString name, bool existing) {
  if (!existing) {
    design->addCondition(name.toStdString(), true);
  }

  condition_names->addItem(name);

  edit_plot->addGraph();
  edit_plot->graph(selected_column)->setName(name);
  edit_plot->graph(selected_column)->setPen(
        QPen(condition_colormap.getColor(selected_column)));

  plotDesignColumn(design, selected_column, edit_plot->graph(selected_column));

  edit_plot->replot();

  condition_names->setCurrentIndex(selected_column + 1);
}

void DesignEditor::handleMouseDown(QMouseEvent *event) {
  mouse_down = true;

  if (selected_column < 0) {
    return;
  }

  int tr;
  double y;
  getTrAndYFromMousePos(event, &tr, &y);

  if (tr < 0 || tr >= design->getNumMeas()) {
    return;
  }

  if (y > max_y) {
    max_y = y;
    edit_plot->yAxis->setRange(min_y - 0.2, max_y + 0.2);
  }

  if (y < min_y) {
    min_y = y;
    edit_plot->yAxis->setRange(min_y - 0.2, max_y + 0.2);
  }

  design->setConditionValueAtTR(tr, selected_column, y);
  plotDesignColumn(design, selected_column, edit_plot->graph(selected_column));
  edit_plot->replot();

  setLabels(tr, design->getConditionValueAtTR(tr, selected_column), y);
}

void DesignEditor::handleMouseUp(QMouseEvent *event) {
  mouse_down = false;
}

void DesignEditor::handleMouseMove(QMouseEvent *event) {

  int tr;
  double y;
  getTrAndYFromMousePos(event, &tr, &y);

  if (tr < 0 || tr >= design->getNumMeas()) {
    return;
  }

  setLabels(tr, design->getConditionValueAtTR(tr, selected_column), y);

  if (!mouse_down) {
    return;
  }

  if (selected_column < 0) {
    return;
  }

  if (y > max_y) {
    max_y = y;
    edit_plot->yAxis->setRange(min_y - 0.2, max_y + 0.2);
  }

  if (y < min_y) {
    min_y = y;
    edit_plot->yAxis->setRange(min_y - 0.2, max_y + 0.2);
  }

  design->setConditionValueAtTR(tr, selected_column, y);
  plotDesignColumn(design, selected_column, edit_plot->graph(selected_column));
  edit_plot->replot();
}

void DesignEditor::finish() {
  design->buildDesignMatrix();
}

QWizardPage* DesignEditor::createMeasPage() {
  QWizardPage *page = new QWizardPage;
  page->setTitle("Setup measurement details");

  QLabel *rep_time_label = new QLabel("Repetition time (ms):");
  QDoubleSpinBox *rep_time = new QDoubleSpinBox;
  rep_time->setRange(0, 10);

  if (design->getTR() > 0) {
    rep_time->setValue(design->getTR());
    setRepTime(design->getTR());
  }
  else {
    rep_time->setValue(2);
    setRepTime(2);
  }

  connect(rep_time, SIGNAL(valueChanged(double)),
          this, SLOT(setRepTime(double)));

  QLabel *num_meas_label = new QLabel("Number of measurements:");
  QSpinBox *num_meas = new QSpinBox;
  num_meas->setRange(1, 10000);

  if (design->getTR() > 0) {
    num_meas->setValue(design->getNumMeas());
    num_meas->setReadOnly(true);
    setNumMeas(design->getNumMeas());
  }
  else {
    num_meas->setValue(100);
    setNumMeas(100);
  }

  connect(num_meas, SIGNAL(valueChanged(int)),
          this, SLOT(setNumMeas(int)));

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

  if (design->getNumInputConditions() == 0) {
    condition_names->addItem("No conditions");
    condition_names->setCurrentIndex(1);
  }
  else {
    for (size_t cond = 0; cond < design->getNumInputConditions(); cond++) {
      selected_column = cond;
      addCondition(QString(design->getConditionName(cond).c_str()), true);
    }
  }

  connect(condition_names, SIGNAL(currentIndexChanged(int)),
          this, SLOT(setSelectedColumn(int)));

  tr_label = new QLabel("TR: ");
  condition_pos_label = new QLabel("Y: ");
  mouse_pos_label = new QLabel("Mouse: ");

  QHBoxLayout *header = new QHBoxLayout;
  header->addWidget(condition_names, 3);
  header->addWidget(tr_label, 1);
  header->addWidget(condition_pos_label, 1);
  header->addWidget(mouse_pos_label, 1);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addLayout(header, 1);
  layout->addWidget(edit_plot, 10);

  page->setLayout(layout);
  return page;
}

void DesignEditor::setRepTime(double rep_time) {
  design->setTR(rep_time);
}

void DesignEditor::setNumMeas(int num_meas) {
  design->setNumMeas(num_meas);
  edit_plot->xAxis->setRange(0, num_meas);
  edit_plot->replot();
}

void DesignEditor::setSelectedColumn(int col) {
  if (col == 0) {
    bool ok;
    QString text = QInputDialog::getText(
      this, "Enter condition name", "Condition name: ",
      QLineEdit::Normal, "", &ok);

    if (ok && !text.isEmpty()) {
      selected_column = design->getNumInputConditions();

      addCondition(text, false);

      if (selected_column == 0) {
        condition_names->removeItem(1);
      }
    }
  }
  else {
    selected_column = col - 1;
  }
}

void DesignEditor::setLabels(int tr, double cond, double mouse) {
  stringstream str;

  str << "TR: " << tr;
  tr_label->setText(QString(str.str().c_str()));

  str.str("");
  str << "Y: " << cond;
  condition_pos_label->setText(QString(str.str().c_str()));

  str.str("");
  str.precision(2);
  str << "Mouse: " << fixed << setw(7) << mouse;
  mouse_pos_label->setText(QString(str.str().c_str()));

}

void DesignEditor::getTrAndYFromMousePos(QMouseEvent* event,
                                         int *tr, double *y) {
  *tr = rint(edit_plot->xAxis->pixelToCoord(event->pos().x()));
  *y = condition_increment *
    round(1 / condition_increment *
          edit_plot->yAxis->pixelToCoord(event->pos().y()));
}
