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
  componentID = moduleString;
  posActivationSumID = "data.image.activation.activation-sum";
  negActivationSumID = "data.image.activation.activation-sum";
  posRoiID = "active";
  negRoiID = "deactive";
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
  else if(name == "posRoiID") {
    posRoiID = text;
    return true;
  }
  else if(name == "negRoiID") {
    negRoiID = text;
    return true;
  }

  return RtStreamComponent::processOption(name,text);
}  

// process a single acquisition
int RtActivationSumDifference::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtActivationSumDifference::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  cout << "PROCESSING " << endl;

  // find the data with the right data IDs
  RtActivation *posact 
    = (RtActivation*) msg->getDataByIDAndRoiID(posActivationSumID,posRoiID);
  RtActivation *negact 
    = (RtActivation*) msg->getDataByIDAndRoiID(negActivationSumID,negRoiID);

  if(posact == NULL || negact == NULL) {
    cout << "activation-sum-difference: couldn't find required rois" << endl;
    ACE_DEBUG((LM_INFO, "RtActivationSumDifference::process: pos or neg activation passed is NULL\n"));
    return 0;
  }
  
  ACE_DEBUG((LM_DEBUG, "differencing  activations for image %d\n", 
	     img->getAcquisitionNum()));
	    
  // create a one element activation image
  RtActivation *diff = new RtActivation(1);

  // compute the difference
  diff->setPixel(0, posact->getPixel(0) - negact->getPixel(0));

  cout << diff->getPixel(0) << " = " << posact->getPixel(0) << " - " 
       << negact->getPixel(0) << endl;

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


