/*=========================================================================
 *  RtFluctuationMonitor.h is the header for a class that estimates the
 *  activation at each voxel in a single image using a GLM-based
 *  correlation analysis based on Hids, et al., 2008
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

#ifndef RTFLUCTUATIONMONITOR_H
#define RTFLUCTUATIONMONITOR_H

#include<vnl/vnl_vector.h>
#include<vnl/vnl_matrix.h>

#include"RtIncrementalGLM.h"
#include"RtOutput.h"

#include<vector>
using namespace std;

// class declaration
class RtFluctuationMonitor : public RtIncrementalGLM {

 public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtFluctuationMonitor();

  // destructor
  virtual ~RtFluctuationMonitor();

  // receive a message that stimulus has been triggered
  void receiveStimTriggered();

  // look for triggered events so we can stop estimation during task
  virtual void setData(RtData *data);

 protected:

  // process a single acquisition
  virtual int process(ACE_Message_Block *mb);

  // process an option
  //  in
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text,
                             const map<string,string> &attr);

  // make sure we are configured properly
  bool validateComponentConfig();

  // start a logfile
  virtual void startDumpAlgoVarsFile();

  //// parameters of the activation estimation algorithm

  // number of timepoints
  unsigned int numTimepoints; // so far

  // store the per pixel sum of squared error
  RtActivation *absEstErrSum;
  int numDataInErrSum;

  // the amount of data to use in the estimation of the error
  unsigned int numDataPointsForErrEst;

  // for task triggering based on activation sum
  bool triggerStim;
  bool isTriggered;
  int afterTriggerSkip;
  int numImagesSinceTrigger;

};

#endif
