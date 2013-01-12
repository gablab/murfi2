/*=========================================================================
 *  RtRoiFeedbackSynth.cpp computes the mean over all voxels in an roi map
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

#include "RtRoiFeedbackSynth.h"
#include "RtDataIDs.h"


#include"boost/random/normal_distribution.hpp"
#include"boost/random/lagged_fibonacci.hpp"

string RtRoiFeedbackSynth::moduleString(ID_ROIFEEDBACKSYNTH);

// default constructor
RtRoiFeedbackSynth::RtRoiFeedbackSynth() : RtStreamComponent(),
					   numTimepoints(0), 
					   mean(0), 
					   sd(1) {
  componentID = moduleString;
}

// destructor
RtRoiFeedbackSynth::~RtRoiFeedbackSynth() {}

// process an option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtRoiFeedbackSynth::processOption(const string &name, 
					   const string &text,
					   const map<string,string> &attrMap) {
  if(name == "mean") {
    return RtConfigVal::convert<float>(mean,text);
  }
  else if(name == "sd") {
    return RtConfigVal::convert<float>(sd,text);
  }

  return RtStreamComponent::processOption(name, text, attrMap);
}  

// validate config
bool RtRoiFeedbackSynth::validateComponentConfig() {
  bool result = true;

  return result;
}

// process a single acquisition
int RtRoiFeedbackSynth::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtRoiFeedbackSynth::process"));

  static boost::normal_distribution<float> rnd(mean, sd);
  static boost::lagged_fibonacci19937 engine;
  numTimepoints++;

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // create a one element activation image
  RtActivation *act = new RtActivation(1);

  // setup the data id
  act->getDataID().setModuleID(componentID);
  act->getDataID().setDataName(NAME_ROIVAL);
  act->getDataID().setRoiID("synth");
  act->getDataID().setTimePoint(numTimepoints);

  act->setPixel(0, rnd.operator()<boost::lagged_fibonacci19937>((engine)));
  
  setResult(msg, act);

  // log the activation
  stringstream logs("");
  logs << "activation synth: " << numTimepoints << ":synth"  
       << " " << act->getPixel(0) << endl;
  log(logs);

  if(ofile.is_open()) {
    ofile << numTimepoints << " " << act->getPixel(0) << endl;
    ofile.flush();
  }

  if(DEBUG_LEVEL & TEMP) {
    cout << "activation synth: " << numTimepoints << " " << act->getPixel(0) << endl;
    cout.flush();
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


