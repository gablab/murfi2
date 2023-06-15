#include "PlotController.h"

#include "qcustomplot.h"

#include<vnl_vector.h>

#include "RtActivation.h"
#include "RtData.h"
#include "RtDataIDs.h"
#include "RtDesignMatrix.h"
#include "RtExperiment.h"
#include "RtFramewiseDisplacement.h"

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
  , dragging_legend(false)
{
  baseline_box = new QCPItemRect(design_plot);
  baseline_box->setPen(QPen(QColor(178, 178, 178)));
  baseline_box->setBrush(QBrush(QColor(178, 178, 178)));
  baseline_box->topLeft->setCoords(0, 0);
  baseline_box->bottomRight->setCoords(0, 0);

  design_plot->addItem(baseline_box);
  design_plot->addItem(design_tr_indicator);

  design_plot->xAxis->setRange(0, 100);
  design_plot->xAxis->setLabel("TR");
  design_plot->yAxis->setRange(0, 1);
  design_plot->yAxis->setLabel("normalized");

  roi_plot->xAxis->setRange(0, 100);
  roi_plot->xAxis->setLabel("TR");
  roi_plot->yAxis->setRange(0, 1);
  roi_plot->yAxis->setLabel("standard dev");

  motion_plot->xAxis->setRange(0, 100);
  motion_plot->xAxis->setLabel("TR");
  motion_plot->yAxis->setRange(0, 1);
  motion_plot->yAxis->setLabel("FD");

  roi_plot->yAxis->setRange(0, 1);
  roi_plot->addItem(roi_tr_indicator);
  roi_plot->yAxis->setRange(-3, 3);

  roi_plot->axisRect()->insetLayout()->setInsetPlacement(
    0, QCPLayoutInset::ipFree);

  motion_plot->addItem(motion_tr_indicator);
  motion_plot->addGraph();
  motion_plot->graph(0)->setPen(QPen(motion_colormap.getColor(0)));

  updateTRIndicators();

  connect(roi_plot, SIGNAL(mouseMove(QMouseEvent*)),
          this, SLOT(mouseMoveSignal(QMouseEvent*)));
  connect(roi_plot, SIGNAL(mousePress(QMouseEvent*)),
          this, SLOT(mousePressSignal(QMouseEvent*)));
  connect(roi_plot, SIGNAL(mouseRelease(QMouseEvent*)),
          this, SLOT(mouseReleaseSignal(QMouseEvent*)));
  connect(roi_plot, SIGNAL(beforeReplot()),
          this, SLOT(beforeReplot()));
}

PlotController::~PlotController() {
}

const QColor& PlotController::getColorForName(const string &name) {
  return roi_colormap.getColorForName(name);
}

void PlotController::handleData(QString qid) {
  RtDataID id;
  id.setFromString(qid.toStdString());

  if (id.getDataName() == NAME_DESIGN) {
    baseline_box->topLeft->setCoords(0, 10);
    baseline_box->bottomRight->setCoords(getNumDataPointsForErrEst(), -10);

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
      roi_plot->legend->setVisible(true);
    }

    RtActivation *val = static_cast<RtActivation*>(getDataStore().getData(id));
    roi_plot->graph(roi_graphs[id.getRoiID()])->addData(id.getTimePoint(),
                                                        val->getPixel(0));
    roi_plot->replot();
  }
  else if (id.getModuleID() == ID_FRAMEWISE_DISPLACEMENT) {
    plotMotion(static_cast<RtFramewiseDisplacement*>(getDataStore().getData(id)));
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

  size_t graph = 0;
  for (size_t col_ind = 0; col_ind < design->getNumColumns(); col_ind++) {
    if (!design->isColumnOfInterest(col_ind)) {
      continue;
    }

    design_plot->addGraph();

    vnl_vector<double> col = design->getColumn(col_ind);
    for (size_t tr = 0; tr < col.size(); tr++) {
      design_plot->graph(graph)->addData(tr, col[tr]);
      design_plot->graph(graph)->setPen(
        QPen(design_colormap.getColor(graph)));
    }
    graph++;
  }

  design_plot->rescaleAxes();
  design_plot->replot();
}

void PlotController::plotMotion(RtFramewiseDisplacement* fd) {
  double displacement = fd->getDisplacement();
  motion_plot->graph(0)->addData(fd->getDataID().getTimePoint(), displacement);

  if (!motion_plot->yAxis->range().contains(displacement)) {
    motion_plot->yAxis->setRange(0, displacement + 1);
  }
}

void PlotController::replotDesign(RtDesignMatrix *design) {
  design_plot->clearPlottables();
  plotDesign(design);
}

void PlotController::mouseMoveSignal(QMouseEvent *event) {
  if (dragging_legend) {
    QRectF rect = roi_plot->axisRect()->insetLayout()->insetRect(0);

    QPointF mousePoint((event->pos().x() - roi_plot->axisRect()->left()) /
                       (double) roi_plot->axisRect()->width(),
                       (event->pos().y() - roi_plot->axisRect()->top()) /
                       (double) roi_plot->axisRect()->height());
    rect.moveTopLeft(mousePoint - drag_legend_origin);
    roi_plot->axisRect()->insetLayout()->setInsetRect(0, rect);
    roi_plot->replot();
  }
}

void PlotController::mousePressSignal(QMouseEvent *event) {
  if (roi_plot->legend->selectTest(event->pos(), false) > 0) {
    cout << event->pos().x() << " " << event->pos().y() << endl;

    dragging_legend = true;

    QPointF mousePoint((event->pos().x() - roi_plot->axisRect()->left()) /
                       (double) roi_plot->axisRect()->width(),
                       (event->pos().y() - roi_plot->axisRect()->top()) /
                       (double) roi_plot->axisRect()->height());
    drag_legend_origin =
      mousePoint - roi_plot->axisRect()->insetLayout()->insetRect(0).topLeft();
  }
}

void PlotController::mouseReleaseSignal(QMouseEvent *event) {
  Q_UNUSED(event)
  dragging_legend = false;
}

void PlotController::beforeReplot() {
  roi_plot->legend->setMaximumSize(roi_plot->legend->minimumSizeHint());
}
