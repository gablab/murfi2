/******************************************************************************
 * RtSingleImageCor.cpp is the implementation of a class that computes the
 * activation in a single image based on Hinds, et al., 2008
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-01-16
 *
 *****************************************************************************/

#include"RtSingleImageCor.h"
#include"RtMRIImage.h"
#include"RtActivation.h"
#include<limits>

string RtSingleImageCor::moduleString("singleimcor");

// default constructor
RtSingleImageCor::RtSingleImageCor() : RtActivationEstimator() {
  componentID = moduleString;
   
  solvers = NULL; 
  numData = 0;
  absEstErrSum = NULL;
  numDataPointsForErrEst = INT_MAX;
  onlyEstErrInBaseline = false;

  conditionOfInterest = 0;
  feedbackConditionSwitching = false;
}

// destructor
RtSingleImageCor::~RtSingleImageCor() {
  // delete all solvers
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

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node
bool RtSingleImageCor::processOption(const string &name, const string &text) {

  // look for known options
  if(name == "numDataPointsForErrEst") {
    return RtConfigVal::convert<int>(numDataPointsForErrEst,text);    
  }
  if(name == "onlyEstErrInBaseline") {
    return RtConfigVal::convert<bool>(onlyEstErrInBaseline,text);    
  }
  if(name == "conditionOfInterest") {
    return RtConfigVal::convert<unsigned int>(conditionOfInterest,text);    
  }
  if(name == "feedbackConditionSwitching") {
    return RtConfigVal::convert<bool>(feedbackConditionSwitching,text);    
  }

  return RtActivationEstimator::processOption(name, text);
}

// initialize the estimation algorithm for a particular image size
// in
//  first acquired image to use as a template for parameter inits
void RtSingleImageCor::initEstimation(RtMRIImage &image) {
  RtActivationEstimator::initEstimation(image);

  // get the number of datapoints we must process
  numData = image.getNumEl();

  // set up the solvers
  solvers = new RtLeastSquaresSolve*[numData];
  for(unsigned int i = 0; i < numData; i++) {
    solvers[i] = new RtLeastSquaresSolve(numTrends+numConditions);
  }

  // setup the estimation error sum of squares
  if(absEstErrSum != NULL) {
    delete absEstErrSum;
  }
  absEstErrSum = new RtActivation(image);
  absEstErrSum->initToZeros();

  numDataPointsInErrEst = 0;

  needsInit = false;
}

// process a single acquisition
int RtSingleImageCor::process(ACE_Message_Block *mb) {
//  static int numComparisons = 0;
//  gp.set_yrange(38000,41000);

  ACE_TRACE(("RtSingleImageCor::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *dat = (RtMRIImage*) msg->getCurrentData();

  if(dat == NULL) {
    cout << "RtSingleImageCor::process: data passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtSingleImageCor:process: data passed is NULL\n"));
    return 0;
  }

  numTimepoints++;

  // initialize the computation if necessary
  if(needsInit) {
    initEstimation(*dat);
  }

  // validate sizes
  if(dat->getNumEl() != numData) {
    ACE_DEBUG((LM_INFO, "RtSingleImageCor::process: new data has wrong number of elements\n"));
    return -1;
  }

  // allocate a new data image for the correlation
  RtActivation *cor = new RtActivation(*dat);
  cor->initToZeros();

  // set threshold
  if(numTimepoints > numTrends+1) {
    cor->setThreshold(getTStatThreshold(1));
//    cout << "single image est: using t threshold of " 
//	 << cor->getThreshold() << endl;
  }

  //// element independent setup

  /// build a design matrix xrow

  // copy the trends
  double *Xrow = new double[numConditions+numTrends];
  for(unsigned int i = 0; i < numTrends; i++) {
    Xrow[i] = trends.get(numTimepoints-1,i);
  }

  // copy the conditions
  // also: search for the maximum magnitude condition regressor 
  // also: check for any regressor over zero
  unsigned int maxMagnitudeCondInd = 0;
  bool anyOverZero = false;
  for(unsigned int i = 0; i < numConditions; i++) {
    Xrow[i+numTrends] = conditions.get(numTimepoints-1,i);

    // check for max amplitude condition
    if(fabs(Xrow[numTrends+i]) > fabs(Xrow[numTrends+maxMagnitudeCondInd])
       && !(modelTemporalDerivatives && conditionIsDerivative(i))
       ) {
      maxMagnitudeCondInd = numTrends+i;
    }
    // check for on condition for any stimulus
    if(Xrow[i+numTrends] > 0) {
      anyOverZero = true;
    }
  }

  // switch conditions to max magnitude if condition switching on
  if(feedbackConditionSwitching) {
    conditionOfInterest = maxMagnitudeCondInd;
    cout << "condition of interest is " << conditionOfInterest << endl;
  }

  // check if we should include this timepoint in variance computation
  bool includeInErr;
  if((numTimepoints > numDataPointsForErrEst)
     || (onlyEstErrInBaseline && anyOverZero)) {
    includeInErr = false;
  }
  else {
    includeInErr = true;
    numDataPointsInErrEst++;
  }

  //// compute t map for each element
  for(unsigned int i = 0; i < dat->getNumEl(); i++) {
    if(!mask.getPixel(i)) {
      cor->setPixel(i,fmod(1.0,0.0)); // assign nan
      continue;
    }

    // include this timepoint in the solver for this voxel
    double y = dat->getElement(i);
    solvers[i]->include(&y,Xrow,1.0);

    // get betas
    double *beta = solvers[i]->regress(0);

    // get activation signal take out everything except regressor of interest
    unsigned int regressorOfInterestIndex = conditionOfInterest;
    if(modelEachBlock) {
      unsigned int blockNum = (numTimepoints > conditionShift)
	? (numTimepoints-1-conditionShift)/blockLen : 0;
      regressorOfInterestIndex 
	= conditionOfInterest * numMeas/blockLen + blockNum;
    }
    if(modelTemporalDerivatives) {
      regressorOfInterestIndex*=2;
      //regressorOfInterestIndex++;
    }
    regressorOfInterestIndex+=numTrends;

    double err = y;
    for(unsigned int j = 0; j < numTrends+numConditions; j++) {
      if(j == regressorOfInterestIndex) {
	continue;
      }
      err -= beta[j]*Xrow[j];
    }

    // get estimation error
    double esterr = err;
    //for(unsigned int j = numTrends; j < numTrends+numConditions; j++) {
    esterr -= beta[regressorOfInterestIndex]*Xrow[regressorOfInterestIndex];
    //}
    
    // update the error in the estimate for the voxel
    double stdDev;
    // check if we should include this timepoint in the error estimate
    if(includeInErr) {
      absEstErrSum->setPixel(i, absEstErrSum->getPixel(i) + fabs(esterr));
    }
    stdDev = absEstErrSum->getPixel(i)/(numDataPointsInErrEst-1);

    // compute the sds away from the mean
    cor->setPixel(i, err / stdDev);

    if(dumpAlgoVars && numTimepoints > 2) {
      dumpFile 
	<< numTimepoints << " " 
	<< i << " " 
	<< y << " "
	<< err << " "
	<< conditions.get(numTimepoints-1,0) << " "
	<< esterr << " "
	<< stdDev << " "
	<< cor->getPixel(i) << " ";
      for(int b = 0; b < numTrends+numConditions; b++) {
	dumpFile << beta[b] << " ";
      }
      dumpFile << endl;
    }

    delete beta;

  }

  // set the image id for handling
  cor->addToID("voxel-singleimcor");
  cor->setRoiID(roiID);

  cout << "done processing single image correlation at ";
  printNow(cout);
  cout << endl;

  setResult(msg,cor);

  return 0;
}

// start a logfile 
void RtSingleImageCor::startDumpAlgoVarsFile() {
  dumpFile << "started at ";
  printNow(dumpFile);
  dumpFile << endl
	   << "time_point "
	   << "voxel_index "
	   << "voxel_intensity "
	   << "activation_signal "
	   << "condition "
	   << "residual "
	   << "std_dev "
	   << "feedback ";
  for(int b = 0; b < numTrends+numConditions; b++) {
    dumpFile << "beta[" << b << "] ";
  }

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

