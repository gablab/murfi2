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
  id = moduleString;

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

  // find the activation with the right roiID
  RtActivation *act 
    = (RtActivation*) msg->getDataByIDAndRoiID(activationID,roiID);

  if(act == NULL) {
    cout << "couldn't find " << roiID << endl;

    ACE_DEBUG((LM_INFO, "RtActivationSum:process: activation passed is NULL\n"));
    return 0;
  }
  
  ACE_DEBUG((LM_DEBUG, "summing activation in image %d\n", 
	     img->getAcquisitionNum()));
	    
  // create a one element activation image
  RtActivation *sum = new RtActivation(1);
  sum->setThreshold(act->getThreshold());

  // compute the absolute difference
  unsigned int numPix = 0;
  for(unsigned int i = 0; i < act->getNumPix(); i++) {
    double pix = act->getPixel(i);
    if(!isnan(pix)) {
      sum->setPixel(0, sum->getPixel(0) + pix);
      numPix++;
    }
  }
  sum->setPixel(0, sum->getPixel(0)/numPix);
  
  
  if(fabs(sum->getPixel(0)) > 10000 | isnan(sum->getPixel(0))) {
    cout << "BIG SUM FOUND: " << sum->getPixel(0) << endl;
  }


  // set the image id for handling
  sum->addToID("activation-sum");
  sum->setRoiID(act->getRoiID());

  setResult(msg, sum);

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


