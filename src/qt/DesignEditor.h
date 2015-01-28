#pragma once

#include <QWizard>

#include "Colormap.h"

class QComboBox;
class QCustomPlot;
class QLabel;
class RtDesignMatrix;

class DesignEditor : public QWizard {
  Q_OBJECT

 public:

  DesignEditor(QWidget *parent, RtDesignMatrix*);
  virtual ~DesignEditor();

 public slots:

  void handleMouseDown(QMouseEvent*);
  void handleMouseMove(QMouseEvent*);
  void handleMouseUp(QMouseEvent*);

  void setRepTime(double);

  void setNumMeas(int);

  void setSelectedColumn(int);

  void finish();

 private:
void addCondition(QString name, bool existing);

  QWizardPage *createMeasPage();
  QWizardPage *createEditPage();

  void setLabels(int tr, double cond, double mouse);

  RtDesignMatrix *design;
  QCustomPlot *edit_plot;

  Colormap condition_colormap;

  QComboBox *condition_names;
  int selected_column;

  QLabel *tr_label;
  QLabel *mouse_pos_label;
  QLabel *condition_pos_label;

  double max_y;
  double min_y;

  bool mouse_down;

};
