/*=========================================================================
 *  RtOutput.h declares a class that implements output operations 
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

#ifndef RTOUTPUT_H
#define RTOUTPUT_H

#include"RtIO.h"
#include"RtConfig.h"
#include"RtData.h"
#include<ostream>

// class declaration
class RtOutput : public RtIO {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtOutput(); 

  // destructor
  virtual ~RtOutput();

  // open and start accepting input
  virtual bool open(RtConfig &config);

  // hand of some data to be output
  virtual void setData(RtData *data);

  // set the id string
  void setID(const string &s);

  // add a string to the output id
  void addToID(string add);

  // get the id string
  string getID();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

protected:

  //*** private data members  ***//

  string outputID;
  bool deleteable;
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


