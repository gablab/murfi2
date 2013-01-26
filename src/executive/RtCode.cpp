/*=========================================================================
 *  RtCode.h is the header for an abstract class that
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

#include"RtCode.h"
#include"RtConductor.h"

// default constructor
RtCode::RtCode() {
  conductor = NULL;
  codeNum = -1;
}

// constructor w/ codenum
RtCode::RtCode(RtConductor *_conductor, int _codeNum)
    : conductor(_conductor), codeNum(_codeNum)
{  }

// destructor
RtCode::~RtCode() {

}


// set conductor
void RtCode::setConductor(RtConductor *_conductor) {
  conductor = _conductor;
}

// set the code number
void RtCode::setCodeNum(int _codeNum) {
  codeNum = _codeNum;
}

// get the code number
int RtCode::getCodeNum() {
  return codeNum;
}

// send the code to the handler
void RtCode::sendCode(RtData *data) {
  ACE_TRACE(("RtCode::sendCode"));

  if(conductor == NULL) {
    return;
  }

  if(data == NULL) {
    conductor->receiveCode(SHUTDOWN,NULL);
    return;
  }

  conductor->receiveCode(codeNum,data);
}

// log a message to the global logfile
void RtCode::log(const string &s) {
  if(conductor == NULL) {
    return;
  }
  conductor->log(s);
}

// log a message to the global logfile
void RtCode::log(stringstream &s) {
  if(conductor == NULL) {
    return;
  }
  conductor->log(s);
}
