/*=========================================================================
 *  RtInputSynth.cpp defines a class that implements scanner
 *  image communication operations
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

#include"RtInputSynth.h"
#include"RtDataIDs.h"
#include"RtActivation.h"
#include"RtExperiment.h"

#include<fstream>

using namespace std;

// destructor
RtInputSynth::~RtInputSynth() {

}

// configure and open
bool RtInputSynth::open(RtConfig &config) {
  RtInput::open(config);

  if(config.isSet("inputsynth:tr")) {
    tr = config.get("inputsynth:tr");
  }

  if(config.isSet("inputsynth:numTrs")) {
    numTrs = config.get("inputsynth:numTrs");
  }

  return true;
}

// close and clean up
//  out
//   success flag
bool RtInputSynth::close() {

  if(isOpen) {
    // other stuff
  }

  RtInput::close();

  return true;
}

// run the scanner input
int RtInputSynth::svc() {
  RtActivation *synth;
  stringstream infos;

  for(unsigned int curTr = 0; curTr < numTrs; curTr++) {

    synth = new RtActivation(0);
    synth->getDataID().setModuleID("inputsynth");
    synth->getDataID().setDataName("none");
    synth->getDataID().setTimePoint(curTr+1);

    sendCode(synth);

    // log that we received the image
    infos.str("");
    infos << "generatred synthetic data at tr " << curTr << endl;
    log(infos);

    cout << "generatred synthetic data at tr " << curTr << endl;

#ifdef WIN32
    Sleep(1000*tr);
#else
    usleep(1000000*tr);
#endif

  }
  return 0;
}
