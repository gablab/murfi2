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

namespace {

void plotDesign(QCustomPlot *plot, RtDesignMatrix* design) {
  for (size_t graph = 0; graph < design->getNumColumns(); graph++) {
    if (!design->isColumnOfInterest(graph)) {
      continue;
    }

    plot->addGraph();

    vnl_vector<double> col = design->getColumn(graph);
    for (size_t tr = 0; tr < col.size(); tr++) {
      plot->graph(graph)->addData(tr, col[tr]);
    }

  }

  plot->rescaleAxes();
  plot->replot();
}

void plotMotion(QCustomPlot *plot, RtMotion* motion) {
  // TODO
  cout << "plotting motion" << endl;
}

} // anonymous namespace


PlotController::PlotController(MainWindow *main_window,
                               QCustomPlot *design_plot,
                               QCustomPlot *roi_plot,
                               QCustomPlot *motion_plot)
  : main_window(main_window)
  , design_plot(design_plot)
  , roi_plot(roi_plot)
  , motion_plot(motion_plot)
  , design_tr_indicator(new QCPItemLine(design_plot))
  , roi_tr_indicator(new QCPItemLine(roi_plot))
  , motion_tr_indicator(new QCPItemLine(motion_plot))
  , current_tr(0)
{
  design_plot->addItem(design_tr_indicator);
  roi_plot->yAxis->setRange(0, 1);
  roi_plot->addItem(roi_tr_indicator);
  roi_plot->yAxis->setRange(-3, 3);
  motion_plot->addItem(motion_tr_indicator);
  motion_plot->yAxis->setRange(-2, 2);
  updateTRIndicators();
}

PlotController::~PlotController() {
  delete design_tr_indicator;
  delete roi_tr_indicator;
  delete motion_tr_indicator;
}

void PlotController::handleData(QString qid) {
  RtDataID id;
  id.setFromString(qid.toStdString());

  if (id.getDataName() == NAME_DESIGN) {

    RtDesignMatrix *design =
      static_cast<RtDesignMatrix*>(getDataStore().getData(id));
    updateTRIndicators();
    plotDesign(design_plot, design);
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
      roi_plot->graph(it->second)->setPen(QPen(main_window->getColorForName(
                                                 id.getRoiID())));
    }

    RtActivation *val = static_cast<RtActivation*>(getDataStore().getData(id));
    roi_plot->graph(roi_graphs[id.getRoiID()])->addData(id.getTimePoint(),
                                                        val->getPixel(0));
    roi_plot->replot();
  }
  else if (id.getModuleID() == ID_MOTION) {
    // TODO
    //plotMotion(motion_plot, static_cast<RtMotion*>(data));
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
