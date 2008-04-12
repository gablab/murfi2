/******************************************************************************
 * RtFluctuationMonitor.cpp is the implementation of a class that computes the
 * activation in a single image based on Hinds, et al., 2008
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-01-16
 *
 *****************************************************************************/

#include"RtFluctuationMonitor.h"
#include"RtMRIImage.h"
#include"RtActivation.h"

string RtFluctuationMonitor::moduleString("fluctuationmonitor");


// default constructor
RtFluctuationMonitor::RtFluctuationMonitor() : RtActivationEstimator() {
  id = moduleString;

  needsInit = true;

  solvers = NULL; 
  numData = 0;
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
}

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node
bool RtFluctuationMonitor::processOption(const string &name, const string &text) {
  // look for known options
//  if(name == "baselineThreshold") {
//    RtConfigVal::convert<double>(tmp,text);    
//    baselineThreshold.put(0,tmp);
//    return true;
//  }

  return RtActivationEstimator::processOption(name, text);
}

// process a single acquisition
int RtFluctuationMonitor::process(ACE_Message_Block *mb) {
//  static int numComparisons = 0;
//  gp.set_yrange(38000,41000);

  ACE_TRACE(("RtFluctuationMonitor::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *dat = (RtMRIImage*)msg->getCurrentData();

  if(dat == NULL) {
    cout << "RtFluctuationMonitor::process: data passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtFluctuationMonitor:process: data passed is NULL\n"));
    return 0;
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

    needsInit = false;
  }

  // validate sizes
  if(dat->getNumEl() != numData) {
    ACE_DEBUG((LM_INFO, "RtFluctuationMonitor::process: new data has wrong number of elements\n"));
    return -1;    
  }

  // allocate a new data image for the estimation
  RtActivation *fluct = new RtActivation(*dat);
  fluct->initToZeros();

  //// element independent setup
  
  // build a design matrix xrow
  double *Xrow = new double[numConditions+numTrends];
  for(unsigned int i = 0; i < numTrends; i++) {
    Xrow[i] = trends->get(numTimepoints-1,i);
  }

  //// compute t map for each element
  double sum = 0.;
  for(unsigned int i = 0; i < dat->getNumEl(); i++) {
    if(!mask.getPixel(i)) {
      fluct->setPixel(i,0.0);
      continue;
    }

    double y = dat->getElement(i);
    solvers[i]->include(&y,Xrow,1.0);

    if(numTimepoints > numTrends) {
      double *beta = solvers[i]->regress(0);
      double *columnSEs = solvers[i]->getSquaredError(0);
//      double se = sqrt((solvers[i]->getTotalSquaredError(0)
//			/(numTimepoints-numTrends))
//		       /columnSEs[numTrends]);
      double err = y;

      // subtract the reconstruction based on trend fit
      for(unsigned int j = 0; j < numTrends; j++) {
	err -= beta[j]*trends->get(numTimepoints-1,j);
      }

      // transform to z score
      err/=columnSEs[numTrends-1];

      sum += err;

      fluct->setPixel(i, err);

      //cout << fluct->getPixel(i) << endl;
      //fluct->setPixel(i, beta[numTrends]/se);

      delete columnSEs;
      delete beta;
    }

  }  
  cout << "sum = " << sum << endl;

  delete Xrow;

  // set the image id for handling
  fluct->addToID("voxel-fluctmon");

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

