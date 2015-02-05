#include "GLMWizardPage.h"

#include <iostream>
#include <sstream>

#include <QCheckBox>
#include <QComboBox>
#include <QDoubleSpinBox>
#include <QFormLayout>
#include <QSpinBox>

using std::endl;
using std::string;
using std::stringstream;

GLMWizardPage::GLMWizardPage(QWidget *parent)
  : QWizardPage(parent) {

  QFormLayout *layout = new QFormLayout;

  mask_combo = new QComboBox;
  mask_combo->addItem("No mask (entire volume)");
  mask_combo->addItem("Brain mask (automatically generated)");
  mask_combo->setCurrentIndex(1);

  layout->addRow("ROI mask", mask_combo);

  rep_time_box = new QDoubleSpinBox;
  rep_time_box->setRange(0.1, 20);
  rep_time_box->setValue(2);

  layout->addRow("Repetition time", rep_time_box);

  num_meas_box = new QSpinBox;
  num_meas_box->setRange(1, 1000);
  num_meas_box->setValue(100);

  layout->addRow("Number of measurements", num_meas_box);

  num_trend_box = new QSpinBox;
  num_trend_box->setRange(0, 10);
  num_trend_box->setValue(1);

  layout->addRow("Highest order trend to model", num_trend_box);

  model_motion_box = new QCheckBox;
  model_motion_box->setChecked(false);

  layout->addRow("Model motion parameters", model_motion_box);

  model_temp_deriv_box = new QCheckBox;
  model_temp_deriv_box->setChecked(false);

  layout->addRow("Model temporal derivatives", model_temp_deriv_box);

  num_points_for_err_est = new QSpinBox;
  num_points_for_err_est->setRange(0, 1000);
  num_points_for_err_est->setValue(15);

  layout->addRow("Number of measurements to use in error est",
                 num_points_for_err_est);

  setLayout(layout);
}

void GLMWizardPage::addROIS(const QStringList &rois) {

  // fill the combo box with the ROI names
  for (int roi = 0; roi < rois.size(); roi++) {
    mask_combo->addItem(rois[roi]);
  }
}

string GLMWizardPage::getConfigString() const {
  stringstream config;

  bool use_brain_mask = mask_combo->currentIndex() == 1;
  if (use_brain_mask) {
    config
      << "<module name=\"mask-gen\">"
      << "  <option name=\"makeavail\">true</option>"
      << "  <option name=\"roiID\"> brain </option>"
      << "  <option name=\"threshold\"> 0.5 </option>"
      << "  <option name=\"save\"> true </option>"
      << "</module>";
  }

  config
    << "<module name=\"incremental-glm\">"
    << "  <option name=\"makeavail\">true</option>";

  if (use_brain_mask) {
    config << "  <option name=\"maskRoiID\">brain</option>";
  }
  else if (mask_combo->currentIndex() > 1) {
    config << "  <option name=\"maskRoiID\">"
           << mask_combo->currentText().toStdString()
           << "</option>";
  }

  // TODO add design matrix config

  config << "</module>";

  config << endl;

  return config.str();
}
