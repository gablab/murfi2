/******************************************************************************
 * RtIncrementalGLM.h is the implementation of a class that computes 
 * activation at each voxel incrementally using Gentleman's method
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-04-01
 *
 *****************************************************************************/

#include"RtIncrementalGLM.h"
#include"RtMRIImage.h"

string RtIncrementalGLM::moduleString(ID_INCREMENTALGLM);

// default constructor
RtIncrementalGLM::RtIncrementalGLM() : RtModelFit(), 
				       numSolvers(0),
				       solvers (NULL) {
  componentID = moduleString;
}

// destructor
RtIncrementalGLM::~RtIncrementalGLM() {
  // delete all solvers
  if(solvers != NULL) {
    for(unsigned int i = 0; i < numSolvers; i++) {
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
bool RtIncrementalGLM::processOption(const string &name, const string &text,
				     const map<string,string> &attrMap) {
  // look for known options

  return RtModelFit::processOption(name, text, attrMap);
}  

// validate the configuration
bool RtIncrementalGLM::validateComponentConfig() {
  bool result = true;
  
  return RtModelFit::validateComponentConfig() && result;
}


// initialize the estimation algorithm for a particular image size
// in
//  first acquired image to use as a template for parameter inits
void RtIncrementalGLM::initEstimation(const RtData &dat, 
				      RtMaskImage *mask) {

  if(mask) { // set the number of solvers (one per voxel)
    numSolvers = mask->getNumberOfOnVoxels();
  }
  else {
    numSolvers = dat.getNumEl();
  }

  // allocate solvers
  solvers = new RtLeastSquaresSolve*[numSolvers];
  for(unsigned int i = 0; i < numSolvers; i++) {
    solvers[i] = new RtLeastSquaresSolve(design.getNumColumns());
  }

  RtModelFit::initEstimation(dat, mask);
}


// process a single acquisition
int RtIncrementalGLM::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtIncrementalGLM::process"));

  timer tim;
  if(printTiming) {
    tim.start();
  }

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *dat = (RtMRIImage*)msg->getCurrentData();

  if(dat == NULL) {
    cerr << "RtIncrementalGLM::process: data passed is NULL" << endl;

    if(logOutput) {
      stringstream logs("");
      logs << "RtIncrementalGLM::process: data passed is NULL" << endl;
      log(logs);
    }

    return 0;
  }

  RtMaskImage *mask = getMaskFromMessage(*msg);

  if(mask == NULL) {
    cerr << "RtIncrementalGLM::process: mask is NULL" << endl;

    if(logOutput) {
      stringstream logs("");
      logs << "RtIncrementalGLM::process: mask is NULL at tr " 
	   << dat->getDataID().getTimePoint() << endl;
      log(logs);
    }

    return 0;
  }

  design.updateAtTr(dat->getDataID().getTimePoint()-1);

  // initialize the computation if necessary
  if(needsInit) {
    // initialize parent and solvers
    initEstimation(*dat, mask);
  }

  // allocate a data images for the betas and residuals
  RtActivation **betas = new RtActivation*[design.getNumColumns()];
  for(unsigned int b = 0; b < design.getNumColumns(); b++) {
    betas[b] = new RtActivation(*dat);

    // setup the data id
    betas[b]->getDataID().setFromInputData(*dat,*this);
    betas[b]->getDataID().setDataName(string(NAME_BETA)
				      + "_" + 
				      design.getColumnName(b));
    betas[b]->getDataID().setRoiID(mask->getDataID().getRoiID());
    betas[b]->initToNans();
  }

  RtActivation *residual = new RtActivation(*dat);
  // setup the data id
  residual->getDataID().setFromInputData(*dat,*this);
  residual->getDataID().setDataName(NAME_RESIDUAL_MSE);
  residual->getDataID().setRoiID(mask->getDataID().getRoiID());  
  residual->initToNans();


  //// element independent setup
  
  // get this design matrix row
  vnl_vector<double> row = design.getRow(dat->getDataID().getTimePoint()-1);

  //// include this timepoint for each voxel
  vector<unsigned int> inds = mask->getOnVoxelIndices();
  unsigned int curSolver = 0;
  for(vector<unsigned int>::iterator it = inds.begin(); it != inds.end(); 
      it++, curSolver++) {

    double y = dat->getElement(*it);
// debug
//    if(curSolver == 0) {
//      cout << "pix: " << dat->getPixel(*it) << endl;
//    }

    solvers[curSolver]->include(&y,row.data_block(),1.0);

    // provide betas only after the number of trends has been passed
    if(dat->getDataID().getTimePoint() > design.getMaxTrendOrder()) {
      double *beta = solvers[curSolver]->regress(0);

      // save beta and res
      for(unsigned int j = 0; j < design.getNumColumns(); j++) {
	betas[j]->setPixel(*it,beta[j]);
      }
      residual->setPixel(*it, solvers[curSolver]->getTotalSquaredError(0));

      if(dumpAlgoVars && dat->getDataID().getTimePoint() > 2) {
	dumpFile 
	  << dat->getDataID().getTimePoint() << " " 
	  << *it << " " 
	  << y << " "
	  << row[0] << " "
	  << residual->getPixel(*it) << " ";
	for(unsigned int b = 0; b < design.getNumColumns(); b++) {
	  dumpFile << beta[b] << " ";
	}
	dumpFile << endl;
      }
	  
      
      delete beta;
    }
    
  }  

  // pass our results out
  for(unsigned int j = 0; j < design.getNumColumns(); j++) {
    setResult(msg,betas[j]);
  }
  setResult(msg,residual);

  if(printTiming) {
    tim.stop();
    cout << "RtIncrementalGLM process at tr " 
	 << dat->getDataID().getTimePoint()
	 << " elapsed time: " << tim.elapsed_time()*1000 << "ms"  << endl;
  }

  if(print) {
    cout << "RtIncrementalGLM: done at tr " 
	 << dat->getDataID().getTimePoint() << endl;
  }

  if(logOutput) {
    stringstream logs("");
    logs << "RtIncrementalGLM: done at tr " 
	 << dat->getDataID().getTimePoint() << endl;
    log(logs);
  }

  return 0;
}

// start a logfile 
void RtIncrementalGLM::startDumpAlgoVarsFile() {
  dumpFile << "started at ";
  printNow(dumpFile);
  dumpFile << endl
	   << "time_point "
	   << "voxel_index "
	   << "voxel_intensity "
	   << "regressor "
	   << "residual ";
  for(unsigned int b = 0; b < design.getNumColumns(); b++) {
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

