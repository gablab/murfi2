/*=========================================================================
 *  RtFluctuationMonitor.cpp is the implementation of a class that monitors
 *  spontaneous fluctuations in the BOLD signal
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

#include"RtFluctuationMonitor.h"
#include"RtMRIImage.h"
#include"RtActivation.h"
#include"RtDataIDs.h"
#include<limits>

string RtFluctuationMonitor::moduleString(ID_FLUCTUATIONMONITOR);


// default constructor
RtFluctuationMonitor::RtFluctuationMonitor() : RtIncrementalGLM() {
  componentID = moduleString;
  outputID += ":" + string(ID_FLUCTUATIONMONITOR);

  absEstErrSum = NULL;
  numDataInErrSum = 0;
  numDataPointsForErrEst = std::numeric_limits<unsigned int>::max();

  numTimepoints = 0;

  triggerStim = false;
  isTriggered = false;
  afterTriggerSkip = 8;
  numImagesSinceTrigger = INT_MAX;
}

// destructor
RtFluctuationMonitor::~RtFluctuationMonitor() {
  if(absEstErrSum != NULL) {
    delete absEstErrSum;
  }
}

// look for events we triggered
void RtFluctuationMonitor::setData(RtData *data) {
  if(data->getDataID().getDataName() == NAME_EVENTTRIGGER_GOOD
     || data->getDataID().getDataName() == NAME_EVENTTRIGGER_BAD) {
    receiveStimTriggered();
  }
}

// receive a message that stimulus has been triggered
void RtFluctuationMonitor::receiveStimTriggered() {
  numImagesSinceTrigger = 0;
  isTriggered = true;
  design.addEvent(numTimepoints, design.getEventDuration());
}

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node
bool RtFluctuationMonitor::processOption(const string &name, const string &text,
                                         const map<string,string> &attrMap) {
  // look for known options
  if(name == "triggerStim") {
    return RtConfigVal::convert<bool>(triggerStim,text);
  }
  if(name == "afterTriggerSkip") {
    return RtConfigVal::convert<int>(afterTriggerSkip,text);
  }
  if(name == "numDataPointsForErrEst") {
    return RtConfigVal::convert<unsigned int>(numDataPointsForErrEst,text);
  }

  return RtIncrementalGLM::processOption(name, text, attrMap);
}

// validate the configuration
bool RtFluctuationMonitor::validateComponentConfig() {
  bool result = true;

  return result;
}

// process a single acquisition
int RtFluctuationMonitor::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtFluctuationMonitor::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *dat = (RtMRIImage*)msg->getCurrentData();

  if(dat == NULL) {
    cout << "RtFluctuationMonitor::process: data passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtFluctuationMonitor:process: data passed is NULL\n"));
    return 0;
  }

  RtMaskImage *mask = getMaskFromMessage(*msg);
  //cout << "operating on image: " << dat->getID() << endl;

  numTimepoints++;

  // initialize the computation if necessary
  if(needsInit) {

    initEstimation(*dat, mask);

    // setup the estimation error sum of squares
    if(absEstErrSum != NULL) {
      delete absEstErrSum;
    }
    absEstErrSum = new RtActivation(*dat);
    absEstErrSum->initToZeros();

  }

  // allocate a new data image for the estimation
  RtActivation *fluct = new RtActivation(*dat);

  // setup the data id
  fluct->getDataID().setFromInputData(*dat,*this);
  fluct->getDataID().setDataName(NAME_ACTIVATION);
  fluct->getDataID().setRoiID(mask->getDataID().getRoiID());

  fluct->initToNans();

  //// element independent setup

  // check for out of error est region
  bool dontInclude = false;
  if(numTimepoints > numDataPointsForErrEst) {
    dontInclude = true;
  }
  else { // check the time since triggers
    if(isTriggered && afterTriggerSkip >= numImagesSinceTrigger++) {
      dontInclude = true;
    }
    else if(isTriggered) {
      isTriggered = false;
      dontInclude = false;
    }
  }

  // increment the number of included points
  if(!dontInclude) {
    numDataInErrSum++;
  }

  if(isTriggered && numImagesSinceTrigger++ >= afterTriggerSkip) {
    isTriggered = false;
  }

  vnl_vector<double> row = design.getRow(dat->getDataID().getTimePoint());

  //// compute t map for each element
  for(unsigned int i = 0; i < dat->getNumEl(); i++) {
    if(!mask->getPixel(i)) {
      continue;
    }

    double y = dat->getElement(i);

    solvers[i]->include(&y,row.data_block(),1.0);

    double *beta = solvers[i]->regress(0);
    double err = y;

    // subtract the reconstruction based on trend fit
    for(unsigned int j = 0; j < row.size(); j++) {
      err -= beta[j]*row[j];
    }

    double stdDev;
    if(!dontInclude) {
      absEstErrSum->setPixel(i, absEstErrSum->getPixel(i) + fabs(err));
    }
    stdDev = absEstErrSum->getPixel(i)/(numDataInErrSum-1);

    fluct->setPixel(i, err/stdDev);

    if(dumpAlgoVars && numTimepoints > 2) {
      dumpFile
          << numTimepoints << " "
          << i << " "
          << y << " "
          << err << " "
          << stdDev << " "
          << fluct->getPixel(i) << " "
          << isTriggered << " "
          << dontInclude;
      for(unsigned int b = 0; b < row.size(); b++) {
        dumpFile << beta[b] << " ";
      }
      dumpFile << endl;
    }

    delete beta;
  }

  setResult(msg,fluct);

  return 0;
}


// start a logfile
void RtFluctuationMonitor::startDumpAlgoVarsFile() {
  dumpFile << "started at ";
  printNow(dumpFile);
  dumpFile << endl
           << "time_point "
           << "voxel_index "
           << "voxel_intensity "
           << "activation_signal "
           << "std_dev "
           << "fluctuation "
           << "trigerred "
           << "skipping ";
  for(unsigned int b = 0; b < design.getNumColumns(); b++) {
    string bstr;
    RtConfigVal::convertToString<int>(bstr,b);
    dumpFile << string("beta") + bstr + " ";
  }
  dumpFile << "end" << endl;
}
