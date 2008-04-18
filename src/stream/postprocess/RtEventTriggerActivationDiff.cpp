/******************************************************************************
 * RtEventTriggerActivationSumDiff.cpp triggers an event based on activation sums
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-04-14
 *
 *****************************************************************************/

#include"RtEventTriggerActivationDiff.h"
#include"RtActivation.h"

#include"RtEvent.h"

string RtEventTriggerActivationDiff::moduleString("event-trigger-activation-diff");

// default constructor
RtEventTriggerActivationDiff::RtEventTriggerActivationDiff() : RtEventTrigger() {
  componentID = moduleString;

  diffThresh = 1.0;
}

// destructor
RtEventTriggerActivationDiff::~RtEventTriggerActivationDiff() {}

// process an option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtEventTriggerActivationDiff::processOption(const string &name, const string &text) {
  if(name == "diffThresh") {
    return RtConfigVal::convert<double>(diffThresh,text);
  }

  return RtEventTrigger::processOption(name, text);
}  

// process a single acquisition
int RtEventTriggerActivationDiff::process(ACE_Message_Block *mb) {  
  ACE_TRACE(("RtEventTriggerActivationDiff::process"));

  static int trsSinceTrigger = afterTriggerSkipTRs;
  
  // check if we need to skip triggers
  if(trsSinceTrigger < afterTriggerSkipTRs) {
    trsSinceTrigger++;
    return 0;
  }

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // find the positive activation sum with the right roiID
  RtActivation *posact 
    = (RtActivation*) msg->getDataByIDAndRoiID("data.image.activation.activation-sum",
						  posroiID);
  if(posact == NULL) {
    cout << "couldn't find positive roi " << posroiID << endl;

    ACE_DEBUG((LM_INFO, "RtEventTriggerActivationDiff:process: no positive ROI found\n"));
    return 0;
  }
  
  // find the positive activation sum with the right roiID
  RtActivation *negact 
    = (RtActivation*) msg->getDataByIDAndRoiID("data.image.activation.activation-sum",
						  negroiID);
  if(negact == NULL) {
    cout << "couldn't find negative roi " << negroiID << endl;

    ACE_DEBUG((LM_INFO, "RtEventTriggerActivationDiff:process: no negative ROI found\n"));
    return 0;
  }

  int tr = ((RtMRIImage*)msg->getCurrentData())->getAcquisitionNum();
  if(tr < initialSkipTRs) {
    return 0;
  }

  cout << "checking for even trigger: " 
       << posact->getPixel(0) << " - " 
       <<  negact->getPixel(0) << " >= " 
       << diffThresh 
       << endl;

  // check for good trigger
  if(posact->getPixel(0) - negact->getPixel(0) >= diffThresh) {
    // trigger
    RtEvent *event = new RtEvent();
    event->setTR(tr);
    event->addToID("trigger.good");
    setResult(msg,event);

    cout << "GOOD EVENT TRIGGERED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << posact->getPixel(0) - negact->getPixel(0) << " " 
	 << diffThresh << endl;

    trsSinceTrigger = 0;
  }
  
  // check for bad trigger
  if(posact->getPixel(0) - negact->getPixel(0) <= -diffThresh) {
    // trigger
    RtEvent *event = new RtEvent();
    event->addToID("trigger.bad");
    event->setTR(tr);
    setResult(msg,event);

    cout << "BAD EVENT TRIGGERED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << posact->getPixel(0) - negact->getPixel(0) 
	 << " " << diffThresh << endl;

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


