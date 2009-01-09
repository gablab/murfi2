/******************************************************************************
 * RtFluctuationMonitor.cpp is the implementation of a class that monitors
 * spontaneous fluctuations in the BOLD signal
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-01-16
 *
 *****************************************************************************/

#include"RtFluctuationMonitor.h"
#include"RtMRIImage.h"
#include"RtActivation.h"
#include"RtDataIDs.h"
#include<limits>

string RtFluctuationMonitor::moduleString(ID_FLUCTUATIONMONITOR);


// default constructor
RtFluctuationMonitor::RtFluctuationMonitor() : RtActivationEstimator() {
  componentID = moduleString;
  outputID += ":" + string(ID_FLUCTUATIONMONITOR);

  dataName = NAME_FLUCTUATIONMONITOR_ACTIVATION;

  needsInit = true;
  solvers = NULL;
  numData = 0;
  absEstErrSum = NULL;
  numDataInErrSum = 0;
  numDataPointsForErrEst = INT_MAX;

  triggerStim = false;
  isTriggered = false;
  afterTriggerSkip = 8;
  numImagesSinceTrigger = INT_MAX;
}

// destructor
RtFluctuationMonitor::~RtFluctuationMonitor() {
  if(solvers != NULL) {
    for(unsigned int i = 0; i < numData; i++) {
      if(solvers[i] != NULL) {
	delete solvers[i];
      }
    }
    delete solvers;
  }

  if(absEstErrSum != NULL) {
    delete absEstErrSum;
  }
}

// look for events we triggered
void RtFluctuationMonitor::setData(RtData *data) {
  if(data->getDataID().getDataName() == NAME_EVENTTRIGGER_GOOD
     || data->getDataID().getDataName() == NAME_EVENTTRIGGER_BAD) {
    receiveStimTriggered();
  }
}

// receive a message that stimulus has been triggered
void RtFluctuationMonitor::receiveStimTriggered() {
  cout << "!!!! received trigger event" << endl;
  numImagesSinceTrigger = 0;
  isTriggered = true;
}

// build a row of the GLM deign matrix 
// in
//  current image
double *RtFluctuationMonitor::getDesignMatrixRow(unsigned int timepoint) {
  double *Xrow = getDesignMatrixRow(timepoint);
  
  // copy the event regressor
  if(modelEvents && isTriggered) {
    Xrow[getNuisanceColumn(EVENT,numEvents)] 
      = eventRegressor.get(numImagesSinceTrigger-1);
    cout << "!!!! event column " << Xrow[getNuisanceColumn(EVENT,numEvents)] << endl;
  }
  else {
    cout << "!!!! not event column " << Xrow[getNuisanceColumn(EVENT,numEvents)] << endl;
  }
    
    return Xrow;       // funtction should return a value
}

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node
bool RtFluctuationMonitor::processOption(const string &name, const string &text,
					 const map<string,string> &attrMap) {
  // look for known options
  if(name == "triggerStim") {
    return RtConfigVal::convert<bool>(triggerStim,text);
  }
  if(name == "afterTriggerSkip") {
    return RtConfigVal::convert<int>(afterTriggerSkip,text);
  }
  if(name == "numDataPointsForErrEst") {
    return RtConfigVal::convert<int>(numDataPointsForErrEst,text);    
  }

  return RtActivationEstimator::processOption(name, text, attrMap);
}

// process a single acquisition
int RtFluctuationMonitor::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtFluctuationMonitor::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *dat = (RtMRIImage*)msg->getCurrentData();

  if(dat == NULL) {
    cout << "RtFluctuationMonitor::process: data passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtFluctuationMonitor:process: data passed is NULL\n"));
    return 0;
  }

  //cout << "operating on image: " << dat->getID() << endl;

  numTimepoints++;

  // initialize the computation if necessary
  if(needsInit) {

    // get the number of datapoints we must process
    numData = dat->getNumEl();

    // build the mask image
    initEstimation(*dat);

    // set up the solvers
    solvers = new RtLeastSquaresSolve*[numData];
    for(unsigned int i = 0; i < numData; i++) {
      solvers[i] = new RtLeastSquaresSolve(getNumNuisanceRegressors()
					   +numConditions);
    }

    // setup the estimation error sum of squares
    if(absEstErrSum != NULL) {
      delete absEstErrSum;
    }
    absEstErrSum = new RtActivation(*dat);
    absEstErrSum->initToZeros();

    // set up the event regressor
//    if(modelEvents) {
//      buildEventRegressor(2*afterTriggerSkip);
//    }

    needsInit = false;
  }

  // validate sizes
  if(dat->getNumEl() != numData) {
    ACE_DEBUG((LM_INFO, "RtFluctuationMonitor::process: new data has wrong number of elements\n"));
    cout << "RtFluctuationMonitor::process: new data has wrong number of elements" << endl;
    return -1;
  }

  //debug
    cout << "fluct mon started at ";
    printNow(cout);
    cout << endl;

  // allocate a new data image for the estimation
  RtActivation *fluct = new RtActivation(*dat);

  // setup the data id
  fluct->getDataID().setFromInputData(*dat,*this);
  fluct->getDataID().setDataName(dataName);
  fluct->getDataID().setRoiID(roiID);

  fluct->initToZeros();

  //// element independent setup

  cout << "isTRIGGERED " << isTriggered << endl;

  
  bool dontInclude = false;
  if(numTimepoints > numDataPointsForErrEst) { // check for out of error est region
    dontInclude = true;
  }
  else { // check the time since triggers
    if(isTriggered && afterTriggerSkip >= numImagesSinceTrigger++) {
      cout << "waiting for triggered stim... " << numImagesSinceTrigger << " of "  << afterTriggerSkip << endl;
      
      dontInclude = true;
    }
    else if(isTriggered) {
      isTriggered = false;
      dontInclude = false;
      numEvents++;
    }
  }

  // increment the number of included points
  if(!dontInclude) {
    numDataInErrSum++;      
  }  
  
  // set threshold
  if(numTimepoints > numTrends+1) {
    fluct->setThreshold(getTStatThreshold(1));
    cout << "fluctuation monitor: using t threshold of "
	 << fluct->getThreshold() << endl;
  }

  if(isTriggered && numImagesSinceTrigger++ >= afterTriggerSkip) {
    isTriggered = false;
    numEvents++;
  }

  double *Xrow = getDesignMatrixRow(dat->getDataID().getTimePoint());

  //// compute t map for each element
  //double sum = 0.;
  for(unsigned int i = 0; i < dat->getNumEl(); i++) {
    if(!mask.getPixel(i)) {
      fluct->setPixel(i,fmod(0,0.));
      continue;
    }

    double y = dat->getElement(i);

    solvers[i]->include(&y,Xrow,1.0);

    // old method for discounting time points
//    if(!dontInclude) {
//      solvers[i]->include(&y,Xrow,1.0);
//    }
//    else { // include a dummy timepoint
//      double *beta = solvers[i]->regress(0);
//
//      double est = 0.;
//      // build the estimate
//      for(unsigned int j = 0; j < numTrends; j++) {
//	est += beta[j]*trends.get(numTimepoints-1,j);
//      }
//
//      solvers[i]->include(&est,Xrow,1.0);
//      delete beta;
//    }

    double *beta = solvers[i]->regress(0);
    double err = y;

    // subtract the reconstruction based on trend fit
    for(unsigned int j = 0; j < getNumNuisanceRegressors(); j++) {
      err -= beta[j]*Xrow[j];
    }

    double stdDev;
    if(!dontInclude) {
      absEstErrSum->setPixel(i, absEstErrSum->getPixel(i) + fabs(err));
    }
    stdDev = absEstErrSum->getPixel(i)/(numDataInErrSum-1);

    fluct->setPixel(i, err/stdDev);

    if(dumpAlgoVars && numTimepoints > 2) {
      dumpFile 
	<< numTimepoints << " " 
	<< i << " " 
	<< y << " "
	<< beta[0] << " "
	<< beta[1] << " "
	<< err << " "
	<< stdDev << " "
	<< fluct->getPixel(i) << " "
	<< isTriggered << " "
	<< dontInclude;
      for(int b = 0; b < numConditions; b++) {
	dumpFile << beta[b] << " ";
      }
      dumpFile << endl;
    }

    delete beta;
  }
  //  cout << "sum = " << sum << endl;

  delete Xrow;

  // set the image id for handling
  //fluct->addToID("voxel-fluctmon");
  //fluct->setRoiID(roiID);

  setResult(msg,fluct);

  //debug
    cout << "fluct mon finished at ";
    printNow(cout);
    cout << endl;

  return 0;
}


// start a logfile 
void RtFluctuationMonitor::startDumpAlgoVarsFile() {
  dumpFile << "started at ";
  printNow(dumpFile);
  dumpFile << endl
	   << "time_point "
	   << "voxel_index "
	   << "voxel_intensity "
	   << "mean "
	   << "linear "
	   << "activation_signal "
	   << "std_dev "
	   << "fluctuation "
	   << "trigerred "
	   << "skipping ";
  dumpFile << "end" << endl;  
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/

