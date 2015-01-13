#pragma once

#include <string>
#include <vector>

#include "QObject"

#include "RtDataID.h"

class QCPItemLine;
class QCustomPlot;

class PlotController : public QObject {
  Q_OBJECT

 public:

  PlotController(QCustomPlot *design_plot,
                 QCustomPlot *roi_plot,
                 QCustomPlot *motion_plot);

  ~PlotController();

  // add an roi to the list of names to listen for. does not do
  // redundancy checking.
  void addRoi(const std::string &name);

 public slots:

  void handleData(QString qid);

 private:

  void updateTRIndicators();

  std::vector<std::string> roi_names;

  QCustomPlot *design_plot;
  QCustomPlot *roi_plot;
  QCustomPlot *motion_plot;

  QCPItemLine *design_tr_indicator;
  QCPItemLine *roi_tr_indicator;
  QCPItemLine *motion_tr_indicator;

  unsigned int current_tr;
};
