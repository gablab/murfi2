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

#include"debug_levels.h"

// put this somewhere else
#ifndef max 
#define max(a,b) ((a) > (b) ? (a) : (b))
#endif


string RtSingleImageCor::moduleString(ID_SINGLEIMCOR);

// default constructor
RtSingleImageCor::RtSingleImageCor() : RtActivationEstimator() {
  componentID = moduleString;
  dataName = NAME_SINGLEIMCOR_ACTIVATION;
   
  solvers = NULL; 
  numData = 0;

  //  residualSumSqImg = NULL;

  estErrSum = NULL;
  numDataPointsForErrEst = INT_MAX;
  errorNorm = L2;

  onlyEstErrInBaseline = false;

  conditionOfInterest = 0;
  feedbackConditionSwitching = false;

  includeConditionMean = true;
}

// destructor
RtSingleImageCor::~RtSingleImageCor() {
  // delete all solvers
  if(solvers != NULL) {
    freeSolvers();
  }

  if(estErrSum != NULL) {
    freeEstErrSum();
  }

}

void RtSingleImageCor::freeSolvers() {
  for(unsigned int i = 0; i < numData; i++) {
    if(solvers[i] != NULL) {
      delete solvers[i];
    }
  }
  delete solvers;
}

void RtSingleImageCor::freeEstErrSum() {
  for(unsigned int i = 0; i < numConditions; i++) {
    if(estErrSum[i] != NULL) {
      delete estErrSum[i];
    }
  }
  delete estErrSum;
}

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node
bool RtSingleImageCor::processOption(const string &name, const string &text,
				     const map<string,string> &attrMap) {

  // look for known options
  if(name == "numDataPointsForErrEst") {
    return RtConfigVal::convert<int>(numDataPointsForErrEst,text);    
  }
  if(name == "errorNorm") {
    if(text == "l1" || text == "L1") {
      errorNorm = L1;
      return true;
    } 
    else if(text == "l2" || text == "L2") {
      errorNorm = L2;
      return true;
    }    
    else if(text == "linf" || text == "LINF" || text == "LInf") {
      errorNorm = LINF;
      return true;
    }    
  }
  if(name == "includeConditionMean") {
    return RtConfigVal::convert<bool>(includeConditionMean,text);    
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

  return RtActivationEstimator::processOption(name, text, attrMap);
}

// initialize the estimation algorithm for a particular image size
// in
//  first acquired image to use as a template for parameter inits
void RtSingleImageCor::initEstimation(RtMRIImage &image) {
  RtActivationEstimator::initEstimation(image);

  // get the number of datapoints we must process
  numData = image.getNumEl();

  if(solvers != NULL) {
    freeSolvers();
  }

  // set up the solvers
  solvers = new RtLeastSquaresSolve*[numData];
  for(unsigned int i = 0; i < numData; i++) {
    solvers[i] = new RtLeastSquaresSolve(numNuisance+numConditions);
  }

  // setup the estimation error sum of squares
  if(estErrSum != NULL) {
    freeEstErrSum();
  }

  estErrSum = new RtActivation*[numConditions];
  for(unsigned int i = 0; i < numConditions; i++) {
    estErrSum[i] = new RtActivation(image);
    estErrSum[i]->initToZeros();
  }

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

  // allocate a new data images for the stats
  RtActivation **stats = new RtActivation*[numConditions];
  for(unsigned int i = 0; i < numConditions; i++) {
    stats[i] = new RtActivation(*dat);

    // setup the data id
    stats[i]->getDataID().setFromInputData(*dat,*this);
    stats[i]->getDataID().setDataName(dataName+"_"+conditionNames[i]);
    stats[i]->getDataID().setRoiID(roiID);

    stats[i]->initToZeros();

    // set threshold
    if(numTimepoints > numNuisance+1) {
      stats[i]->setThreshold(getTStatThreshold(1));
      if(DEBUG_LEVEL & BASIC) {
	cerr << "single image est: using t threshold of " 
	     << stats[i]->getThreshold() << endl;
      }
    }
  }

  // residual sum of squares map
  RtActivation *res = new RtActivation(*dat);
  // setup the data id
  res->getDataID().setFromInputData(*dat,*this);
  res->getDataID().setDataName(NAME_SINGLEIMCOR_RESIDUAL);
  res->getDataID().setRoiID(roiID);
  res->initToZeros();

  //// element independent setup

  /// build a design matrix row
  int curCol = 0;

  // copy the trends
  double *Xrow = new double[numConditions+numNuisance];
  for(unsigned int i = 0; i < numTrends; i++, curCol++) {
    Xrow[curCol] = trends.get(numTimepoints-1,i);
  }

  // copy the motion parameters
  if(modelMotionParameters) {
    Xrow[curCol++] = dat->getMotionParameter(MOTION_TRANSLATION_X);
    Xrow[curCol++] = dat->getMotionParameter(MOTION_TRANSLATION_Y);
    Xrow[curCol++] = dat->getMotionParameter(MOTION_TRANSLATION_Z);

    Xrow[curCol++] = dat->getMotionParameter(MOTION_ROTATION_X);
    Xrow[curCol++] = dat->getMotionParameter(MOTION_ROTATION_Y);
    Xrow[curCol++] = dat->getMotionParameter(MOTION_ROTATION_Z);  
  }  

  // copy the conditions
  // also: search for the maximum magnitude condition regressor 
  // also: check for any regressor over zero
  unsigned int maxMagnitudeCondInd = 0;
  bool anyOverZero = false;
  for(unsigned int i = 0; i < numConditions; i++, curCol++) {
    Xrow[curCol] = conditions.get(numTimepoints-1,i);

    // check for max amplitude condition
    if(fabs(Xrow[numNuisance+i]) > fabs(Xrow[numNuisance+maxMagnitudeCondInd])
       && !(modelTemporalDerivatives && conditionIsDerivative(i))
       ) {
      maxMagnitudeCondInd = numNuisance+i;
    }
    // check for on condition for any stimulus
    if(Xrow[i+numNuisance] > 0) {
      anyOverZero = true;
    }
  }

  if(DEBUG_LEVEL & TEMP) {
    cout << " xrow " << numTimepoints << ":";
    for(int i = 0; i < numNuisance+numConditions; i++) {
      cout << Xrow[i] << " ";
    }
    cout << endl;
  } 

  // switch conditions to max magnitude if condition switching on
  if(feedbackConditionSwitching) {
    conditionOfInterest = maxMagnitudeCondInd;
    if(DEBUG_LEVEL & ADVANCED) {
      cerr << "condition of interest is " << conditionOfInterest << endl;
    }
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
      for(int c = 0; c < numConditions; c++) {
	stats[c]->setPixel(i,fmod(1.0,0.0)); // assign nan
      }
      continue;
    }

    // include this timepoint in the solver for this voxel
    double y = dat->getElement(i);
    solvers[i]->include(&y,Xrow,1.0);

    // get stats and residual
    double *beta = solvers[i]->regress(0);

    // note that residual is always the standard L2 version, despite which
    // norm we are using for the stat scaling
    res->setPixel(i, sqrt(solvers[i]->getTotalSquaredError(0)/(numTimepoints-1)));

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
    regressorOfInterestIndex += numNuisance;

    // compute stats for each condition
    for(int c = 0; c < numConditions; c++) {
      double meanCondActivity = 0;
      int condIndex = c + numNuisance;
      double err = y;
      for(unsigned int j = 0; j < numNuisance+numConditions; j++) {
	if(j == condIndex) {
	  meanCondActivity = beta[j]*Xrow[j];
	} 
	err -= beta[j]*Xrow[j];
      }

      if(DEBUG_LEVEL & ADVANCED) {
	static double lastErr = 0;
	cerr << i << ": err is " << err << " ?= " << sqrt(solvers[i]->getTotalSquaredError(0)) - lastErr << endl;
	lastErr = sqrt(solvers[i]->getTotalSquaredError(0));
      }

    
      // update the error in the estimate for the voxel

      // get estimation error and compute the standard deviation based on
      // the error sum and current error norm and include the error in the
      // estimate if desired
      double dev;
      switch(errorNorm) {
        case L1:
	  if(includeInErr) {
	    estErrSum[c]->setPixel(i, estErrSum[c]->getPixel(i) + fabs(err));
	  }

	  dev = estErrSum[c]->getPixel(i)/(numDataPointsInErrEst-1);
	  break;

        case L2:
        default:
	  if(includeInErr) {
	    estErrSum[c]->setPixel(i, estErrSum[c]->getPixel(i) + err*err);
	  }

	  dev = sqrt(estErrSum[c]->getPixel(i)/(numDataPointsInErrEst-1));
	  break;

        case LINF:
	  if(includeInErr) {
	    estErrSum[c]->setPixel(i, max(estErrSum[c]->getPixel(i),fabs(err)));
	  }

	  dev = estErrSum[c]->getPixel(i);

	  break;
      }

      if(DEBUG_LEVEL & ADVANCED) {
	cerr << dev << " ?= " 
	     << sqrt(solvers[i]->getTotalSquaredError(0)/(numTimepoints-1))
	     << endl;
      }


      // compute the sds away from the mean (magic kinda happens here)
      if(includeConditionMean) {
	stats[c]->setPixel(i, (meanCondActivity + err) / dev);
      }
      else {
	stats[c]->setPixel(i, (meanCondActivity) + (err / dev));
      }

      if(DEBUG_LEVEL & ADVANCED) {
  	cerr 
	  << numTimepoints << " " 
	  << i << " " 
	  << y << " "
	  << err + meanCondActivity << " "
	  << conditions.get(numTimepoints-1,0) << " "
	  << err << " "
	  << dev << " "
	  << stats[c]->getPixel(i) << " " << endl;
      }
      if(DEBUG_LEVEL & MODERATE) {        
	cerr << err << "/" << dev << "=" <<  stats[c]->getPixel(i) << endl;
      }

      if(dumpAlgoVars && numTimepoints > 2) {
	dumpFile 
	  << numTimepoints << " " 
	  << i << " " 
	  << y << " "
	  << err + meanCondActivity << " "
	  << conditions.get(numTimepoints-1,0) << " "
	  << err << " "
	  << dev << " "
	  << stats[c]->getPixel(i) << " ";
	for(int b = 0; b < numNuisance+numConditions; b++) {
	  dumpFile << beta[b] << " ";
	}
	dumpFile << endl;
      }
    }

    delete beta;
  }

  if(DEBUG_LEVEL & BASIC) {
    cout << "done processing single image correlation at ";
    printNow(cout);
    cout << endl;
  }

  // set the results
  for(int c = 0; c < numConditions; c++) {
    setResult(msg,stats[c]);
  }
  delete [] stats;

  setResult(msg,res);


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
  for(int b = 0; b < numNuisance+numConditions; b++) {
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

