/*=========================================================================
 *  RtEventTriggerActivationSums.cpp triggers an event based on activation sums
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

#include"RtEventTriggerActivationSums.h"
#include"RtActivation.h"
#include"RtDataIDs.h"

#include"RtEvent.h"

string RtEventTriggerActivationSums::moduleString(
    ID_EVENTTRIGGER_ACTIVATIONSUMS);

// default constructor
RtEventTriggerActivationSums::RtEventTriggerActivationSums()
  : RtEventTrigger() {
  componentID = moduleString;
}

// destructor
RtEventTriggerActivationSums::~RtEventTriggerActivationSums() {}

// process an option
//  in
//   name of the option to process
//   val  text of the option node
bool RtEventTriggerActivationSums::processOption(
    const string &name, const string &text, const map<string,string> &attrMap) {
  return RtStreamComponent::processOption(name, text, attrMap);
}

// validate the configuration
bool RtEventTriggerActivationSums::validateComponentConfig() {
  bool result = true;

  return result;
}

// process a single acquisition
int RtEventTriggerActivationSums::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtEventTriggerActivationSums::process"));

  static int trsSinceTrigger = afterTriggerSkipTRs;

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
    cout << "couldn't find positive roi " << posRoiID << endl;

    ACE_DEBUG(
        (LM_INFO,
         "RtEventTriggerActivationSums:process: no positive ROI found\n"));
    return 0;
  }
  if(monitorNeg && negact == NULL) {
    cout << "couldn't find negative roi " << negRoiID << endl;

    ACE_DEBUG(
        (LM_INFO,
         "RtEventTriggerActivationSums:process: no negative ROI found\n"));
    return 0;
  }

  int tr = posact->getDataID().getTimePoint();
  if(tr < initialSkipTRs) {
    return 0;
  }

  // check for good trigger
  if((!monitorPos || posact->getPixel(0) >= posThresh)
     && (!monitorNeg || negact->getPixel(0) <= negThresh)) {

    RtEvent *event = new RtEvent();

    event->getDataID().setFromInputData(*posact,*this);
    event->getDataID().setDataName(NAME_EVENTTRIGGER_GOOD);
    event->getDataID().setTimePoint(tr);

    setResult(msg,event);

    trsSinceTrigger = 0;
  }

  // check for bad trigger
  if(triggerBothDirections
     && (!monitorPos || posact->getPixel(0) <= -posThresh)
     && (!monitorNeg || negact->getPixel(0) >= -negThresh)) {

    RtEvent *event = new RtEvent();

    event->getDataID().setFromInputData(*posact,*this);
    event->getDataID().setDataName(NAME_EVENTTRIGGER_BAD);
    event->getDataID().setTimePoint(tr);

    setResult(msg,event);

    trsSinceTrigger = 0;
  }

  return 0;
}
