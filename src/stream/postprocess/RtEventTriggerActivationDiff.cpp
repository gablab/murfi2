/*=========================================================================
 *  RtEventTriggerActivationSumDiff.cpp triggers an event based on
 *  activation sums
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

#include"RtEventTriggerActivationDiff.h"
#include"RtDataIDs.h"
#include"RtActivation.h"

#include"RtEvent.h"

string RtEventTriggerActivationDiff::moduleString(
    ID_EVENTTRIGGER_ACTIVATIONDIFF);

// default constructor
RtEventTriggerActivationDiff::RtEventTriggerActivationDiff()
  : RtEventTrigger() {

  posActivationSumModuleID = ID_ROICOMBINE;
  negActivationSumModuleID = ID_ROICOMBINE;
  posActivationSumDataName = NAME_ROIMEAN;
  negActivationSumDataName = NAME_ROIMEAN;

  componentID = moduleString;
  diffThresh = 1.0;
}

// destructor
RtEventTriggerActivationDiff::~RtEventTriggerActivationDiff() {}

// process an option
//  in
//   name of the option to process
//   val  text of the option node
bool RtEventTriggerActivationDiff::processOption(
    const string &name, const string &text, const map<string,string> &attrMap) {
  if(name == "diffThresh") {
    return RtConfigVal::convert<double>(diffThresh,text);
  }
  else if(name == "posActivationModuleID") {
    posActivationSumModuleID = text;
    return true;
  }
  else if(name == "negActivationModuleID") {
    negActivationSumModuleID = text;
    return true;
  }
  else if(name == "posActivationDataName") {
    posActivationSumDataName = text;
    return true;
  }
  else if(name == "negActivationDataName") {
    negActivationSumDataName = text;
    return true;
  }

  return RtEventTrigger::processOption(name, text, attrMap);
}

// validate the configuration
bool RtEventTriggerActivationDiff::validateComponentConfig() {
  bool result = true;

  return result;
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
      = (RtActivation*) msg->getData(posActivationSumModuleID,
                                     posActivationSumDataName,
                                     posRoiID);
  RtActivation *negact
      = (RtActivation*) msg->getData(negActivationSumModuleID,
                                     negActivationSumDataName,
                                     negRoiID);

  if(posact == NULL) {
    cout << "couldn't find positive roi " << posRoiID << endl;

    ACE_DEBUG(
        (LM_INFO,
         "RtEventTriggerActivationDiff:process: no positive ROI found\n"));
    return 0;
  }

  if(negact == NULL) {
    cout << "couldn't find negative roi " << negRoiID << endl;

    ACE_DEBUG(
        (LM_INFO,
         "RtEventTriggerActivationDiff:process: no negative ROI found\n"));
    return 0;
  }

  int tr = posact->getDataID().getTimePoint();
  cout << "checking for even trigger: " << tr << endl;

  if(tr < initialSkipTRs) {
    return 0;
  }

  // check for goodrigger
  if(posact->getPixel(0) - negact->getPixel(0) >= diffThresh) {
    // trigger
    RtEvent *event = new RtEvent();

    event->getDataID().setFromInputData(*posact,*this);
    event->getDataID().setDataName(NAME_EVENTTRIGGER_GOOD);
    event->getDataID().setTimePoint(tr);

    setResult(msg,event);

    // log the trigger
    stringstream logs("");
    logs << "trigger event good at tr " << tr << ": "
         << posact->getPixel(0) - negact->getPixel(0) << " >= "
         << diffThresh << endl;
    log(logs);

    trsSinceTrigger = 0;
  }

  // check for bad trigger
  if(posact->getPixel(0) - negact->getPixel(0) <= -diffThresh) {
    // trigger
    RtEvent *event = new RtEvent();

    event->getDataID().setFromInputData(*posact,*this);
    event->getDataID().setDataName(NAME_EVENTTRIGGER_BAD);
    event->getDataID().setTimePoint(tr);

    setResult(msg,event);

    // log the trigger
    stringstream logs("");
    logs << "trigger event bad at tr " << tr << ": "
         << posact->getPixel(0) - negact->getPixel(0) << " <= "
         << -diffThresh << endl;
    log(logs);

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
