#pragma once

#include <sstream>

#include <QWizard>

#include "Colormap.h"

class GLMWizardPage;
class MaskWizardPage;
class ROICombineWizardPage;

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

  void handleNextButton();

  void finish();

 private:

  MaskWizardPage *mask_page;
  GLMWizardPage *glm_page;
  ROICombineWizardPage *roi_page;

  RtConfigFmriRun *run_config;

  bool finished;
};
