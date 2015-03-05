#pragma once

#include <QColor>
#include <QMainWindow>
#include <QString>

#include "RtDataListener.h"

namespace Ui {
class MainWindow;
}

class DesignEditor;
class PlotController;

class MainWindow : public QMainWindow, public RtDataListener {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void notify(const RtDataID&);
  const QColor& getColorForName(const std::string &name);

 signals:
  void dataReady(QString);

 private slots:
  void newUnivariateRun();
  void openRun();
  void editDesign();

 private:

  Ui::MainWindow *ui;
  PlotController *plot_controller;
  DesignEditor *design_editor;
};
