/*=========================================================================
 *  RtCurrentActivation.cpp is the implementation of a class
 *
 * ****************************************************************************/
#include "RtDesignMatrix.h"

#include"RtCurrentActivation.h"
#include"RtMRIImage.h"
#include"RtActivation.h"
#include"RtDataIDs.h"
#include"RtExperiment.h"
#include"RtElementAccess.h"
#include"util/timer/timer.h"

#include"debug_levels.h"

string RtCurrentActivation::moduleString(ID_CURRENTACTIVATION);

// default constructor
RtCurrentActivation::RtCurrentActivation() : RtStreamComponent() {

  componentID = moduleString;
  steadyStateResidual = NULL;
  numDataPointsForErrEst = std::numeric_limits<unsigned int>::max();
}

// destructor
RtCurrentActivation::~RtCurrentActivation() {
}

// initialize the estimation algorithm for a particular image size
void RtCurrentActivation::initEstimation(RtMRIImage &dat, RtMaskImage *mask) {

}

// process an option in name of the option to process val text of the option
// node
bool RtCurrentActivation::processOption(const string &name, const string &text,
                                        const map<string, string> &attrMap) {

  // look for known options
  if (name == "modelFitModuleID") {
    modelFitModuleID = text;
    return true;
  }

  if (name == "modelFitRoiID") {
    modelFitRoiID = text;
    return true;
  }

  if (name == "numDataPointsForErrEst") {
    return RtConfigVal::convert<unsigned int>(numDataPointsForErrEst, text);
  }

  if (name == "saveResult") {
    return RtConfigVal::convert<bool>(saveResult, text);
  }

  return RtStreamComponent::processOption(name, text, attrMap);
}

// validate the configuration
bool RtCurrentActivation::validateComponentConfig() {
  bool result = true;

  if (modelFitModuleID.empty()) {
    cerr << "RtCurrentActivation::process: modelFitModuleID is empty" << endl;
    result = false;
  }

  if (modelFitRoiID.empty()) {
    cerr << "RtCurrentActivation::process: modelFitRoiID is empty" << endl;
    result = false;
  }

  return result;
}

// process a single acquisition
int RtCurrentActivation::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtCurrentActivation::process"));

  timer tim;
  if(printTiming) {
    tim.start();
  }

  // get pointer to message
  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *dat = (RtMRIImage*) msg->getCurrentData();

  // check for validity of data
  if (dat == NULL) {
    cerr << "RtCurrentActivation::process: data passed is NULL" << endl;

    if(logOutput) {
      stringstream logs("");
      logs << "RtCurrentActivation::process: data passed is NULL" << endl;
      log(logs);
    }

    return 0;
  }

  // get mask from msg
  RtMaskImage *mask = getMaskFromMessage(*msg);

  // check validity of mask
  if (mask == NULL) {
    cerr << "RtCurrentActivation::process: mask is NULL" << endl;

    if(logOutput) {
      stringstream logs("");
      logs << "RtCurrentActivation::process: mask is NULL at tr "
           << dat->getDataID().getTimePoint() << endl;
      log(logs);
    }

    return 0;
  }

  // initialize the computation if necessary
  if (needsInit) {
    initEstimation(*dat, mask);
  }

  // get design
  // TODO this may not work if there are more than one design matrix
  RtDataID tempDataID;
  tempDataID.setDataName(NAME_DESIGN);

  // debug
  // getDataStore().getAvailableData();

  RtDesignMatrix *design = static_cast<RtDesignMatrix*>(
      getDataStore().getData(tempDataID));

  if(design == NULL) {
    cerr << "error: could not find design matrix in datastore!" << endl;
    cerr << "searched for design matrix id: " << tempDataID << endl;
    return 0;
  }

  // allocate a new data image for the stats
  RtActivation *currentActivation = new RtActivation(*dat);

  // setup the data id
  currentActivation->getDataID().setFromInputData(*dat, *this);
  currentActivation->getDataID().setDataName(NAME_ACTIVATION);
  currentActivation->getDataID().setRoiID(modelFitRoiID);

  currentActivation->initToNans();

  // get the residual and the beta images for discounting nuissance
  // signals

  // find the betas
  RtActivation **betas = new RtActivation*[design->getNumColumns()];
  for(unsigned int j = 0; j < design->getNumColumns(); j++) {
    betas[j] = (RtActivation*) msg->getData(modelFitModuleID,
                                            string(NAME_BETA)
                                            + "_" +
                                            design->getColumnName(j),
                                            modelFitRoiID);
    // check for found
    if (betas[j] == NULL) {
      cerr << "RtCurrentActivation:process: beta " << j << " is null" << endl;

      if(logOutput) {
        stringstream logs("");
        logs << "RtCurrentActivation::process: beta " << j
             << " is NULL at tr "
             << dat->getDataID().getTimePoint() << endl;
        log(logs);
      }

      return 0;
    }
  }

  // get residual from message if timepoint is less than
  // numDataPointsForErrEst otherwise use the steady state
  // residual value
  RtActivation *residual;
  if (dat->getDataID().getTimePoint() < numDataPointsForErrEst) {
    // get residual off of msg
    residual = (RtActivation *) msg->getData(modelFitModuleID,
                                             NAME_RESIDUAL_MSE,
                                             modelFitRoiID);
    // save off residual
    steadyStateResidual = residual;
  }
  else {
    // post-numDataPointsForErrEst, use saved residual
    residual = steadyStateResidual;
    residual->protectFromPurge();
  }

  // check that residual is not null
  if (residual == NULL) {
    cerr << "RtCurrentActivation:process: residual is null" << endl;

    if(logOutput) {
      stringstream logs("");
      logs << "RtCurrentActivation::process: residual is NULL at tr "
           << dat->getDataID().getTimePoint() << endl;
      log(logs);
    }

    return 0;
  }

  // get this design matrix row
  vnl_vector<double> Xrow = design->getRow(dat->getDataID().getTimePoint()-1);

  // include this timepoint for each voxel
  RtElementAccess datAc(dat, mask);
  RtElementAccess resAc(residual, mask);
  vector<unsigned int> inds = datAc.getElementIndices();
  for(vector<unsigned int>::iterator it = inds.begin();
      it != inds.end(); it++) {
    // get voxel intensity
    double y = datAc.getDoubleElement(*it);

    double *betavals = new double[Xrow.size()];

    // compute error
    double err = y;
    for (unsigned int j = 0; j < Xrow.size(); j++) {
      if (!design->isColumnOfInterest(j)) {
        double beta = betas[j]->getPixel(*it);
        err -= beta * Xrow[j];
        betavals[j] = beta;
      }
      else { // for debug output
        betavals[j] = betas[j]->getPixel(*it);
      }
    }

    // compute the dev and current activation (magic happens here)
    double dev = sqrt(resAc.getDoubleElement(*it)
                      / (residual->getDataID().getTimePoint()));
    currentActivation->setPixel(*it, err / dev);

    if (dumpAlgoVars && dat->getDataID().getTimePoint() > 2) {
      dumpFile
          << dat->getDataID().getTimePoint() << " "
          << *it << " "
          << y << " "
          << err << " "
          << Xrow[0] << " "
          << residual->getPixel(*it) << " "
          << dev << " "
          << currentActivation->getPixel(*it) << " ";
      for (unsigned int b = 0; b < design->getNumColumns(); b++) {
        dumpFile << betavals[b] << " ";
      }
      dumpFile << endl;
    }

    delete [] betavals;

  }
  setResult(msg, currentActivation);
  setResult(msg, residual);

  delete [] betas;


  if(printTiming) {
    tim.stop();
    cout << "RtCurrentActivation process at tr "
         << dat->getDataID().getTimePoint()
         << " elapsed time: " << tim.elapsed_time()*1000 << "ms"  << endl;
  }

  if(print) {
    cout << "RtCurrentActivation: done at tr "
         << dat->getDataID().getTimePoint() << endl;
  }

  if(logOutput) {
    stringstream logs("");
    logs << "RtCurrentActivation: done at tr "
         << dat->getDataID().getTimePoint() << endl;
    log(logs);
  }

  if(saveResult) {
    string fn = getExperimentConfig().getVolFilename(
        dat->getDataID().getSeriesNum(),
        dat->getDataID().getTimePoint());
    string stem = getExperimentConfig().get("study:volumeFileStem").str();
    currentActivation->setFilename(fn.replace(fn.rfind(stem), stem.size(),
                                              "curact"));
    currentActivation->save();
  }

  return 0;
}

// start a logfile
void RtCurrentActivation::startDumpAlgoVarsFile() {
  dumpFile << "started at ";
  printNow(dumpFile);
  dumpFile << endl
           << "time_point "
           << "voxel_index "
           << "voxel_intensity "
           << "activation_signal "
           << "condition "
           << "residual "
           << "dev "
           << "current_activation "; // aka feedback
  for (int b = 0; b < 3; b++) {
    dumpFile << "beta[" << b << "] ";
  }

  dumpFile << "end" << endl;
}
