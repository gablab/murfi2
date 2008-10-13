/******************************************************************************
 * RtRoiDifference.cpp computes the difference between two
 * activaton sums
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-02-25
 *
 *****************************************************************************/

#include"RtRoiDifference.h"
#include"RtRoi2Feedback.h"

string RtRoiDifference::moduleString(ID_ROIDIFFERENCE);

// default constructor
RtRoiDifference::RtRoiDifference() : RtStreamComponent() {
  componentID = moduleString;
  dataName = NAME_ROIDIFFERENCE;

  posRoiModuleID = ID_ROIMEAN2FEEDBACK;
  negRoiModuleID = ID_ROIMEAN2FEEDBACK;
  posRoiDataName = NAME_ROIMEAN;
  negRoiDataName = NAME_ROIMEAN;
  posRoiRoiID = "active";
  negRoiRoiID = "deactive";
}

// destructor
RtRoiDifference::~RtRoiDifference() {}

// process an option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtRoiDifference::processOption(const string &name, 
					  const string &text,
					  const map<string,string> &attrMap) {

  // look for options
  if(name == "posRoiModuleID") {
    posRoiModuleID = text;
    return true;
  }
  else if(name == "negRoiModuleID") {
    negRoiModuleID = text;
    return true;
  }
  else if(name == "posRoiDataName") {
    posRoiDataName = text;
    return true;
  }
  else if(name == "negRoiDataName") {
    negRoiDataName = text;
    return true;
  }
  else if(name == "posRoiRoiID") {
    posRoiRoiID = text;
    return true;
  }
  else if(name == "negRoiRoiID") {
    negRoiRoiID = text;
    return true;
  }

  return RtStreamComponent::processOption(name,text,attrMap);
}  

// process a single acquisition
int RtRoiDifference::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtRoiDifference::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  cout << "PROCESSING " << endl;

  // find the data with the right data IDs
  RtActivation *posact 
    = (RtActivation*) msg->getData(posRoiModuleID,
				   posRoiDataName,
				   posRoiRoiID);
  RtActivation *negact 
    = (RtActivation*) msg->getData(negRoiModuleID,
				   negRoiDataName,
				   negRoiRoiID);

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
//     ACE_DEBUG((LM_INFO, "RtRoiDifference::process: pos or neg activation passed is NULL\n"));
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


