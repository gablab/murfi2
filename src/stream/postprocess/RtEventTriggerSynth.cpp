/******************************************************************************
 * RtEventTriggerActivationSumDiff.cpp triggers an event based on activation sums
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-04-14
 *
 *****************************************************************************/

#include"RtEventTriggerSynth.h"
#include"RtDataIDs.h"
#include"RtEvent.h"

#include"boost/random/normal_distribution.hpp"

string RtEventTriggerSynth::moduleString(ID_EVENTTRIGGER_SYNTH);

// default constructor
RtEventTriggerSynth::RtEventTriggerSynth() : RtEventTrigger() {
  componentID = moduleString;
  dataName = "";

  meanISI = 4;
  sdISI = 1;
}

// destructor
RtEventTriggerSynth::~RtEventTriggerSynth() {}

// process an option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtEventTriggerSynth::processOption(const string &name, 
					   const string &text,
					   const map<string,string> &attrMap) {
  if(name == "meanISI") {
    return RtConfigVal::convert<float>(meanISI,text);
  }
  else if(name == "sdISI") {
    return RtConfigVal::convert<float>(sdISI,text);
  }

  return RtEventTrigger::processOption(name, text, attrMap);
}  

// process a single acquisition
int RtEventTriggerSynth::process(ACE_Message_Block *mb) {  
  ACE_TRACE(("RtEventTriggerSynth::process"));

  static normal_distribution rnd(meanISI, sdISI);
  static int nextTrigger = rnd();
  
  //debug
//    cout << "event trigger started at ";
//    printNow(cout);
//    cout << endl;

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

  if(posact == NULL) {
    cout << "couldn't find positive roi " << posroiID << endl;

    ACE_DEBUG((LM_INFO, "RtEventTriggerSynth:process: no positive ROI found\n"));
    return 0;
  }

  if(negact == NULL) {
    cout << "couldn't find negative roi " << negroiID << endl;

    ACE_DEBUG((LM_INFO, "RtEventTriggerSynth:process: no negative ROI found\n"));
    return 0;
  }

  int tr = posact->getDataID().getTimePoint();
  if(tr < initialSkipTRs) {
    return 0;
  }

  cout << "checking for even trigger: " 
       << posact->getPixel(0) << " - " 
       <<  negact->getPixel(0) << " >= " 
       << diffThresh 
       << endl;

  // check for goodrigger
  if(posact->getPixel(0) - negact->getPixel(0) >= diffThresh) {
    // trigger
    RtEvent *event = new RtEvent();
   
    event->getDataID().setFromInputData(*posact,*this);
    event->getDataID().setDataName(NAME_EVENTTRIGGER_GOOD);
    event->getDataID().setTimePoint(tr);

//    event->setTR(tr);
//    event->addToID("trigger.good");
    setResult(msg,event);

    // log the trigger
    stringstream logs("");
    logs << "trigger event good at tr " << tr << ": " 
	 << posact->getPixel(0) - negact->getPixel(0) << " >= " 
	 << diffThresh << endl;
    log(logs);

    // debug
    cout << "GOOD EVENT TRIGGERED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << posact->getPixel(0) - negact->getPixel(0) << " " 
	 << diffThresh << endl;

    trsSinceTrigger = 0;
  }
  
  // check for bad trigger
  if(posact->getPixel(0) - negact->getPixel(0) <= -diffThresh) {
    // trigger
    RtEvent *event = new RtEvent();
   
    event->getDataID().setFromInputData(*posact,*this);
    event->getDataID().setDataName(NAME_EVENTTRIGGER_BAD);
    event->getDataID().setTimePoint(tr);

    //event->addToID("trigger.bad");
    //event->setTR(tr);
    setResult(msg,event);

    // log the trigger
    stringstream logs("");
    logs << "trigger event bad at tr " << tr << ": " 
	 << posact->getPixel(0) - negact->getPixel(0) << " <= " 
	 << -diffThresh << endl;
    log(logs);
    
    // debug
    cout << "BAD EVENT TRIGGERED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
    cout << posact->getPixel(0) - negact->getPixel(0) 
	 << " " << diffThresh << endl;

    trsSinceTrigger = 0;
  }

  //debug
//    cout << "event trigger finished at ";
//    printNow(cout);
//    cout << endl;

  
  return 0;
}

// generate an isi
int RtEventTriggerSynth::process(ACE_Message_Block *mb) {  
  ACE_TRACE(("RtEventTriggerSynth::process"));

}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


