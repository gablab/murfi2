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
  void newExperiment();
  void newRun();
  void openRun();
  // bool saveRun();
  // bool saveRunAs();
  // void runAgain();
  void editDesign();

 private:

  Ui::MainWindow *ui;
  PlotController *plot_controller;
  DesignEditor *design_editor;
};