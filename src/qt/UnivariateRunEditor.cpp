#include "UnivariateRunEditor.h"

#include <string>

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFileDialog>
#include <QFormLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QSpinBox>
#include <QString>
#include <QStringList>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWizardPage>

#include "RtConfigFmriRun.h"

#include "FileButton.h"
#include "GLMWizardPage.h"

using std::string;

namespace {
enum MASK_COLS {
  NAME_COL = 0,
  FILE_COL,
  ALIGN_COL,
  NUM_MASK_COLS
};

} // anonymous namespace

UnivariateRunEditor::UnivariateRunEditor(QWidget *parent,
                                         RtConfigFmriRun *run_config)
  : QWizard(parent)
  , run_config(run_config)
  , finished(false) {

  addPage(createMaskPage());
  glm_page = new GLMWizardPage(this);
  addPage(glm_page);
  addPage(createROICombinePage());

  connect(this->button(QWizard::NextButton), SIGNAL(clicked()),
          this, SLOT(handleNextButton()));

  connect(this->button(QWizard::FinishButton), SIGNAL(clicked()),
          this, SLOT(finish()));
}

UnivariateRunEditor::~UnivariateRunEditor() {}

void UnivariateRunEditor::insertMaskRow() {
  mask_table->insertRow(mask_table->rowCount());

  FileButton *file_button = new FileButton(mask_table);
  mask_table->setCellWidget(mask_table->rowCount() - 1, FILE_COL, file_button);

  QCheckBox *align_box = new QCheckBox(mask_table);
  align_box->setChecked(true);
  mask_table->setCellWidget(mask_table->rowCount() - 1, ALIGN_COL, align_box);
}

void UnivariateRunEditor::removeMaskRow() {
  mask_table->removeRow(mask_table->currentRow());
}

void UnivariateRunEditor::handleNextButton() {
  int page = currentPage()->nextId();

  // tell the glm wizard about the roi names
  if (page == 2) {
    QStringList rois;
    for (int mask = 0; mask < mask_table->rowCount(); mask++) {
      rois << mask_table->item(mask, NAME_COL)->text();
    }
    glm_page->addROIS(rois);
  }
}

void UnivariateRunEditor::finish() {
  makeMaskConfig();
  makeROIConfig();
  mask_config << glm_page->getConfigString() << roi_config.str();

  stringstream config;
  config
    << "<processor> "
    << "<module name=\"mosaic\">"
    << "  <output>display</output>"
    << "  <output>log</output>"
    << "</module>"
    << mask_config.str()
    << "</processor>";

  if (run_config->parseConfigStr(config.str())) {
    finished = true;
  }
}

QWizardPage* UnivariateRunEditor::createMaskPage() {
  QWizardPage *page = new QWizardPage;
  page->setTitle("Mask setup");

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

  page->setLayout(layout);

  connect(addMask, SIGNAL(clicked()), this, SLOT(insertMaskRow()));
  connect(removeMask, SIGNAL(clicked()), this, SLOT(removeMaskRow()));

  return page;
}

QWizardPage* UnivariateRunEditor::createROICombinePage() {
  QWizardPage *page = new QWizardPage;
  page->setTitle("ROI combination setup");

  return page;
}

void UnivariateRunEditor::makeMaskConfig() {
  mask_config.str("");

  for (int mask = 0; mask < mask_table->rowCount(); mask++) {
    mask_config
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
}

void UnivariateRunEditor::makeROIConfig() {
  roi_config.str("");

  roi_config
    << "  <module name=\"roi-combine\">"
    << "    <option name=\"makeavail\">true</option>"
    << "    <output>infoserver</output>"
    << "    <output>infoclient</output>"
    << "    <option name=\"method\"> weighted-ave </option>"
    << "    <option name=\"maskRoiID\"> active </option>"
    << "    <option name=\"inputModuleID\"> current-activation </option>"
    << "    <option name=\"inputDataName\"> activation-img </option>"
    << "    <option name=\"inputRoiID\"> brain </option>"
    << "    <option name=\"weightModuleID\"> incremental-glm </option>"
    << "    <option name=\"weightDataName\"> residual_mse-img </option>"
    << "  </module>";
}
