#pragma once

#include <QColor>
#include <QMainWindow>
#include <QString>

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

  QColor getColor(int index);

  QColor getColorForName(const std::string &name);

 signals:
  void dataReady(QString);

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

  map<std::string, QColor> name_colors;
};
