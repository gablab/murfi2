/******************************************************************************
 * RtActivationSum.h is the header for a class that computes the difference
 * between two images
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include"RtActivationSum.h"

string RtActivationSum::moduleString("activation-sum");

// default constructor
RtActivationSum::RtActivationSum() : RtStreamComponent() {
  id = moduleString;
}

// destructor
RtActivationSum::~RtActivationSum() {}

// process a single acquisition
int RtActivationSum::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtActivationSum::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  RtMRIImage *img = (RtMRIImage*)msg->getCurrentData();

  if(img == NULL) {
    ACE_DEBUG((LM_INFO, "RtActivationSum:process: image passed is NULL\n"));
    return 0;
  }
  
  ACE_DEBUG((LM_DEBUG, "summing activation in image %d\n", 
	     img->getAcquisitionNum()));
	    
  // create a one element activation image
  RtActivation *sum = new RtActivation(1);

  // compute the absolute difference
  for(unsigned int i = 0; i < img->getNumPix(); i++) {
    sum->setPixel(0, sum->getPixel(0) + img->getPixel(i));
  }  

  // set the image id for handling
  sum->addToID("activation-sum");

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


