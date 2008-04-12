/******************************************************************************
 * RtIncrementalGLM.h is the implementation of a class that computes 
 * activation at each voxel incrementally using Gentleman's method
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-04-01
 *
 *****************************************************************************/

#include"RtIncrementalGLM.h"
#include"RtMRIImage.h"

string RtIncrementalGLM::moduleString("incrglm");

// default constructor
RtIncrementalGLM::RtIncrementalGLM() : RtActivationEstimator() {
  id = moduleString;

  needsInit = true;

  solvers = NULL; 
  numData = 0;
}

// destructor
RtIncrementalGLM::~RtIncrementalGLM() {
  // delete all solvers
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
bool RtIncrementalGLM::processOption(const string &name, const string &text) {
  // look for known options

  return RtActivationEstimator::processOption(name, text);
}  

// process a single acquisition
int RtIncrementalGLM::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtIncrementalGLM::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *dat = (RtMRIImage*)msg->getCurrentData();

  if(dat == NULL) {
    cout << "RtIncrementalGLM::process: data passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtIncrementalGLM:process: data passed is NULL\n"));
    return 0;
  }

//  tc.put(numTimepoints,(double) dat->getElement(16*32*32 + 28*32 + 14));
//  gp.reset_plot();
//  gp.plot_x(tc,"timeseries");
  
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
    ACE_DEBUG((LM_INFO, "RtIncrementalGLM::process: new data has wrong number of elements\n"));
    return -1;    
  }

  // allocate a new data image for the estimation
  RtActivation *est = new RtActivation(*dat);
  est->initToZeros();

  if(numTimepoints > numTrends+1) {
    est->setThreshold(getTStatThreshold(numTimepoints-numTrends-1));
    cout << "t thresh: " << est->getThreshold() << " (p=" 
	 << getProbThreshold() << ")" << endl;
  }

  //// element independent setup
  
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
      est->setPixel(i,0.0);
      continue;
    }

    double y = dat->getElement(i);
    solvers[i]->include(&y,Xrow,1.0);

    if(numTimepoints > numTrends+1) {
      double *beta = solvers[i]->regress(0);
      double *columnSEs = solvers[i]->getSquaredError(0);
//      double se = sqrt((solvers[i]->getTotalSquaredError(0)
//			/(numTimepoints-numTrends))
//		       /columnSEs[numTrends]);

      est->setPixel(i,
		    beta[numTrends]*sqrt(columnSEs[numTrends]
			 * (numTimepoints-numTrends)/(double)numTimepoints
			 / solvers[i]->getTotalSquaredError(0)));

      //cout << est->getPixel(i) << endl;
      //est->setPixel(i, beta[numTrends]/se);

      delete columnSEs;
      delete beta;
    }

  }  
//  cout << endl;

  delete Xrow;

  // set the image id for handling
  est->addToID("voxel-incrglm");

  setResult(msg,est);

  // test if this is the last measurement for mask conversion and saving
  if(numTimepoints == numMeas && saveTVol) {
    cout << "saving t vol to " << saveTVolFilename << endl;
    est->write(saveTVolFilename);
  }
  if(numTimepoints == numMeas && savePosResultAsMask) {
    RtMaskImage *activationMask = est->toMask(POS);
    activationMask->setFilename(savePosAsMaskFilename);
    activationMask->save();
  }
  if(numTimepoints == numMeas && saveNegResultAsMask) {
    RtMaskImage *activationMask = est->toMask(POS);
    activationMask->setFilename(saveNegAsMaskFilename);
    activationMask->save();
  }

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

