#include "UnivariateRunEditor.h"

#include <string>

#include <QCheckBox>
#include <QFileDialog>
#include <QHBoxLayout>
#include <QPushButton>
#include <QString>
#include <QStringList>
#include <QTableWidget>
#include <QVBoxLayout>
#include <QWizardPage>

#include "RtConfigFmriRun.h"

#include "FileButton.h"

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
  addPage(createGLMPage());
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
  int page = currentPage()->nextId() - 1;

  if (page == 1) { // mask setup
    mask_config.str("");

    if (brain_mask_checkbox->isChecked()) {
      mask_config
        << "<module name=\"mask-gen\">"
        << "  <option name=\"makeavail\">true</option>"
        << "  <option name=\"roiID\"> brain </option>"
        << "  <option name=\"threshold\"> 0.5 </option>"
        << "  <option name=\"save\"> true </option>"
        << "</module>";
    }

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

}

void UnivariateRunEditor::finish() {
  finished = true;
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

  brain_mask_checkbox = new QCheckBox(
    "Only compute GLM within brain mask", this);
  brain_mask_checkbox->setChecked(true);

  QVBoxLayout *layout = new QVBoxLayout;
  layout->addLayout(addRemoveLayout, 1);
  layout->addWidget(mask_table, 9);
  layout->addWidget(brain_mask_checkbox, 1);

  page->setLayout(layout);

  connect(addMask, SIGNAL(clicked()), this, SLOT(insertMaskRow()));
  connect(removeMask, SIGNAL(clicked()), this, SLOT(removeMaskRow()));

  return page;
}

QWizardPage* UnivariateRunEditor::createGLMPage() {
  QWizardPage *page = new QWizardPage;
  page->setTitle("GLM setup");

  return page;
}

QWizardPage* UnivariateRunEditor::createROICombinePage() {
  QWizardPage *page = new QWizardPage;
  page->setTitle("ROI combination setup");

  return page;
}
