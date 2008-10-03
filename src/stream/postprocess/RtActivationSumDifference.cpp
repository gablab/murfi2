/******************************************************************************
 * RtActivationSumDifference.cpp computes the difference between two
 * activaton sums
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-02-25
 *
 *****************************************************************************/

#include"RtActivationSumDifference.h"
#include"RtActivationSum.h"

string RtActivationSumDifference::moduleString(ID_ACTIVATIONSUMDIFFERENCE);

// default constructor
RtActivationSumDifference::RtActivationSumDifference() : RtStreamComponent() {
  componentID = moduleString;
  dataName = NAME_ACTIVATIONSUMDIFFERENCE;

  posActivationSumModuleID = ID_ACTIVATIONSUM;
  negActivationSumModuleID = ID_ACTIVATIONSUM;
  posActivationSumDataName = NAME_ACTIVATIONSUM;
  negActivationSumDataName = NAME_ACTIVATIONSUM;
  posRoiID = "active";
  negRoiID = "deactive";
}

// destructor
RtActivationSumDifference::~RtActivationSumDifference() {}

// process an option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtActivationSumDifference::processOption(const string &name, 
					  const string &text,
					  const map<string,string> &attrMap) {

  // look for options
  if(name == "posActivationModuleID") {
    posActivationSumModuleID = text;
    return true;
  }
  else if(name == "negActivationModuleID") {
    negActivationSumModuleID = text;
    return true;
  }
  else if(name == "posActivationDataName") {
    posActivationSumDataName = text;
    return true;
  }
  else if(name == "negActivationDataName") {
    negActivationSumDataName = text;
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

  return RtStreamComponent::processOption(name,text,attrMap);
}  

// process a single acquisition
int RtActivationSumDifference::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtActivationSumDifference::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  cout << "PROCESSING " << endl;

  // find the data with the right data IDs
  RtActivation *posact 
    = (RtActivation*) msg->getData(posActivationSumModuleID,
				   posActivationSumDataName,
				   posRoiID);
  RtActivation *negact 
    = (RtActivation*) msg->getData(negActivationSumModuleID,
				   negActivationSumDataName,
				   negRoiID);

  double p = 0;
  if(posact != NULL) {
    p = posact->getPixel(0);
  }

  double n = 0;
  if(negact != NULL) {
    n = negact->getPixel(0);
  }

 //  if(posact == NULL || negact == NULL) {
//     cout << "activation-sum-difference: couldn't find required rois" << endl;
//     ACE_DEBUG((LM_INFO, "RtActivationSumDifference::process: pos or neg activation passed is NULL\n"));
//     return 0;
//   }
  
//  ACE_DEBUG((LM_DEBUG, "differencing  activations for image %d\n", 
//	     img->getAcquisitionNum()));
	    
  // create a one element activation image
  RtActivation *diff = new RtActivation(1);

  // setup the data id
  diff->getDataID().setFromInputData(*posact,*this);
  diff->getDataID().setDataName(dataName);
  diff->getDataID().setRoiID(roiID);
  

  // compute the difference
  diff->setPixel(0,  p - n);

  cout << diff->getPixel(0) << " = " << p << " - " << n << endl;

  // set the image id for handling
  //diff->addToID("activation-sum-difference");

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


