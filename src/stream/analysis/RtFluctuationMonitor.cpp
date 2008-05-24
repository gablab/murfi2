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

string RtFluctuationMonitor::moduleString("fluctuationmonitor");


// default constructor
RtFluctuationMonitor::RtFluctuationMonitor() : RtActivationEstimator() {
  componentID = moduleString;
  outputID += ":" + string(ID_FLUCTUATIONMONITOR);

  needsInit = true;
  solvers = NULL;
  numData = 0;
  absEstErrSum = NULL;
  numDataInErrSum = 0;

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
  if(data->getID().find(ID_EVENTTRIGGER) != string::npos) {
    receiveStimTriggered();
  }
}

// receive a message that stimulus has been triggered
void RtFluctuationMonitor::receiveStimTriggered() {
  cout << "!!!! received trigger event" << endl;
  numImagesSinceTrigger = 0;
  isTriggered = true;
}

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node
bool RtFluctuationMonitor::processOption(const string &name, const string &text) {
  // look for known options
  if(name == "triggerStim") {
    return RtConfigVal::convert<bool>(triggerStim,text);
  }
  if(name == "afterTriggerSkip") {
    return RtConfigVal::convert<int>(afterTriggerSkip,text);
  }

  return RtActivationEstimator::processOption(name, text);
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

  cout << "operating on image: " << dat->getID() << endl;

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
      solvers[i] = new RtLeastSquaresSolve(numTrends+numConditions);
    }

    // setup the estimation error sum of squares
    if(absEstErrSum != NULL) {
      delete absEstErrSum;
    }
    absEstErrSum = new RtActivation(*dat);
    absEstErrSum->initToZeros();

    needsInit = false;
  }

  // validate sizes
  if(dat->getNumEl() != numData) {
    ACE_DEBUG((LM_INFO, "RtFluctuationMonitor::process: new data has wrong number of elements\n"));
    cout << "RtFluctuationMonitor::process: new data has wrong number of elements" << endl;
    return -1;
  }

  // allocate a new data image for the estimation
  RtActivation *fluct = new RtActivation(*dat);
  fluct->initToZeros();

  //// element independent setup

  // check the time since triggers
  bool dontInclude = false;
  if(isTriggered && afterTriggerSkip >= numImagesSinceTrigger++) {
    cout << "waiting for triggered stim... " << numImagesSinceTrigger << " of "  << afterTriggerSkip << endl;

    dontInclude = true;
  }
  else if(isTriggered) {
    isTriggered = false;
    dontInclude = false;
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

  // build a design matrix xrow
  double *Xrow = new double[numConditions+numTrends];
  for(unsigned int i = 0; i < numTrends; i++) {
    Xrow[i] = trends.get(numTimepoints-1,i);
  }

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
    for(unsigned int j = 0; j < numTrends; j++) {
      err -= beta[j]*trends.get(numTimepoints-1,j);
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
	<< err << " "
	<< stdDev << " "
	<< fluct->getPixel(i) << " "
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
  fluct->addToID("voxel-fluctmon");
  fluct->setRoiID(roiID);

  setResult(msg,fluct);


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
	   << "activation_signal "
	   << "std_dev "
	   << "fluctuation "
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

