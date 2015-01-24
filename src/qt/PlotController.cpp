#include "PlotController.h"

#include "qcustomplot.h"

#include<vnl/vnl_vector.h>

#include "RtActivation.h"
#include "RtData.h"
#include "RtDataIDs.h"
#include "RtDesignMatrix.h"
#include "RtExperiment.h"
#include "RtMotion.h"

using std::pair;
using std::string;
using std::vector;

PlotController::PlotController(QCustomPlot *design_plot,
                               QCustomPlot *roi_plot,
                               QCustomPlot *motion_plot)
  : design_plot(design_plot)
  , design_colormap(Colormap::ColormapType::DESIGN)
  , roi_plot(roi_plot)
  , roi_colormap(Colormap::ColormapType::ROI)
  , motion_plot(motion_plot)
  , motion_colormap(Colormap::ColormapType::MOTION)
  , design_tr_indicator(new QCPItemLine(design_plot))
  , roi_tr_indicator(new QCPItemLine(roi_plot))
  , motion_tr_indicator(new QCPItemLine(motion_plot))
  , current_tr(0)
{
  design_plot->addItem(design_tr_indicator);

  roi_plot->yAxis->setRange(0, 1);
  roi_plot->addItem(roi_tr_indicator);
  roi_plot->yAxis->setRange(-3, 3);
  roi_plot->legend->setVisible(true);

  motion_plot->addItem(motion_tr_indicator);
  motion_plot->yAxis->setRange(-2, 2);
  for (int i = 0; i < 6; i++) {
    motion_plot->addGraph();
    motion_plot->graph(i)->setPen(QPen(motion_colormap.getColor(i)));
  }

  updateTRIndicators();
}

PlotController::~PlotController() {
  delete design_tr_indicator;
  delete roi_tr_indicator;
  delete motion_tr_indicator;
}

const QColor& PlotController::getColorForName(const string &name) {
  return roi_colormap.getColorForName(name);
}

void PlotController::handleData(QString qid) {
  RtDataID id;
  id.setFromString(qid.toStdString());

  if (id.getDataName() == NAME_DESIGN) {
    RtDesignMatrix *design =
      static_cast<RtDesignMatrix*>(getDataStore().getData(id));
    updateTRIndicators();
    plotDesign(design);
    roi_plot->xAxis->setRange(0, design->getNumRows());
    roi_plot->replot();
    motion_plot->xAxis->setRange(0, design->getNumRows());
    motion_plot->replot();
  }
  else if (id.getModuleID() == ID_ROICOMBINE) {
    map<string, int>::const_iterator it = roi_graphs.find(id.getRoiID());
    if (it == roi_graphs.end()) {
      it = roi_graphs.insert(
        pair<string, int>(id.getRoiID(), roi_plot->graphCount())).first;
      roi_plot->addGraph();
      roi_plot->graph(it->second)->setPen(QPen(roi_colormap.getColorForName(
                                                 id.getRoiID())));
      roi_plot->graph(it->second)->setName(QString(id.getRoiID().c_str()));
    }

    RtActivation *val = static_cast<RtActivation*>(getDataStore().getData(id));
    roi_plot->graph(roi_graphs[id.getRoiID()])->addData(id.getTimePoint(),
                                                        val->getPixel(0));
    roi_plot->replot();
  }
  else if (id.getModuleID() == ID_MOTION) {
    plotMotion(static_cast<RtMotion*>(getDataStore().getData(id)));
  }

  if (id.getTimePoint() != DATAID_NUM_UNSET_VALUE &&
      id.getTimePoint() > current_tr) {
    current_tr = id.getTimePoint();
    updateTRIndicators();
  }
}

void PlotController::updateTRIndicators() {
  // TODO get ranges automatically
  design_tr_indicator->start->setCoords(current_tr, 1000);
  design_tr_indicator->end->setCoords(current_tr, -1000);
  design_plot->replot();

  roi_tr_indicator->start->setCoords(current_tr, 1000);
  roi_tr_indicator->end->setCoords(current_tr, -1000);
  roi_plot->replot();

  motion_tr_indicator->start->setCoords(current_tr, 1000);
  motion_tr_indicator->end->setCoords(current_tr, -1000);
  motion_plot->replot();
}

void PlotController::plotDesign(RtDesignMatrix* design) {
  for (size_t graph = 0; graph < design->getNumColumns(); graph++) {
    if (!design->isColumnOfInterest(graph)) {
      continue;
    }

    design_plot->addGraph();

    vnl_vector<double> col = design->getColumn(graph);
    for (size_t tr = 0; tr < col.size(); tr++) {
      design_plot->graph(graph)->addData(tr, col[tr]);
      design_plot->graph(graph)->setPen(
        QPen(design_colormap.getColor(graph)));
    }

  }

  design_plot->rescaleAxes();
  design_plot->replot();
}

void PlotController::plotMotion(RtMotion* motion) {
  motion_plot->graph(TRANSLATION_X)->addData(
    motion->getDataID().getTimePoint(),
    motion->getMotionDimension(TRANSLATION_X));

  motion_plot->graph(TRANSLATION_Y)->addData(
    motion->getDataID().getTimePoint(),
    motion->getMotionDimension(TRANSLATION_Y));

  motion_plot->graph(TRANSLATION_Z)->addData(
    motion->getDataID().getTimePoint(),
    motion->getMotionDimension(TRANSLATION_Z));

  motion_plot->graph(ROTATION_X)->addData(
    motion->getDataID().getTimePoint(),
    motion->getMotionDimension(ROTATION_X));

  motion_plot->graph(ROTATION_Y)->addData(
    motion->getDataID().getTimePoint(),
    motion->getMotionDimension(ROTATION_Y));

  motion_plot->graph(ROTATION_Z)->addData(
    motion->getDataID().getTimePoint(),
    motion->getMotionDimension(ROTATION_Z));
}
