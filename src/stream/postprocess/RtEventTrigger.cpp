/******************************************************************************
 * RtEventTrigger.cpp triggers an event based on activation sums
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-04-14
 *
 *****************************************************************************/

#include"RtEventTrigger.h"
#include"RtDataIDs.h"
#include"RtActivation.h"

#include"RtEvent.h"

string RtEventTrigger::moduleString(ID_EVENTTRIGGER);

// default constructor
RtEventTrigger::RtEventTrigger() : RtStreamComponent() {
  componentID = moduleString;

  posRoiID = "active";
  negRoiID = "deactive";
  posThresh = 3.0;
  negThresh = 3.0;

  afterTriggerSkipTRs = 0;
  initialSkipTRs = 0;
  triggerBothDirections = true;
}

// destructor
RtEventTrigger::~RtEventTrigger() {}

// process an option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtEventTrigger::processOption(const string &name, const string &text,
				   const map<string,string> &attrMap) {
  if(name == "posRoiID") {
    posRoiID = text;
    return true;
  }
  if(name == "negRoiID") {
    negRoiID = text;
    return true;
  }
  if(name == "monitorPos") {
    return RtConfigVal::convert<bool>(monitorPos,text);
  }
  if(name == "monitorNeg") {
    return RtConfigVal::convert<bool>(monitorNeg,text);
  }  
  if(name == "posThresh") {
    return RtConfigVal::convert<double>(posThresh,text);
  }
  if(name == "negThresh") {
    return RtConfigVal::convert<double>(negThresh,text);
  }
  if(name == "afterTriggerSkipTRs") {
    return RtConfigVal::convert<int>(afterTriggerSkipTRs,text);
  }
  if(name == "initialSkipTRs") {
    return RtConfigVal::convert<int>(initialSkipTRs,text);
  }
  return RtStreamComponent::processOption(name, text, attrMap);
}  

// validate the configuration
bool RtEventTrigger::validateComponentConfig() {
  bool result = true;

  return  result;
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


