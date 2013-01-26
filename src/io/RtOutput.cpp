/*=========================================================================
 *  RtOutput.cpp defines a class that implements output operations
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

static char *VERSION = "$Id$";

#include"RtOutput.h"

// default constructor
RtOutput::RtOutput() {
  outputID = "output";
}

// destructor
RtOutput::~RtOutput() {

}

// init
bool RtOutput::open(RtConfig &config) {
  if(!RtIO::open(config)) {
    return false;
  }

  return true;
}

// hand off some data to be output
void RtOutput::setData(RtData *data) {
  //
}

// set the id string
void RtOutput::setID(const string &s) {
  outputID = s;
}

// add a string to the output id
void RtOutput::addToID(string add) {
  outputID += add;
}

// get the id string
string RtOutput::getID() {
  return outputID;
}


// gets the version
//  out:
//   cvs version string for this class
char *RtOutput::getVersionString() {
  return VERSION;
}
