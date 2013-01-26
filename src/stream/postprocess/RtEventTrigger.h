/*=========================================================================
 *  RtEventTrigger.h is the header for a class to trigger an event based on
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

#ifndef RTEVENTTRIGGER_H
#define RTEVENTTRIGGER_H

#include"RtStreamComponent.h"

// class declaration
class RtEventTrigger : public RtStreamComponent {

 public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtEventTrigger();

  // destructor
  virtual ~RtEventTrigger();

 protected:

  // process an option
  //  in
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text,
                             const map<string,string> &attr);

  // make sure we are configured properly
  bool validateComponentConfig();

  // process a single acquisition
  virtual int process(ACE_Message_Block *mb) = 0;

  // roiIDs for the activation sums
  string posRoiID;
  string negRoiID;

  // flags or whether to monitor rois
  bool monitorPos;
  bool monitorNeg;

  // whether we should trigger on good and bad activation directions
  bool triggerBothDirections;

  // thresholds for triggering
  double posThresh;
  double negThresh;

  int afterTriggerSkipTRs;
  int initialSkipTRs;
};

#endif
