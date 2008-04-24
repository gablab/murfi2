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

string RtSingleImageCor::moduleString("singleimcor");

// debugging
#define DUMP 0
#ifdef DUMP
#include<fstream>
static ofstream dumpfile("/tmp/single_image_dump.txt");
#endif

// default constructor
RtSingleImageCor::RtSingleImageCor() : RtActivationEstimator() {
  componentID = moduleString;
   
  baselineThreshold.set_size(1);
  baselineThreshold.put(0,0.0);
  solvers = NULL; 
  numData = 0;
  estErrSumSq = NULL;
}

// destructor
RtSingleImageCor::~RtSingleImageCor() {
  deleteBaselineMeans();

  // delete all solvers
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

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node
bool RtSingleImageCor::processOption(const string &name, const string &text) {
  double tmp;

  // look for known options
  if(name == "baselineThreshold") {
    RtConfigVal::convert<double>(tmp,text);    
    baselineThreshold.put(0,tmp);
    return true;
  }

  return RtActivationEstimator::processOption(name, text);
}


// initialize the estimation algorithm for a particular image size
// in
//  first acquired image to use as a template for parameter inits
void RtSingleImageCor::initEstimation(RtMRIImage &image) {
  RtActivationEstimator::initEstimation(image);

  // create images to store baseline estimates
  initBaselineMeans(&image);

  // get the number of datapoints we must process
  numData = image.getNumEl();

  // set up the solvers
  solvers = new RtLeastSquaresSolve*[numData];
  for(unsigned int i = 0; i < numData; i++) {
    solvers[i] = new RtLeastSquaresSolve(numTrends+numConditions);
  }

  // setup the estimation error sum of squares
  if(estErrSumSq != NULL) {
    delete estErrSumSq;
  }
  estErrSumSq = new RtActivation(image);
  estErrSumSq->initToZeros();


  needsInit = false;
}

// initialize the baseline mean estimation parameters to match an image
// in 
//  mri image to be used as a template to allocate space for running means
void RtSingleImageCor::initBaselineMeans(RtMRIImage *img) {
  // clean up existing versions
  deleteBaselineMeans();

  // allocate vectors and initialize them
  for(unsigned int i = 0; i < numConditions; i++) {
    baselineMeans.push_back(new RtActivation(*img));
  }

  numBaselineTimepoints.set_size(numConditions);
  numBaselineTimepoints.fill(0);
}

// deallocate all baseline mean related images
void RtSingleImageCor::deleteBaselineMeans() {
  for(unsigned int i = 0; i < baselineMeans.size(); i++) {
    if(baselineMeans[i] != NULL) {
      delete baselineMeans[i];
    }
  }
}

// process a single acquisition
int RtSingleImageCor::process(ACE_Message_Block *mb) {
//  static int numComparisons = 0;
//  gp.set_yrange(38000,41000);

  ACE_TRACE(("RtSingleImageCor::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *dat = (RtMRIImage*)msg->getCurrentData();

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
    cout << "single image est: using t threshold of " 
	 << cor->getThreshold() << endl;
  }

  //// element independent setup

  // decide if we are in an "on" or "off" stimulus condition for each condition
  static vnl_vector<unsigned int> isBaseline(numConditions);
  isBaseline.fill(0);

  // decide whether we are in an on or off condition
  for(unsigned int j = 0; j < numConditions; j++) {
    // when in baseline condition, set the flag and increment the 
    // number of timepoints this baseline condition has
    if(conditions->get(numTimepoints-1,j) < baselineThreshold.get(j)) { 
      isBaseline.put(j,1);
      numBaselineTimepoints.put(j,numBaselineTimepoints.get(j)+1);
    }
  }

  // build a design matrix xrow
  double *Xrow = new double[numConditions+numTrends];
  for(unsigned int i = 0; i < numTrends; i++) {
    Xrow[i] = trends->get(numTimepoints-1,i);
  }
  for(unsigned int i = 0; i < numConditions; i++) {
    Xrow[i+numTrends] = conditions->get(numTimepoints-1,i);
  }

  //// compute t map for each element
  for(unsigned int i = 0; i < dat->getNumEl(); i++) {
    if(!mask.getPixel(i)) {
      cor->setPixel(i,fmod(1.0,0.0)); // assign nan
      continue;
    }

    double y = dat->getElement(i);


//    #ifdef DUMP
//    //if(dat->getPixel(i) > 4090) {
//      dumpfile << c->get(i,numTrends+1) << " " << pow(c->get(i,numTrends+1),2) << " " << pow(z_hat/b_old,2) << " " << sqrt(pow(c->get(i,numTrends+1),2)+pow(z_hat/b_old,2)) << endl;
//      //}
//    #endif


    // decide whether we are in an on or off condition
    // NOTE: by testing after the mask we are assuming a constant mask over time
//    for(unsigned int j = 0; j < numConditions; j++) {
//      if(isBaseline.get(j)) { // off
//	// check whether this is the first image of the baseline
//	if(numBaselineTimepoints.get(j) == 1) {	  
//	  // replace the mean with the voxel intensity for first timepoint
//	  baselineMeans[j]->setPixel(i, y);
//	}
//	else {
//	  // sum the voxel intensity within this baseline condition
//	  baselineMeans[j]->setPixel(i, baselineMeans[j]->getPixel(i) + y);
//	}
////
////    if(i == 16*32*32 + 28*32 + 14) {
////      fprintf(stderr,"%f ", baselineMeans[j]->getPixel(i)/(numBaselineTimepoints.get(j) > 0 ? numBaselineTimepoints.get(j) : 1));
////    }    
//      }
//      else {
//	// update the mean
//	if(numBaselineTimepoints.get(j) > 0) {
//	  baselineMeans[j]->setPixel(i, 
//	      baselineMeans[j]->getPixel(i)/(numBaselineTimepoints.get(j)));
//	}
//
////    if(i == 16*32*32 + 28*32 + 14) {
////      fprintf(stderr,"%f ", baselineMeans[j]->getPixel(i));
////    }
//
//
//
//	// change y to be the mean of the last baseline in the on condition
//	// TODO: model trends within the on block (maybe this already happens)	
//	y = baselineMeans[j]->getPixel(i);
//      }
//    }

    // include this timepoint in the solver for this voxel
    solvers[i]->include(&y,Xrow,1.0);

    #ifdef DUMP
    #endif

    // compute the correlation after we have enough timepoints to
    //if(numTimepoints > numTrends+1) {
    double *beta = solvers[i]->regress(0);
    //double *columnSEs = solvers[i]->getSquaredError(0);
    //      double se = sqrt((solvers[i]->getTotalSquaredError(0)
    //			/(numTimepoints-numTrends))
    //		       /columnSEs[numTrends]);

    double err = y;
    for(unsigned int j = 0; j < numTrends; j++) {
      err -= beta[j]*Xrow[j];
    }
    
    // update the error in the estimate for the voxel
    double esterr = err;
    for(unsigned int j = numTrends; j < numTrends+numConditions; j++) {
      esterr -= beta[j]*Xrow[j];
    }
    estErrSumSq->setPixel(i, estErrSumSq->getPixel(i) + esterr*esterr);

    cor->setPixel(i, err * sqrt((numTimepoints)/estErrSumSq->getPixel(i)));
    //cout << est->getPixel(i) << endl;

    delete beta;

//      cor->setPixel(i, sqrt(numTimepoints-numTrends-1)
//		    * c->get(i,numTrends)/c->get(i,numTrends+1));
//      cout << sqrt(numTimepoints-numTrends-1)
//	* c->get(i,numTrends)/c->get(i,numTrends+1) << " ";
//    if(i == 1 && numTimepoints == 2) {
    if(i == 16*32*32 + 28*32 + 14) {
      //#ifdef DUMP
      cout << numTimepoints << " " << i << " " << dat->getPixel(i) << " "
	 << beta[0] << " " 
	 << beta[1] << " " 
	 << beta[2] << " " 
	 << " " <<  cor->getPixel(i) << endl;
    cout.flush();
    }
    //#endif

  }

  // set the image id for handling
  cor->addToID("voxel-singleimcor");
  cor->setRoiID(roiID);

  // in the first non-baseline condition reset the number of baseline timepoints
//  for(unsigned int j = 0; j < numTrends+1; j++) {
//    if(numBaselineTimepoints.get(j) > 0 && !isBaseline.get(j)) {
//      numBaselineTimepoints.put(j,0);
//    }
//  }

  setResult(msg,cor);

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

