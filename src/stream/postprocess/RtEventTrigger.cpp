/******************************************************************************
 * RtEventTrigger.cpp triggers an event based on activation sums
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-04-14
 *
 *****************************************************************************/

#include"RtEventTrigger.h"
#include"RtActivation.h"

#include"RtEvent.h"

string RtEventTrigger::moduleString("event-trigger");

// default constructor
RtEventTrigger::RtEventTrigger() : RtStreamComponent() {
  componentID = moduleString;

  posroiID = "active";
  negroiID = "deactive";
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
bool RtEventTrigger::processOption(const string &name, const string &text) {
  if(name == "posroiID") {
    posroiID = text;
    return true;
  }
  if(name == "negroiID") {
    negroiID = text;
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
  return RtStreamComponent::processOption(name, text);
}  

// process a single acquisition
//int RtEventTrigger::process(ACE_Message_Block *mb) {  
//  ACE_TRACE(("RtEventTrigger::process"));
//
//  static int trsSinceTrigger = afterTriggerSkipTRs;
//  
//  cout << "event trigger dump: " 
//    << monitorPos << " "
//    << monitorNeg << " "
//    << posThresh << " "
//    << negThresh << " "
//    << posroiID << " "
//    << negroiID << endl;
//
//  if(!monitorPos && !monitorNeg) {
//    return 0;
//  }
//
//  // check if we need to skip triggers
//  if(trsSinceTrigger < afterTriggerSkipTRs) {
//    trsSinceTrigger++;
//    return 0;
//  }
//
//  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();
//
//  // find the positive activation sum with the right roiID
//  RtActivation *posact 
//    = (RtActivation*) msg->getDataByIDAndRoiID("data.image.activation.activation-sum",
//						  posroiID);
//  if(monitorPos && posact == NULL) {
//    cout << "couldn't find positive roi " << posroiID << endl;
//
//    ACE_DEBUG((LM_INFO, "RtEventTrigger:process: no positive ROI found\n"));
//    return 0;
//  }
//  
//  // find the positive activation sum with the right roiID
//  RtActivation *negact 
//    = (RtActivation*) msg->getDataByIDAndRoiID("data.image.activation.activation-sum",
//						  negroiID);
//  if(monitorNeg && negact == NULL) {
//    cout << "couldn't find negative roi " << negroiID << endl;
//
//    ACE_DEBUG((LM_INFO, "RtEventTrigger:process: no negative ROI found\n"));
//    return 0;
//  }
//
//  cout << "checking for even trigger: " 
//       << posact->getPixel(0) << " >= " << posThresh 
//       << " and "
//       << negact->getPixel(0) << " <= " << negThresh 
//       << endl;
//
//  // check for good trigger
//  if((!monitorPos || posact->getPixel(0) >= posThresh) 
//     && (!monitorNeg || negact->getPixel(0) <= negThresh)) {
//    // trigger
//    RtEvent *event = new RtEvent();
//    event->addToID("trigger.good");
//    setResult(msg,event);
//
//    cout << "GOOD EVENT TRIGGERED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
//    cout << posact->getPixel(0) << " " << posThresh << endl;
//
//    trsSinceTrigger = 0;
//  }
//  
//  // check for bad trigger
//  if((!monitorPos || posact->getPixel(0) <= -posThresh) 
//     && (!monitorNeg || negact->getPixel(0) >= -negThresh)) {
//    // trigger
//    RtEvent *event = new RtEvent();
//    event->addToID("trigger.bad");
//    setResult(msg,event);
//
//    cout << "BAD EVENT TRIGGERED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
//    cout << posact->getPixel(0) << " " << posThresh << endl;
//
//    trsSinceTrigger = 0;
//  }
//  
//  return 0;
//}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


