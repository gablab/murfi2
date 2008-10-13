/******************************************************************************
 * RtEventTriggerActivationSums.cpp triggers an event based on activation sums
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-04-14
 *
 *****************************************************************************/

#include"RtEventTriggerActivationSums.h"
#include"RtActivation.h"
#include"RtDataIDs.h"

#include"RtEvent.h"

string RtEventTriggerActivationSums::moduleString(ID_EVENTTRIGGER_ACTIVATIONSUMS);

// default constructor
RtEventTriggerActivationSums::RtEventTriggerActivationSums() : RtEventTrigger() {
  componentID = moduleString;
  dataName = "";
}

// destructor
RtEventTriggerActivationSums::~RtEventTriggerActivationSums() {}

// process an option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtEventTriggerActivationSums::processOption(const string &name, 
					  const string &text,
					  const map<string,string> &attrMap) {
  return RtStreamComponent::processOption(name, text, attrMap);
}  

// process a single acquisition
int RtEventTriggerActivationSums::process(ACE_Message_Block *mb) {  
  ACE_TRACE(("RtEventTriggerActivationSums::process"));

  static int trsSinceTrigger = afterTriggerSkipTRs;
  
  cout << "event trigger dump: " 
    << monitorPos << " "
    << monitorNeg << " "
    << posThresh << " "
    << negThresh << " "
    << posroiID << " "
    << negroiID << endl;

  if(!monitorPos && !monitorNeg) {
    return 0;
  }

  // check if we need to skip triggers
  if(trsSinceTrigger < afterTriggerSkipTRs) {
    trsSinceTrigger++;
    return 0;
  }

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // find the positive activation sum with the right roiID
  RtActivation *posact 
    = (RtActivation*) msg->getData(posActivationSumModuleID,
				   posActivationSumDataName,
				   posRoiID);
  RtActivation *negact 
    = (RtActivation*) msg->getData(negActivationSumModuleID,
				   negActivationSumDataName,
				   negRoiID);

  if(monitorPos && posact == NULL) {
    cout << "couldn't find positive roi " << posroiID << endl;

    ACE_DEBUG((LM_INFO, "RtEventTriggerActivationSums:process: no positive ROI found\n"));
    return 0;
  }
  if(monitorNeg && negact == NULL) {
    cout << "couldn't find negative roi " << negroiID << endl;

    ACE_DEBUG((LM_INFO, "RtEventTriggerActivationSums:process: no negative ROI found\n"));
    return 0;
  }

  int tr = posact->getDataID().getTimePoint();
  if(tr < initialSkipTRs) {
    return 0;
  }

  cout << "checking for even trigger: " 
       << posact->getPixel(0) << " >= " << posThresh 
       << " and "
       << negact->getPixel(0) << " <= " << negThresh 
       << endl;

  // check for good trigger
  if((!monitorPos || posact->getPixel(0) >= posThresh) 
     && (!monitorNeg || negact->getPixel(0) <= negThresh)) {
    // trigger
//    RtEvent *event = new RtEvent();
//    event->setTR(tr);
//    event->addToID("trigger.good");

    RtEvent *event = new RtEvent();
   
    event->getDataID().setFromInputData(*posact,*this);
    event->getDataID().setDataName(NAME_EVENTTRIGGER_GOOD);
    event->getDataID().setTimePoint(tr);

    setResult(msg,event);

    cout << "GOOD EVENT TRIGGERED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << posact->getPixel(0) << " " << posThresh << endl;

    trsSinceTrigger = 0;
  }
  
  // check for bad trigger
  if(triggerBothDirections 
     && (!monitorPos || posact->getPixel(0) <= -posThresh) 
     && (!monitorNeg || negact->getPixel(0) >= -negThresh)) {

    // trigger
//    RtEvent *event = new RtEvent();
//    event->setTR(tr);
//    event->addToID("trigger.bad");

    RtEvent *event = new RtEvent();
   
    event->getDataID().setFromInputData(*posact,*this);
    event->getDataID().setDataName(NAME_EVENTTRIGGER_BAD);
    event->getDataID().setTimePoint(tr);

    setResult(msg,event);

    cout << "BAD EVENT TRIGGERED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << posact->getPixel(0) << " " << posThresh << endl;

    trsSinceTrigger = 0;
  }
  
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


