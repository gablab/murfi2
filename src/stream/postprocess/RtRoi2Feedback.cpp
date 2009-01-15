/******************************************************************************
 * RtRoi2Feedback.cpp is a base class for methods of transforming roi
 * activations into feedback
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#include "RtRoi2Feedback.h"

string RtRoi2Feedback::moduleString(ID_ROI2FEEDBACK);

// default constructor
RtRoi2Feedback::RtRoi2Feedback() : RtActivationEstimator() {
  componentID = moduleString;

  activationModuleID = ID_SINGLEIMCOR;
  activationDataName = string(NAME_SINGLEIMCOR_ACTIVATION);
  activationRoiID = "unset";
}

// destructor
RtRoi2Feedback::~RtRoi2Feedback() {}

// process an option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtRoi2Feedback::processOption(const string &name, const string &text,
				    const map<string,string> &attrMap) {
  if(name == "activationDataName") {
    activationDataName = text;
    return true;
  }
  if(name == "activationModuleID") {
    activationModuleID = text;
    return true;
  }
  else if(name == "activationRoiID") {
    activationRoiID = text;
    return true;
  }

  return RtActivationEstimator::processOption(name, text, attrMap);
}  


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


