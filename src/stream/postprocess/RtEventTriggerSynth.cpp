/*=========================================================================
 *  RtEventTriggerSynth.cpp synthesizes event triggers
 *
 *  Copyright 2007-2013, the MURFI dev team.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

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

// validate the configuration
bool RtEventTriggerSynth::validateComponentConfig() {
  bool result = true;

  return RtEventTrigger::validateComponentConfig() && result;
}

// process a single acquisition
int RtEventTriggerSynth::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtEventTriggerSynth::process"));

  if(disabled) {
    return 0;
  }

  static boost::normal_distribution<float> rnd(meanISI, sdISI);
  static boost::lagged_fibonacci19937 engine;

  static float nextTrigger
      = rnd.operator()<boost::lagged_fibonacci19937>((engine));
  static int tr = 0;
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
  }
  else {
    event->getDataID().setDataName(NAME_EVENTTRIGGER_BAD);

    // log the trigger
    stringstream logs("");
    logs << "trigger event bad at tr " << tr << endl;
    log(logs);
  }

  setResult(msg,event);

  nextTrigger = rnd.operator()<boost::lagged_fibonacci19937>((engine));

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
