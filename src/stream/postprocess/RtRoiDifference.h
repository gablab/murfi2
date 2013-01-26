/*=========================================================================
 *  RtRoiDifference.h is the header for a class that computes the
 *  difference between two activation map combinations
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

#ifndef RTROIDIFFERENCE_H
#define RTROIDIFFERENCE_H

#include"RtStreamComponent.h"
#include"RtActivation.h"

// class declaration
class RtRoiDifference : public RtStreamComponent {

 public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtRoiDifference();

  // destructor
  virtual ~RtRoiDifference();

 protected:

  // process an option
  //  in
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text,
                             const map<string,string> &attr);

  // validate config
  bool validateComponentConfig();

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // dataID for the positive and negative activation sums
  string posRoiModuleID;
  string negRoiModuleID;
  string posRoiDataName;
  string negRoiDataName;
  string posRoiRoiID;
  string negRoiRoiID;

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
