#pragma once

#include <map>
#include <string>
#include <vector>

#include "QObject"

#include "RtDataID.h"

#include "MainWindow.h"

class QCPItemLine;
class QCustomPlot;

class PlotController : public QObject {
  Q_OBJECT

 public:

  PlotController(MainWindow *main_window,
                 QCustomPlot *design_plot,
                 QCustomPlot *roi_plot,
                 QCustomPlot *motion_plot);

  ~PlotController();

 public slots:

  void handleData(QString qid);

 private:

  void updateTRIndicators();

  MainWindow *main_window;

  std::map<std::string, int> roi_graphs;

  QCustomPlot *design_plot;
  QCustomPlot *roi_plot;
  QCustomPlot *motion_plot;

  QCPItemLine *design_tr_indicator;
  QCPItemLine *roi_tr_indicator;
  QCPItemLine *motion_tr_indicator;

  unsigned int current_tr;
};
