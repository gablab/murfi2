/******************************************************************************
 * RtRoiMedian2Feedback.cpp computes the median over all voxels in an roi map
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include "RtRoiMedian2Feedback.h"
#include "RtDataIDs.h"

string RtRoiMedian2Feedback::moduleString(ID_ROIMEDIAN2FEEDBACK);

// default constructor
RtRoiMedian2Feedback::RtRoiMedian2Feedback() : RtRoi2Feedback() {
  componentID = moduleString;
  dataName = NAME_ROIVAL;

  activationModuleID = ID_SINGLEIMCOR;
  activationDataName = string(NAME_SINGLEIMCOR_ACTIVATION);
  activationRoiID = "unset";
}

// destructor
RtRoiMedian2Feedback::~RtRoiMedian2Feedback() {}

// process a single acquisition
int RtRoiMedian2Feedback::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtRoiMedian2Feedback::process"));

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

    ACE_DEBUG((LM_INFO, "RtRoiMedian2Feedback:process: activation passed is NULL\n"));
    return 0;
  }

  // check that the mri image and activation are the same size
  if(act->getNumEl() != img->getNumEl()) {
    cout << "activation and image are different sizes! skipping." << endl;
    return 0;
  }
  
//  ACE_DEBUG((LM_DEBUG, "median-ing activation in image %d\n", 
//	     img->getDataID()->getTimepoint()));
	    
  if(needsInit) {
    initEstimation(*img);
  }
	    
  // fill a vector with the values
  vector<double> roiVals;
  unsigned int numPix = 0;
  for(unsigned int i = 0; i < act->getNumPix(); i++) {
    if(!mask.getPixel(i)) {
      continue;
    }

    double pix = act->getPixel(i);
    if(!isnan(pix)) {
      roiVals.push_back(pix);
      numPix++;
    }
  }

  double medVal = 0;
  if(numPix != 0) {
    sort(roiVals.begin(), roiVals.end());
    medVal = (numPix%2 == 0)
      ? (roiVals[numPix/2] + roiVals[numPix/2-1]) / 2 : roiVals[numPix/2]; 
  }

  // create a one element activation image
  RtActivation *median = new RtActivation(1);

  // setup the data id
  median->getDataID().setFromInputData(*act,*this);
  median->getDataID().setDataName(dataName);
  median->getDataID().setRoiID(roiID);

  median->setThreshold(act->getThreshold());
  median->setPixel(0, medVal);

  setResult(msg, median);

  // log the median
  stringstream logs("");
  logs << "activation median: " << acqNum << " " 
       << activationModuleID << ":" << activationDataName << ":" << roiID  
       << " " << median->getPixel(0) << endl;
  log(logs);

  if(ofile.is_open()) {
    ofile << acqNum << " " << median->getPixel(0) << endl;
    ofile.flush();
  }

  if(DEBUG_LEVEL & BASIC) {
    cout << "activation median: " << roiID << ":" << acqNum << " " << median->getPixel(0) << endl;
    cout.flush();
  }

//  cout << "done processing activation median at ";
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


