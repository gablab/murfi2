#pragma once

#include <string>

#include <QWizardPage>

class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QSpinBox;

// qwizard page for GLM configuration
class GLMWizardPage : public QWizardPage {
  Q_OBJECT

 public:

  explicit GLMWizardPage(QWidget *);

  void addROIS(const QStringList &rois);

  std::string getConfigString() const;

 private:

  QComboBox *mask_combo;
  QDoubleSpinBox *rep_time_box;
  QSpinBox *num_meas_box;
  QSpinBox *num_trend_box;
  QCheckBox *model_motion_box;
  QCheckBox *model_temp_deriv_box;
  QSpinBox *num_points_for_err_est;

};
