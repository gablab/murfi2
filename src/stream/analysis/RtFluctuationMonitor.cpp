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
#include<limits>

string RtFluctuationMonitor::moduleString("fluctuationmonitor");


// default constructor
RtFluctuationMonitor::RtFluctuationMonitor() : RtActivationEstimator() {
  id = moduleString;

  needsInit = true;
  solvers = NULL;
  numData = 0;
  estErrSumSq = NULL;

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

  if(estErrSumSq != NULL) {
    delete estErrSumSq;
  }
}

// receive a message that stimulus has been triggered
void RtFluctuationMonitor::receiveStimTriggered() {
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

  // check the time since triggers
  if(isTriggered && afterTriggerSkip >= numImagesSinceTrigger++) {
    cout << "waiting for triggered stim... " << numImagesSinceTrigger << " of "  << afterTriggerSkip << endl;
    return 0;
  }
  else if(isTriggered) {
    isTriggered = false;
  }

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
    if(estErrSumSq != NULL) {
      delete estErrSumSq;
    }
    estErrSumSq = new RtActivation(*dat);
    estErrSumSq->initToZeros();

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

  // set threshold
  if(numTimepoints > numTrends+1) {
    fluct->setThreshold(getTStatThreshold(1));
    cout << "fluctuation monitor: using t threshold of "
	 << fluct->getThreshold() << endl;
  }

  // build a design matrix xrow
  double *Xrow = new double[numConditions+numTrends];
  for(unsigned int i = 0; i < numTrends; i++) {
    Xrow[i] = trends->get(numTimepoints-1,i);
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

    if(numTimepoints > numTrends) {
      double *beta = solvers[i]->regress(0);
      double err = y;

      // subtract the reconstruction based on trend fit
      for(unsigned int j = 0; j < numTrends; j++) {
	err -= beta[j]*trends->get(numTimepoints-1,j);
      }

      estErrSumSq->setPixel(i, estErrSumSq->getPixel(i) + err*err);

      fluct->setPixel(i,
	     sqrt((numTimepoints-1)/estErrSumSq->getPixel(i)) * err);

      //sum += fluct->getPixel(i);

      //cout << fluct->getPixel(i) << endl;
      //fluct->setPixel(i, beta[numTrends]/se);

      //if(i == 16*32*32 + 28*32 + 14) {
//      //#ifdef DUMP
//    if(1) {
//      cerr  << numTimepoints << " " << dat->getPixel(i)
//	    << " " << fluct->getPixel(i) << " " << beta[0] << " "
//	    << beta[1] << " " << beta[0] + beta[1]*numTimepoints
//	    << " " <<  estErrSumSq->getPixel(i)
//	    << endl;
//      //dumpfile.flush();
//      //#endif
//    }
//
//      cout << "z: "; printVnlVector(*z); cout << endl;
//      cout << "f: "; printVnlVector(*f); cout << endl;
//      cout << "g: "; printVnlVector(*g); cout << endl;
//      cout << "h: "; printVnlVector(*h); cout << endl;
//      cout << "C: " << endl;
//      C->print(cout);
//      cout << "c: "; printVnlVector(c->get_row(i)); cout << endl;
//
//      //if(i > 40) {
//	int trash;
//	cin >> trash;
//      //}
//    }


      delete beta;
    }

  }
  //  cout << "sum = " << sum << endl;

  delete Xrow;

  // set the image id for handling
  fluct->addToID("voxel-fluctmon");
  fluct->setRoiID(roiID);

  setResult(msg,fluct);


  return 0;
}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/

