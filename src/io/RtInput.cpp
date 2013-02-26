/*=========================================================================
 *  RtInput.cpp defines a class that implements input operations
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

#include"RtInput.h"

// default constructor
RtInput::RtInput() {
  inputID = "input";
  activatable = false;
}

// destructor
RtInput::~RtInput() {
}

// init
bool RtInput::open(RtConfig &config) {
  // set up the callback

  return true;
}


// closes both the io object and task base
//  out:
//   true (for success) or false
bool RtInput::close() {

  // close super classes
  ACE_Task_Base::close();

  return RtIO::close();
}

// set the id string
void RtInput::setID(const string &s) {
  inputID = s;
}

// add a string to the input id
void RtInput::addToID(string add) {
  inputID += add;
}

// get the id string
string RtInput::getID() {
  return inputID;
}
