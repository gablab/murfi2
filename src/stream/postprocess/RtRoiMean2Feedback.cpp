/******************************************************************************
 * RtRoiMean2Feedback.cpp computes the mean over all voxels in an roi map
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include "RtRoiMean2Feedback.h"

string RtRoiMean2Feedback::moduleString(ID_ROIMEAN2FEEDBACK);

// default constructor
RtRoiMean2Feedback::RtRoiMean2Feedback() : RtRoi2Feedback() {
  componentID = moduleString;
  dataName = NAME_ROIMEAN;

  activationModuleID = ID_SINGLEIMCOR;
  activationDataName = string(NAME_SINGLEIMCOR_ACTIVATION) + "_0";
  activationRoiID = "unset";
}

// destructor
RtRoiMean2Feedback::~RtRoiMean2Feedback() {}

// process a single acquisition
int RtRoiMean2Feedback::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtRoiMean2Feedback::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  RtMRIImage *img = (RtMRIImage*)msg->getCurrentData();
  int acqNum = img->getDataID().getTimePoint();

  // find the activation with the right roiID
  RtActivation *act 
    = (RtActivation*) msg->getData(activationModuleID, 
				   activationDataName,
				   activationRoiID);

  if(act == NULL) {
    cout << "couldn't find " << activationModuleID << ":" << activationDataName << ":" << activationRoiID << endl;

    ACE_DEBUG((LM_INFO, "RtRoiMean2Feedback:process: activation passed is NULL\n"));
    return 0;
  }

  // check that the mri image and activation are the same size
  if(act->getNumEl() != img->getNumEl()) {
    cout << "activation and image are different sizes! skipping." << endl;
    return 0;
  }
  
  ACE_DEBUG((LM_DEBUG, "mean-ing activation in image %d\n", 
	     img->getDataID()->getTimepoint()));

  if(needsInit) {
    initEstimation(*img);
  }
	    
  // compute the sum
  double sum = 0;
  unsigned int numPix = 0;
  for(unsigned int i = 0; i < act->getNumPix(); i++) {
    if(!mask.getPixel(i)) {
      continue;
    }

    double pix = act->getPixel(i);
    if(!isnan(pix)) {
      if(DEBUG_LEVEL & BASIC) {
	cout << pix << " ";
      }

      sum += pix;
      numPix++;
    }
  }

      if(DEBUG_LEVEL & BASIC) {
	cout << endl;
      }


  // create a one element activation image
  RtActivation *mean = new RtActivation(1);

  // setup the data id
  mean->getDataID().setFromInputData(*act,*this);
  mean->getDataID().setDataName(dataName);
  mean->getDataID().setRoiID(roiID);

  mean->setThreshold(act->getThreshold());
  mean->setPixel(0, sum/numPix);
  
  // set the image id for handling
  //mean->addToID("activation-mean");
  //mean->setRoiID(roiID);

  setResult(msg, mean);

  // log the mean
  stringstream logs("");
  logs << "activation mean: " << acqNum << " " 
       << activationModuleID << ":" << activationDataName << ":" << roiID  
       << " " << mean->getPixel(0) << endl;
  log(logs);

  if(ofile.is_open()) {
    ofile << acqNum << " " << mean->getPixel(0) << endl;
    ofile.flush();
  }

  if(DEBUG_LEVEL & BASIC) {
    cout << "activation mean: " << acqNum << " " << mean->getPixel(0) 
	 << " ( " << numPix << ") pixels"
	 << endl;
    cout.flush();
  }

//  cout << "done processing activation mean at ";
//  printNow(cout);
//  cout << endl;

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


