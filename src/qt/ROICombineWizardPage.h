#pragma once

#include <string>

#include <QStringList>
#include <QWizardPage>

class QTableWidget;

// qwizard page for GLM configuration
class ROICombineWizardPage : public QWizardPage {
  Q_OBJECT

 public:

  explicit ROICombineWizardPage(QWidget *);

  virtual ~ROICombineWizardPage();

  void setMaskNames(const QStringList &names);

  std::string getConfigString();

 public slots:

  void insertROICombineRow();
  void removeROICombineRow();

 private:

  QTableWidget *roi_combine_table;
  QStringList mask_names;
};
