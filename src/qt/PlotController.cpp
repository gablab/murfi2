#include "PlotController.h"

#include "qcustomplot.h"

#include "RtData.h"
#include "RtDataIDs.h"
#include "RtDesignMatrix.h"
#include "RtMotion.h"

using std::string;
using std::vector;

namespace {

void plotDesign(QCustomPlot *plot, RtDesignMatrix* design) {
  // TODO
  cout << "plotting design" << endl;
}

void plotMotion(QCustomPlot *plot, RtMotion* motion) {
  // TODO
  cout << "plotting motion" << endl;
}

} // anonymous namespace


PlotController::PlotController(QCustomPlot *design_plot,
                               QCustomPlot *roi_plot,
                               QCustomPlot *motion_plot)
  : design_plot(design_plot)
  , roi_plot(roi_plot)
  , motion_plot(motion_plot)
{}

PlotController::~PlotController() {}

void PlotController::addRoi(const string &name) {
  roi_names.push_back(name);
}

void PlotController::handleData(RtData *data) {
  RtDataID id = data->getDataID();
  if (id.getDataName() == NAME_DESIGN) {
    plotDesign(design_plot, static_cast<RtDesignMatrix*>(data));
  }
  else if (id.getModuleID() == ID_ROICOMBINE) {
    for (size_t i = 0; i < roi_names.size(); i++) {
      if (roi_names[i] == id.getRoiID()) {
        cout << "got a new value for roi " << roi_names[i] << endl;
        //plotRoi(roi_plot,
        break;
      }
    }

    // not found, add to the names and setup a new plot
    roi_names.push_back(id.getRoiID());
    // TODO: setup new plot
  }
  else if (id.getModuleID() == ID_MOTION) {
    plotMotion(motion_plot, static_cast<RtMotion*>(data));
  }
}
