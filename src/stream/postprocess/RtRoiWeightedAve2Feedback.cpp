/******************************************************************************
 * RtRoiWeightedAve2Feedback.cpp computes the weighted average over voxels
 * in an roi map
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include "RtRoiWeightedAve2Feedback.h"
#include "RtDataIDs.h"

string RtRoiWeightedAve2Feedback::moduleString(ID_ROIWEIGHTEDAVE2FEEDBACK);

// default constructor
RtRoiWeightedAve2Feedback::RtRoiWeightedAve2Feedback() : RtRoi2Feedback() {
  componentID = moduleString;
  dataName = NAME_ROIVAL;

  activationModuleID = ID_SINGLEIMCOR;
  activationDataName = string(NAME_SINGLEIMCOR_ACTIVATION);
  activationRoiID = "unset";

  weightModuleID = ID_SINGLEIMCOR;
  weightDataName = string(NAME_SINGLEIMCOR_RESIDUAL);
}

// destructor
RtRoiWeightedAve2Feedback::~RtRoiWeightedAve2Feedback() {}

// process an option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtRoiWeightedAve2Feedback::processOption(const string &name, 
					  const string &text,
					  const map<string,string> &attrMap) {
  if(name == "weightDataName") {
    weightDataName = text;
    return true;
  }
  if(name == "weightModuleID") {
    weightModuleID = text;
    return true;
  }

  return RtRoi2Feedback::processOption(name, text, attrMap);
}  


// process a single acquisition
int RtRoiWeightedAve2Feedback::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtRoiWeightedAve2Feedback::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  RtMRIImage *img = (RtMRIImage*)msg->getCurrentData();
  int acqNum = img->getDataID().getTimePoint();

  // find the activation with the right roiID
  RtActivation *act 
    = (RtActivation*) msg->getData(activationModuleID, 
				   activationDataName,
				   activationRoiID);

  // find the weights (same roi id as the activation)
  RtActivation *weights
    = (RtActivation*) msg->getData(weightModuleID, 
				   weightDataName,
				   activationRoiID);

  if(act == NULL) {
    cout << "couldn't find " << activationModuleID << ":" << activationDataName << ":" << activationRoiID << endl;

    ACE_DEBUG((LM_INFO, "RtRoiWeightedAve2Feedback:process: activation passed is NULL\n"));
    return 0;
  }

  if(weights == NULL) {
    cout << "couldn't find " << weightModuleID << ":" << weightDataName << ":" << activationRoiID << endl;

    ACE_DEBUG((LM_INFO, "RtRoiWeightedAve2Feedback:process: weights passed are NULL\n"));
    return 0;
  }

  // check that the mri image and activation are the same size
  if(act->getNumEl() != img->getNumEl()) {
    cout << "activation and image are different sizes! skipping." << endl;
    return 0;
  }
  
//  ACE_DEBUG((LM_DEBUG, "weightedAve-ing activation in image %d\n", 
//	     img->getDataID()->getTimepoint()));
	    
  if(needsInit) {
    initEstimation(*img);
  }
	    
  // fill a vector with the values
  double weightedSum = 0;
  double summedWeights = 0;
  for(unsigned int i = 0; i < act->getNumPix(); i++) {
    if(!mask.getPixel(i)) {
      continue;
    }

    double pix = act->getPixel(i);
    double weight = weights->getPixel(i);

    if(DEBUG_LEVEL & ADVANCED) {
      cout << "roiwa: " << i << " " << pix << " " << weight << endl;
    }

    if(!isnan(pix)) {
      weightedSum += weight*pix;
      summedWeights += weight;
    }
  }

  // create a one element activation image
  RtActivation *weightedAve = new RtActivation(1);

  // setup the data id
  weightedAve->getDataID().setFromInputData(*act,*this);
  weightedAve->getDataID().setDataName(dataName);
  weightedAve->getDataID().setRoiID(roiID);

  weightedAve->setThreshold(act->getThreshold());
  weightedAve->setPixel(0, weightedSum/summedWeights);

  setResult(msg, weightedAve);

  // log the weightedAve
  stringstream logs("");
  logs << "activation weightedAve: " << acqNum << " " 
       << activationModuleID << ":" << activationDataName << ":" << roiID  
       << " " << weightedAve->getPixel(0) << endl;
  log(logs);

  if(ofile.is_open()) {
    ofile << acqNum << " " << weightedAve->getPixel(0) << endl;
    ofile.flush();
  }

  if(DEBUG_LEVEL & BASIC) {
    cout << "activation weightedAve: " << acqNum << " " << weightedAve->getPixel(0) << endl;
    cout.flush();
  }

//  cout << "done processing activation weightedAve at ";
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


