/*=========================================================================
 *  RtVar.h is the header for a class that computes the variance over a
 *  set of images. this is an implementation of the west (1979) algorithm for
 *  incremental variance computation. 
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

#ifndef RTVAR_H
#define RTVAR_H

#include"RtStreamComponent.h"
#include"RtActivation.h"

// class declaration
class RtVar : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtVar();

  // destructor
  ~RtVar();

protected:

  // process an option
  //  in 
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text, 
			     const map<string,string> &attr);

  // validate config
  virtual bool validateComponentConfig();

  // process a single acquisition
  virtual int process(ACE_Message_Block *mb);

  unsigned int numTimePoints;

  // the mean image to use in var calculation
  string meanModuleID;  
  string meanDataName;  

  // saves the variance numerator image up to t-1
  RtActivation varnum;
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


