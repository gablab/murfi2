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
  dataIDForSum = "data.image.activation";
}

// destructor
RtActivationSum::~RtActivationSum() {}

// process an option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtActivationSum::processOption(const string &name, const string &text) {

  // look for options
  if(name == "dataIDForSum") {
    dataIDForSum = text;
    return true;
  }

  return false;
}  

// process a single acquisition
int RtActivationSum::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtActivationSum::process"));


  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // find the data with the right data ID
  RtActivation *act = (RtActivation*)msg->getDataByID(dataIDForSum);

  if(act == NULL) {
    ACE_DEBUG((LM_INFO, "RtActivationSum:process: activation passed is NULL\n"));
    return 0;
  }
  
  ACE_DEBUG((LM_DEBUG, "summing activation in image %d\n", 
	     img->getAcquisitionNum()));
	    
  // create a one element activation image
  RtActivation *sum = new RtActivation(1);

  // compute the absolute difference
  for(unsigned int i = 0; i < act->getNumPix(); i++) {
    sum->setPixel(0, sum->getPixel(0) + act->getPixel(i));
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


