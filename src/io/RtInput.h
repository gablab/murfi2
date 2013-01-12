/*=========================================================================
 *  RtInput.h declares a class that implements input operations 
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

#ifndef RTINPUT_H
#define RTINPUT_H

//#include <sigc++/sigc++.h>

#include"ace/Task.h"

#include"RtIO.h"
#include"RtData.h"
#include"RtConfig.h"

// class declaration
class RtInput : public RtIO, public ACE_Task_Base {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtInput(); 

  // destructor
  virtual ~RtInput();
  
  // set the activatable property
  void setActivatable(bool _activatable) { activatable = _activatable; }

  // get the activatable property
  bool isActivatable() { return activatable; }

  // opens an io object and initialize it
  //  out:
  //   true (for success) or false
  virtual bool open(RtConfig &config);

  // closes both the io object and task base
  //  out:
  //   true (for success) or false
  virtual bool close();

  // set the id string
  void setID(const string &s);

  // add a string to the input id
  void addToID(string add);

  // get the id string
  string getID();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

protected:

  //*** private data members  ***//

  // callback to receive the input
  //sigc::signal<void, RtData&> callback;
  
  string inputID;
  bool activatable;

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


