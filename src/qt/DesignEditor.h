#pragma once

#include <QWizard>

class QComboBox;
class QCustomPlot;
class RtDesignMatrix;

class DesignEditor : public QWizard {
  Q_OBJECT

 public:

  DesignEditor(QWidget *parent, RtDesignMatrix*);
  virtual ~DesignEditor();

 public slots:

  void handleConditionClick(QMouseEvent*);

  void setRepTime(int);

  void setNumMeas(int);

  void setSelectedColumn(int);

 private:
  void addCondition(QString name);

  QWizardPage *createMeasPage();
  QWizardPage *createEditPage();

  RtDesignMatrix *design;
  QCustomPlot *edit_plot;

  QComboBox *condition_names;
  int selected_column;
};
