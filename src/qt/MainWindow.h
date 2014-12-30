#pragma once

#include <QMainWindow>

#include "RtDataListener.h"

namespace Ui {
class MainWindow;
}

class PlotController;

class MainWindow : public QMainWindow, public RtDataListener {
  Q_OBJECT

 public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();

  void notify(const RtDataID&);

 private slots:
  void newExperiment();
  void newRun();
  void openRun();
  // bool saveRun();
  // bool saveRunAs();
  // void runAgain();

 private:

  Ui::MainWindow *ui;
  PlotController *plot_controller;
};
