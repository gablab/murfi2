/******************************************************************************
 * RtActivationSum.cpp computes the sum over all voxels in an roi map
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include"RtActivationSum.h"

string RtActivationSum::moduleString("activation-sum");

#define DUMPAS 0
#ifdef DUMPAS
#include<fstream>
static ofstream dumpfile("/tmp/activation_sum_dump.txt");
#endif


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
  static int img = 0;
  
  ACE_TRACE(("RtActivationSum::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

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
    if(!isnan(pix)) {
      sum += pix;
      numPix++;

#ifdef DUMPAS
      dumpfile << img << " " << i << numPix << " " << " " << pix
	       << " " << sum << endl;
      dumpfile.flush();
#endif
      
    }
  }

  // create a one element activation image
  RtActivation *mean = new RtActivation(1);
  mean->setThreshold(act->getThreshold());
  mean->setPixel(0, sum/numPix);
  
//  
//  if(fabs(mean->getPixel(0)) > 100 | isnan(mean->getPixel(0))) {
//    cout << "BIG SUM FOUND: " << mean->getPixel(0) << endl;
//    mean->setPixel(0, 0);
//  }
//

  // set the image id for handling
  mean->addToID("activation-sum");
  mean->setRoiID(act->getRoiID());

  setResult(msg, mean);

  img++;

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


