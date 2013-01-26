/*=========================================================================
 *  RtEventTrigger.cpp triggers an event based on activation sums
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
