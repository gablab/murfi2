/******************************************************************************
 * RtActivationSum.cpp computes the sum over all voxels in an roi map
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include"RtActivationSum.h"

string RtActivationSum::moduleString("activation-sum");

// default constructor
RtActivationSum::RtActivationSum() : RtActivationEstimator() {
  componentID = moduleString;

  activationID = "data.image.activation.voxel-singleimcor-stat_0";
  activationRoiID = "unset";
}

// destructor
RtActivationSum::~RtActivationSum() {}

// process an option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtActivationSum::processOption(const string &name, const string &text,
				    const map<string,string> &attrMap) {
  if(name == "activationID") {
    activationID = text;
    return true;
  }
  else if(name == "activationRoiID") {
    activationRoiID = text;
    return true;
  }

  return RtActivationEstimator::processOption(name, text, attrMap);
}  

// process a single acquisition
int RtActivationSum::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtActivationSum::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  RtMRIImage *img = (RtMRIImage*)msg->getCurrentData();
  int acqNum = img->getAcquisitionNum();

  // find the activation with the right roiID
  RtActivation *act 
    = (RtActivation*) msg->getDataByIDAndRoiID(activationID,activationRoiID);

  if(act == NULL) {
    cout << "couldn't find " << activationID << ":" << activationRoiID << endl;

    ACE_DEBUG((LM_INFO, "RtActivationSum:process: activation passed is NULL\n"));
    return 0;
  }
  
  ACE_DEBUG((LM_DEBUG, "summing activation in image %d\n", 
	     img->getAcquisitionNum()));
	    
  // compute the sum
  double sum = 0;
  unsigned int numPix = 0;
  for(unsigned int i = 0; i < act->getNumPix(); i++) {
    double pix = act->getPixel(i);
    if(!isnan(pix)) {
      sum += pix;
      numPix++;
    }
  }

  // create a one element activation image
  RtActivation *mean = new RtActivation(1);
  mean->setThreshold(act->getThreshold());
  mean->setPixel(0, sum/numPix);
  
  // set the image id for handling
  mean->addToID("activation-sum");
  mean->setRoiID(roiID);

  setResult(msg, mean);

  // log the sum
  stringstream logs("");
  logs << "activation sum: " << acqNum << " " 
       << activationID << ":" << roiID  
       << " " << mean->getPixel(0) << endl;
  log(logs);

  if(ofile.is_open()) {
    ofile << acqNum << " " << mean->getPixel(0) << endl;
    ofile.flush();
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


