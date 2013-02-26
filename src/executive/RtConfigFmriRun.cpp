/*=========================================================================
 *  RtConfigFmriRun.cpp defines a class that controls configuration of
 *  a single real-time fMRI experimental run.
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

#include"RtConfigFmriRun.h"
#include"RtLimit.h"

#include"ace/Trace.h"

// defaults
static const unsigned int DEFAULT_SCANNERPORT = 15000;
static const string DEFAULT_IMAGETYPE = "epi";
static const string DEFAULT_STUDYREFNAME(  "study_ref");

// set default config info
void RtConfigFmriRun::setDefaults() {

  set("scanner:disabled",false);
  set("scanner:receiveImages",true);
  set("scanner:port", DEFAULT_SCANNERPORT);

  set("scanner:saveImages",true);
  set("scanner:onlyReadMoco",true);
  set("scanner:unmosaic",true);

}

// validate the configuration
// checks for valid setup of different parts of the program
// returns true for success
bool RtConfigFmriRun::validateConfig() {
  //ACE_TRACE(("RtConfigFmriRun::validateConfig"));

  bool valid = true;
  stringstream ss;

  // check image receiver
  if(get("scanner:disabled")==false) {
    if((unsigned int) get("scanner:port") < 1
       || (unsigned int) get("scanner:port") > MAX_TCPIP_PORT_NUM) {
      cerr << "WARNING: invalid port number for receiving scanner images"
           << endl;
      set("scanner:receiveImages",false);
    }
  }

  // check optional acquisition parameters
  if(!isSet("scanner:imageType")==false) {
    set("scanner:imageType",DEFAULT_IMAGETYPE);
  }

  // check required acquisition parameters
  if(!isSet("scanner:tr")) {
    cerr << "ERROR: tr must be set!" << endl;
  }

  if(!isSet("scanner:measurements")) {
    cerr << "ERROR: number of measurements must be set!" << endl;
  }

  if(!isSet("scanner:matrixSize")) {
    cerr << "ERROR: matrix size must be set!" << endl;
  }

  if(!isSet("scanner:slices")) {
    cerr << "ERROR: slices must be set!" << endl;
  }

  if(!isSet("scanner:voxdim1")) {
    cerr << "ERROR: voxdim1 must be set!" << endl;
  }

  if(!isSet("scanner:voxdim2")) {
    cerr << "ERROR: voxdim2 must be set!" << endl;
  }

  if(!isSet("scanner:voxdim3")) {
    cerr << "ERROR: voxdim3 must be set!" << endl;
  }

  // study reference volume
  if(!isSet("study:xfm:referenceVol")) {
    path p;
    p.operator=(get("study:xfm:directory").filepath()
                / (DEFAULT_STUDYREFNAME + "."
                   + get("study:volumeFormat").str()));
    cout << "using default study reference volume name " << p.string() << endl;
    set("study:xfm:referenceVol",p.string());
  }

  return valid;
}
