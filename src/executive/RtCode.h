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

#ifndef RTCODE_H
#define RTCODE_H

#include"RtData.h"
#include<sstream>
#include<string>

using namespace std;

// explicitly defined codes (not the only ones

#define START_CODE_STREAM 0
#define START_CODE_INPUTS 1
#define SCANNER_IMAGE_RECEIVED START_CODE_INPUTS
#define START_CODE_OUTPUTS 32768
#define SHUTDOWN 65535

class RtConductor;

class RtCode {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtCode();
  
  // constructor w/ codenum
  RtCode(RtConductor *_conductor, int _codeNum);

  // destructor
  virtual ~RtCode();

  // set conductor
  void setConductor(RtConductor *_conductor);

  // set the code number
  void setCodeNum(int _codeNum);

  // get the code number
  int getCodeNum();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString() {
    return "$Id$";
  }

protected:

  // send the code to the handler
  void sendCode(RtData *data);

  // log a message to the global logfile
  void log(const string &s);

  // log a message to the global logfile
  void log(stringstream &s);

  // conductor instance to send codes to
  RtConductor *conductor;

  // code number
  int codeNum;

};

#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


