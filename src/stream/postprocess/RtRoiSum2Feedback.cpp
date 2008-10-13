/******************************************************************************
 * RtRoiSum2Feedback.cpp computes the sum over all voxels in an roi map
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include "RtRoiSum2Feedback.h"

string RtRoiSum2Feedback::moduleString(ID_ROISUM2FEEDBACK);

// default constructor
RtRoiSum2Feedback::RtRoiSum2Feedback() : RtRoi2Feedback() {
  componentID = moduleString;
  dataName = NAME_ROISUM;

  activationModuleID = ID_SINGLEIMCOR;
  activationDataName = string(NAME_SINGLEIMCOR_ACTIVATION) + "_0";
  activationRoiID = "unset";
}

// destructor
RtRoiSum2Feedback::~RtRoiSum2Feedback() {}

// process a single acquisition
int RtRoiSum2Feedback::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtRoiSum2Feedback::process"));

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

    ACE_DEBUG((LM_INFO, "RtRoiSum2Feedback:process: activation passed is NULL\n"));
    return 0;
  }

  // check that the mri image and activation are the same size
  if(act->getNumEl() != img->getNumEl()) {
    cout << "activation and image are different sizes! skipping." << endl;
    return 0;
  }
  
  ACE_DEBUG((LM_DEBUG, "summing activation in image %d\n", 
	     img->getDataID()->getTimepoint()));
	    
  if(needsInit) {
    initEstimation(*img);
  }
	    
  // compute the sum
  double sumVox = 0;
  unsigned int numPix = 0;
  for(unsigned int i = 0; i < act->getNumPix(); i++) {
    if(!mask.getPixel(i)) {
      continue;
    }

    double pix = act->getPixel(i);
    if(!isnan(pix)) {
      sumVox += pix;
      numPix++;
    }
  }

  // create a one element activation image
  RtActivation *sum = new RtActivation(1);

  // setup the data id
  sum->getDataID().setFromInputData(*act,*this);
  sum->getDataID().setDataName(dataName);
  sum->getDataID().setRoiID(roiID);

  sum->setThreshold(act->getThreshold());
  sum->setPixel(0, sumVox);
  
  // set the image id for handling
  //sum->addToID("activation-sum");
  //sum->setRoiID(roiID);

  setResult(msg, sum);

  // log the sum
  stringstream logs("");
  logs << "activation sum: " << acqNum << " " 
       << activationModuleID << ":" << activationDataName << ":" << roiID  
       << " " << sum->getPixel(0) << endl;
  log(logs);

  if(ofile.is_open()) {
    ofile << acqNum << " " << sum->getPixel(0) << endl;
    ofile.flush();
  }

  if(DEBUG_LEVEL & BASIC) {
    cout << "activation sum: " << acqNum << " " << sum->getPixel(0) << endl;
    cout.flush();
  }

//  cout << "done processing activation sum at ";
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


