/*=========================================================================
 *  RtProcessor.h is the header for a class for a processing stream
 *  component for a real-time fMRI.
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

#ifndef RTPROCESSOR_H
#define RTPROCESSOR_H

#include"RtStreamComponent.h"
#include"RtData.h"
#include"RtPasser.h"

// class declaration
class RtProcessor : public RtStreamComponent, public RtStream {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtProcessor();

  // destructor
  ~RtProcessor();

  //*** initialization routines  ***//

  // initialize stream and prepare to run
  //  out:
  //   true (for success) or false
  bool configure(RtConfig &config);

  // adds all modules to the stream
  //  in
  //   config: configuration info
  bool addModules(RtConfig &config);

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString() const;

  

protected:

  // pure virtual for validation of component configuration
  bool validateComponentConfig();

  // process a single acquisition
  int process(ACE_Message_Block *mb);

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


