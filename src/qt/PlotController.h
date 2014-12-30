#pragma once

#include <string>
#include <vector>

class QCustomPlot;
class RtData;

class PlotController {
 public:

  PlotController(QCustomPlot *design_plot,
                 QCustomPlot *roi_plot,
                 QCustomPlot *motion_plot);

  ~PlotController();

  // add an roi to the list of names to listen for. does not do
  // redundancy checking.
  void addRoi(const std::string &name);

  void handleData(RtData *data);

 private:

  std::vector<std::string> roi_names;

  QCustomPlot *design_plot;
  QCustomPlot *roi_plot;
  QCustomPlot *motion_plot;

};
