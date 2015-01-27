#pragma once

#include <QWizard>

#include "Colormap.h"

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

  void setRepTime(double);

  void setNumMeas(int);

  void setSelectedColumn(int);

 private:
void addCondition(QString name, bool existing);

  QWizardPage *createMeasPage();
  QWizardPage *createEditPage();

  RtDesignMatrix *design;
  QCustomPlot *edit_plot;

  Colormap condition_colormap;

  QComboBox *condition_names;
  int selected_column;

  double max_y;
  double min_y;

};
