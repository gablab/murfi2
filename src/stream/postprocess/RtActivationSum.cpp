/******************************************************************************
 * RtActivationSum.cpp computes the sum over all voxels in an roi map
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include"RtActivationSum.h"

string RtActivationSum::moduleString("activation-sum");

// default constructor
RtActivationSum::RtActivationSum() : RtStreamComponent() {
  componentID = moduleString;

  activationID = "data.image.activation.voxel-singleimcor";
}

// destructor
RtActivationSum::~RtActivationSum() {}

// process an option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtActivationSum::processOption(const string &name, const string &text) {
  if(name == "roiID") {
    roiID = text;
    return true;
  }
  if(name == "activationID") {
    activationID = text;
    return true;
  }

  return RtStreamComponent::processOption(name, text);
}  

// process a single acquisition
int RtActivationSum::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtActivationSum::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  RtMRIImage *img = (RtMRIImage*)msg->getCurrentData();
  int acqNum = img->getAcquisitionNum();

  // find the activation with the right roiID
  RtActivation *act 
    = (RtActivation*) msg->getDataByIDAndRoiID(activationID,roiID);

  if(act == NULL) {
    cout << "couldn't find " << activationID << ":" << roiID << endl;

    ACE_DEBUG((LM_INFO, "RtActivationSum:process: activation passed is NULL\n"));
    return 0;
  }
  
  ACE_DEBUG((LM_DEBUG, "summing activation in image %d\n", 
	     img->getAcquisitionNum()));
	    
  // compute the absolute difference
  double sum = 0;
  unsigned int numPix = 0;
  for(unsigned int i = 0; i < act->getNumPix(); i++) {
    double pix = act->getPixel(i);
    if(!_isnan(pix)) {
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
  mean->setRoiID(act->getRoiID());

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

  cout << "done processing activation sum at ";
  printNow(cout);
  cout << endl;

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


