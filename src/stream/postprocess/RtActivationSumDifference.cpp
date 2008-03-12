/******************************************************************************
 * RtActivationSumDifference.cpp computes the difference between two
 * activaton sums
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-02-25
 *
 *****************************************************************************/

#include"RtActivationSumDifference.h"
#include"RtActivationSum.h"

string RtActivationSumDifference::moduleString("activation-sum-difference");

// default constructor
RtActivationSumDifference::RtActivationSumDifference() : RtStreamComponent() {
  id = moduleString;
  posActivationSumID = "data.image.activation.pos";
  negActivationSumID = "data.image.activation.neg";
}

// destructor
RtActivationSumDifference::~RtActivationSumDifference() {}

// process an option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtActivationSumDifference::processOption(const string &name, const string &text) {

  // look for options
  if(name == "posActivationID") {
    posActivationSumID = text;
    return true;
  }
  else if(name == "negActivationID") {
    negActivationSumID = text;
    return true;
  }

  return false;
}  

// process a single acquisition
int RtActivationSumDifference::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtActivationSumDifference::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // find the data with the right data IDs
  RtActivation *posact 
    = (RtActivation*) msg->getDataByIDAndRoiID("data.image.activation.activation-sum",posActivationSumID);
  RtActivation *negact 
    = (RtActivation*) msg->getDataByIDAndRoiID("data.image.activation.activation-sum",negActivationSumID);

  if(posact == NULL || negact == NULL) {
    ACE_DEBUG((LM_INFO, "RtActivationSumDifference::process: pos or neg activation passed is NULL\n"));
    return 0;
  }
  
  ACE_DEBUG((LM_DEBUG, "differencing  activations for image %d\n", 
	     img->getAcquisitionNum()));
	    
  // create a one element activation image
  RtActivation *diff = new RtActivation(1);

  // compute the difference
  diff->setPixel(1, posact->getPixel(1) - negact->getPixel(1));

  cout << diff->getPixel(1) << " = " << posact->getPixel(1) << " - " 
       << negact->getPixel(1) << endl;

  // set the image id for handling
  diff->addToID("activation-sum-difference");

  setResult(msg, diff);

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


