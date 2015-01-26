#pragma once

#include <QWizard>

class QCustomPlot;
class RtDesignMatrix;

class DesignEditor : public QWizard {
  Q_OBJECT

 public:

  DesignEditor(QWidget *parent, RtDesignMatrix*);
  virtual ~DesignEditor();

 public slots:

  void addCondition(QString name);

 private:
  RtDesignMatrix *design;
  QCustomPlot *edit_plot;
  int selected_column;
};
