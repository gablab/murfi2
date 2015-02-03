#pragma once

#include <sstream>

#include <QWizard>

#include "Colormap.h"

class QCheckBox;
class QTableWidget;
class QWizardPage;

class RtConfigFmriRun;

class UnivariateRunEditor : public QWizard {
  Q_OBJECT

 public:

  UnivariateRunEditor(QWidget *parent, RtConfigFmriRun *run_config);
  virtual ~UnivariateRunEditor();

  bool hasFinished() const {
    return finished;
  }

 public slots:

  void insertMaskRow();
  void removeMaskRow();

  void handleNextButton();

  void finish();

 private:

  QWizardPage *createMaskPage();
  QWizardPage *createGLMPage();
  QWizardPage *createROICombinePage();

  RtConfigFmriRun *run_config;

  std::stringstream mask_config;
  std::stringstream glm_config;
  std::stringstream roi_config;

  QTableWidget *mask_table;
  QCheckBox *brain_mask_checkbox;

  bool finished;
};
