#pragma once

#include <QDialog>

#include "Colormap.h"

class QComboBox;
class QCPItemRect;
class QCustomPlot;
class QLabel;
class RtDesignMatrix;

class DesignEditor : public QDialog {
  Q_OBJECT

 public:

  DesignEditor(QWidget *parent, RtDesignMatrix*);
  virtual ~DesignEditor();

  bool hasFinished() const {
    return finished;
  }

 public slots:

  void handleMouseDown(QMouseEvent*);
  void handleMouseMove(QMouseEvent*);
  void handleMouseUp(QMouseEvent*);

  void setSelectedColumn(int);

  void cancel();
  void finish();

 private:
  void addCondition(QString name, bool existing);

  void setLabels(int tr, double cond, double mouse);

  void getTrAndYFromMousePos(QMouseEvent *event, int *tr, double *y);

  RtDesignMatrix *design;
  QCustomPlot *edit_plot;

  Colormap condition_colormap;

  QComboBox *condition_names;
  int selected_column;

  QLabel *tr_label;
  QLabel *mouse_pos_label;
  QLabel *condition_pos_label;
  double condition_increment;

  QCPItemRect *baseline_box;

  double max_y;
  double min_y;

  bool mouse_down;
  bool finished;
};
