#include "MaskWizardPage.h"

#include <iostream>
#include <sstream>

#include <QCheckBox>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QVBoxLayout>

#include "FileButton.h"

namespace {
enum MASK_COLS {
  NAME_COL = 0,
  FILE_COL,
  ALIGN_COL,
  NUM_MASK_COLS
};

} // anonymous namespace

using std::endl;
using std::string;
using std::stringstream;

MaskWizardPage::MaskWizardPage(QWidget *parent)
  : QWizardPage(parent) {

  setTitle("Mask setup");

  QHBoxLayout *addRemoveLayout = new QHBoxLayout;
  QPushButton *addMask = new QPushButton("&Add mask file", this);
  QPushButton *removeMask = new QPushButton("&Remove selected mask", this);
  addRemoveLayout->addWidget(addMask);
  addRemoveLayout->addWidget(removeMask);

  mask_table = new QTableWidget(0, NUM_MASK_COLS, this);

  QStringList header;
  header << "ROI name" << "File" << "Align";
  mask_table->setHorizontalHeaderLabels(header);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addLayout(addRemoveLayout, 1);
  layout->addWidget(mask_table, 9);

  setLayout(layout);

  connect(addMask, SIGNAL(clicked()), this, SLOT(insertMaskRow()));
  connect(removeMask, SIGNAL(clicked()), this, SLOT(removeMaskRow()));
}

MaskWizardPage::~MaskWizardPage() {
}

QStringList MaskWizardPage::getMaskNames() const {
  QStringList rois;
  for (int mask = 0; mask < mask_table->rowCount(); mask++) {
    rois << mask_table->item(mask, NAME_COL)->text();
  }
  return rois;
}

string MaskWizardPage::getConfigString() {
  stringstream config;

  for (int mask = 0; mask < mask_table->rowCount(); mask++) {
    config
      << "<module name=\"mask-load\">"
      << "  <option name=\"makeavail\">true</option>"
      << "  <option name=\"roiID\"> "
      << mask_table->item(mask, NAME_COL)->text().toStdString()
      << " </option>"
      << "  <option name=\"filename\"> "
      << static_cast<FileButton*>(
        mask_table->cellWidget(mask, FILE_COL))->getPath()
      << " </option>"
      << "  <option name=\"align\"> "
      << (static_cast<QCheckBox*>(
            mask_table->cellWidget(
              mask, ALIGN_COL))->isChecked() ? "true" : "false")
      << " </option>"
      << "  <option name=\"save\"> true </option>"
      << "</module>";
  }

  config << endl;

  return config.str();
}

void MaskWizardPage::insertMaskRow() {
  mask_table->insertRow(mask_table->rowCount());

  FileButton *file_button = new FileButton(mask_table);
  mask_table->setCellWidget(mask_table->rowCount() - 1, FILE_COL, file_button);

  QCheckBox *align_box = new QCheckBox(mask_table);
  align_box->setChecked(true);
  mask_table->setCellWidget(mask_table->rowCount() - 1, ALIGN_COL, align_box);
}

void MaskWizardPage::removeMaskRow() {
  mask_table->removeRow(mask_table->currentRow());
}
