/******************************************************************************
 * RtEventTriggerSynth.cpp synthesizes event triggers
 *
 * Oliver Hinds <ohinds@mit.edu> 2009-01-14
 *
 *****************************************************************************/

#include"RtEventTriggerSynth.h"
#include"RtDataIDs.h"
#include"RtEvent.h"

#include"boost/random/normal_distribution.hpp"
#include"boost/random/lagged_fibonacci.hpp"

string RtEventTriggerSynth::moduleString(ID_EVENTTRIGGER_SYNTH);

// default constructor
RtEventTriggerSynth::RtEventTriggerSynth() : RtEventTrigger() {
  componentID = moduleString;

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

  if(disabled) {
    return 0;
  }

  static boost::normal_distribution<float> rnd(meanISI, sdISI);
  static boost::lagged_fibonacci19937 engine;

  static float nextTrigger = rnd.operator()<boost::lagged_fibonacci19937>((engine));
  static int tr = 0;
  
  //debug
//    cout << "event trigger started at ";
//    printNow(cout);
//    cout << endl;

  tr++;

  // check if now is not yet a trigger time
  nextTrigger--;
  if(nextTrigger > 0) {
    return 0;
  }

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // trigger
  RtEvent *event = new RtEvent();
  event->getDataID().setTimePoint(tr);
  event->getDataID().setModuleID(componentID);

  // generate whether its good or bad randomly
  if(rnd.operator()<boost::lagged_fibonacci19937>((engine)) < meanISI) {
    event->getDataID().setDataName(NAME_EVENTTRIGGER_GOOD);

    // log the trigger
    stringstream logs("");
    logs << "trigger event good at tr " << tr << endl;
    log(logs);

    // debug
    cout << "GOOD EVENT TRIGGERED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
  }
  else {
    event->getDataID().setDataName(NAME_EVENTTRIGGER_BAD);

    // log the trigger
    stringstream logs("");
    logs << "trigger event bad at tr " << tr << endl;
    log(logs);

    // debug
    cout << "BAD EVENT TRIGGERED!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!" << endl;
  }

  setResult(msg,event);

  nextTrigger = rnd.operator()<boost::lagged_fibonacci19937>((engine));

  //debug
//    cout << "event trigger finished at ";
//    printNow(cout);
//    cout << endl;

  
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


