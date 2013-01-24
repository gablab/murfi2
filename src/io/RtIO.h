/*=========================================================================
 *  RtIO.h declares a base class for input and output operations
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

#ifndef RTIO_H
#define RTIO_H

#include"RtCode.h"
#include"RtConfig.h"
#include"ace/Date_Time.h"

// class declaration
class RtIO : public RtCode {

 public:

  //*** constructors/destructors  ***//

  // default constructor
  RtIO();

  // destructor
  virtual ~RtIO();

  //*** initialization routines  ***//

  // opens an io object and initialize it
  //  out:
  //   true (for success) or false
  virtual bool open(RtConfig &config);

  // closes an io object
  //  out:
  //   true (for success) or false
  virtual bool close();

  // set the deleteable property
  void setDeleteable(bool _deleteable) { deleteable = _deleteable; }

  // get the deleteable property
  bool isDeleteable() { return deleteable; }

  // set the id string
  void setID(const string &s);

  // get the id string
  string getID();

  // prints the current time
  void printNow(ostream &os);

  // gets the version
  //  out:
  //   cvs version string for this class
  virtual char *getVersionString();

 protected:

  bool isOpen;
  ACE_Date_Time now;

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
