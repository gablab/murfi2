#include "UnivariateRunEditor.h"

#include <string>

#include <QPushButton>

#include "RtConfigFmriRun.h"

#include "GLMWizardPage.h"
#include "MaskWizardPage.h"
#include "ROICombineWizardPage.h"

using std::string;

UnivariateRunEditor::UnivariateRunEditor(QWidget *parent,
                                         RtConfigFmriRun *run_config)
  : QWizard(parent)
  , run_config(run_config)
  , finished(false) {

  mask_page = new MaskWizardPage(this);
  addPage(mask_page);

  glm_page = new GLMWizardPage(this);
  addPage(glm_page);

  roi_page = new ROICombineWizardPage(this);
  addPage(roi_page);

  connect(this->button(QWizard::NextButton), SIGNAL(clicked()),
          this, SLOT(handleNextButton()));

  connect(this->button(QWizard::FinishButton), SIGNAL(clicked()),
          this, SLOT(finish()));
}

UnivariateRunEditor::~UnivariateRunEditor() {}

void UnivariateRunEditor::handleNextButton() {
  int page = currentPage()->nextId();

  // tell the other wizard pages about the mask names
  if (page == 2) {
    QStringList masks = mask_page->getMaskNames();
    glm_page->addMaskNames(masks);
    roi_page->setMaskNames(masks);
  }
}

void UnivariateRunEditor::finish() {

  stringstream config;
  config
    << "<processor> "
    << "<module name=\"mosaic\">"
    << "  <output>display</output>"
    << "  <output>log</output>"
    << "</module>"
    << mask_page->getConfigString()
    << glm_page->getConfigString()
    << roi_page->getConfigString()
    << "</processor>";

  if (run_config->parseConfigStr(config.str())) {
    finished = true;
  }
}
