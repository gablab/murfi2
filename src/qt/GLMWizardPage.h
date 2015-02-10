#pragma once

#include <string>

#include <QWizardPage>

class QCheckBox;
class QComboBox;
class QDoubleSpinBox;
class QSpinBox;
class RtDesignMatrix;

// qwizard page for GLM configuration
class GLMWizardPage : public QWizardPage {
  Q_OBJECT

 public:

  explicit GLMWizardPage(QWidget *);

  virtual ~GLMWizardPage();

  void addMaskNames(const QStringList &masks);

  std::string getConfigString();

 public slots:

  void editDesign();

 private:

  QComboBox *mask_combo;
  QDoubleSpinBox *rep_time_box;
  QSpinBox *num_meas_box;
  QSpinBox *num_trend_box;
  QCheckBox *model_motion_box;
  QCheckBox *model_temp_deriv_box;
  QSpinBox *num_points_for_err_est;

  RtDesignMatrix *design;

};
