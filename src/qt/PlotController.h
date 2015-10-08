#pragma once

#include <map>
#include <string>
#include <vector>

#include "QColor"
#include "QMouseEvent"
#include "QObject"

#include "RtDataID.h"

#include "Colormap.h"

class QCPItemLine;
class QCPItemRect;
class QCustomPlot;
class RtDesignMatrix;
class RtMotion;

class PlotController : public QObject {
  Q_OBJECT

 public:

  PlotController(QCustomPlot *design_plot,
                 QCustomPlot *roi_plot,
                 QCustomPlot *motion_plot);

  ~PlotController();

  const QColor& getColorForName(const std::string &name);

  void replotDesign(RtDesignMatrix *design);

 public slots:

  void handleData(QString qid);

 private slots:

  void mouseMoveSignal(QMouseEvent *event);
  void mousePressSignal(QMouseEvent *event);
  void mouseReleaseSignal(QMouseEvent *event);
  void beforeReplot();

 private:

  void updateTRIndicators();

  void plotDesign(RtDesignMatrix* design);

  void plotMotion(RtMotion* motion);

  std::map<std::string, int> roi_graphs;

  QCustomPlot *design_plot;
  Colormap design_colormap;

  QCustomPlot *roi_plot;
  Colormap roi_colormap;

  QCustomPlot *motion_plot;
  Colormap motion_colormap;

  QCPItemLine *design_tr_indicator;
  QCPItemRect *baseline_box;
  QCPItemLine *roi_tr_indicator;
  QCPItemLine *motion_tr_indicator;

  unsigned int current_tr;

  bool dragging_legend;
  QPointF drag_legend_origin;

};
