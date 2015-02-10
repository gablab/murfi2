#pragma once

#include <string>

#include <QStringList>
#include <QWizardPage>

class QTableWidget;

// qwizard page for GLM configuration
class MaskWizardPage : public QWizardPage {
  Q_OBJECT

 public:

  explicit MaskWizardPage(QWidget *);

  virtual ~MaskWizardPage();

  QStringList getMaskNames() const;

  std::string getConfigString();

 public slots:

  void insertMaskRow();
  void removeMaskRow();

 private:

  QTableWidget *mask_table;
};
