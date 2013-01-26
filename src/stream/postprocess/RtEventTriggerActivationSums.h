/*=========================================================================
 *  RtEventTriggerActivationSums.h is the header for a class to trigger an
 *  event based on activation sums
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

#ifndef RTEVENTTRIGGERACTIVATIONSUMS_H
#define RTEVENTTRIGGERACTIVATIONSUMS_H

#include"RtEventTrigger.h"

// class declaration
class RtEventTriggerActivationSums : public RtEventTrigger {

 public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtEventTriggerActivationSums();

  // destructor
  virtual ~RtEventTriggerActivationSums();

 protected:

  // process an option
  //  in
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text,
                             const map<string,string> &attr);

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // make sure we are configured properly
  bool validateComponentConfig();

  // ids for the activation sums
  string posActivationSumModuleID;
  string negActivationSumModuleID;
  string posActivationSumDataName;
  string negActivationSumDataName;

  // flags or whether to monitor rois
  bool monitorPos;
  bool monitorNeg;

  // whether we should trigger on good and bad activation directions
  bool triggerBothDirections;

  // thresholds for triggering
  double posThresh;
  double negThresh;

  int afterTriggerSkipTRs;
};

#endif
