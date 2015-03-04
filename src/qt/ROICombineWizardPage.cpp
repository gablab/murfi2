#include "ROICombineWizardPage.h"

#include <iostream>
#include <sstream>

#include <QComboBox>
#include <QHBoxLayout>
#include <QPushButton>
#include <QTableWidget>
#include <QVBoxLayout>

namespace {
enum TABLE_COL {
  MASK_COL = 0,
  METHOD_COL,
  NUM_TABLE_COLS
};

enum COMBINATION_METHOD {
  MEAN = 0,
  VARIANCE,
  MEDIAN,
  WEIGHTED_AVE,
  WEIGHTED_RAW,
  SUM,
  MIN,
  MAX,
  NUM_METHODS
};

QComboBox *makeCombinationCombo() {
  QComboBox *box = new QComboBox();

  for (int i = 0; i < NUM_METHODS; i++) {
    switch (i) {
      case MEAN:
        box->addItem("Mean");
        break;
      case MEDIAN:
        box->addItem("Median");
        break;
      case WEIGHTED_AVE:
        box->addItem("Weighted average");
        break;
      case WEIGHTED_RAW:
        box->addItem("Weighted raw");
        break;
    }
  }

  return box;
}

} // anonymous namespace

using std::endl;
using std::string;
using std::stringstream;

ROICombineWizardPage::ROICombineWizardPage(QWidget *parent)
  : QWizardPage(parent) {

  setTitle("ROI combination setup");

  QHBoxLayout *addRemoveLayout = new QHBoxLayout;
  QPushButton *addCombine = new QPushButton("&Add ROI combination file", this);
  QPushButton *removeCombine = new QPushButton("&Remove selected combination",
                                            this);
  addRemoveLayout->addWidget(addCombine);
  addRemoveLayout->addWidget(removeCombine);

  roi_combine_table = new QTableWidget(0, NUM_TABLE_COLS, this);

  QStringList header;
  header << "Input mask" << "Combination method";
  roi_combine_table->setHorizontalHeaderLabels(header);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addLayout(addRemoveLayout, 1);
  layout->addWidget(roi_combine_table, 9);

  setLayout(layout);

  connect(addCombine, SIGNAL(clicked()), this, SLOT(insertROICombineRow()));
  connect(removeCombine, SIGNAL(clicked()), this, SLOT(removeROICombineRow()));
}

ROICombineWizardPage::~ROICombineWizardPage() {
}

void ROICombineWizardPage::setMaskNames(const QStringList &masks) {
  mask_names = masks;
}

string ROICombineWizardPage::getConfigString() {
  stringstream config;

  for (int combine = 0; combine < roi_combine_table->rowCount(); combine++) {
    QComboBox *mask_box = static_cast<QComboBox*>(
      roi_combine_table->cellWidget(combine, MASK_COL));
    QComboBox *method_box = static_cast<QComboBox*>(
      roi_combine_table->cellWidget(combine, METHOD_COL));

    config
      << "<module name=\"roi-combine\">"
      << "  <option name=\"makeavail\">true</option>"
      << "  <option name=\"print\">true</option>"
      << "  <output>infoserver</output>"
      << "  <output>infoclient</output>"
      << "  <option name=\"maskRoiID\"> "
      << mask_box->currentText().toStdString()
      << " </option>"
      << "<option name=\"inputModuleID\"> current-activation </option>"
      << "<option name=\"inputDataName\"> activation-img </option>"
      << "<option name=\"inputRoiID\"> brain </option>";

    switch (method_box->currentIndex()) {
      case MEAN:
        config << "<option name=\"method\"> mean </option>";
        break;
      case VARIANCE:
        config << "<option name=\"method\"> variance </option>";
        break;
      case MEDIAN:
        config << "<option name=\"method\"> median </option>";
        break;
      case WEIGHTED_AVE:
        config << "<option name=\"method\"> weighted-ave </option>"
               << "<option name=\"weightModuleID\"> incremental-glm </option>"
               << "<option name=\"weightDataName\"> residual_mse-img </option>";
        break;
      case WEIGHTED_RAW:
        config << "<option name=\"method\"> weighted-raw </option>";
        break;
      case SUM:
        config << "<option name=\"method\"> sum </option>";
        break;
      case MIN:
        config << "<option name=\"method\"> min </option>";
        break;
      case MAX:
        config << "<option name=\"method\"> sax </option>";
        break;
    }


    config << "</module>";
  }

  config << endl;

  return config.str();
}

void ROICombineWizardPage::insertROICombineRow() {
  roi_combine_table->insertRow(roi_combine_table->rowCount());

  QComboBox *mask_combo = new QComboBox;
  mask_combo->addItems(mask_names);
  roi_combine_table->setCellWidget(roi_combine_table->rowCount() - 1,
                                   MASK_COL, mask_combo);

  roi_combine_table->setCellWidget(roi_combine_table->rowCount() - 1,
                                   METHOD_COL, makeCombinationCombo());
}

void ROICombineWizardPage::removeROICombineRow() {
  roi_combine_table->removeRow(roi_combine_table->currentRow());
}
