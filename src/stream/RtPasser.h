/*=========================================================================
 *  RtPasser.h is the header for a class that passes data from an
 *  stream to output
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

#ifndef RTPASSER_H
#define RTPASSER_H

#include"RtOutput.h"
#include"RtData.h"
#include"RtStreamComponent.h"

// class declaration
class RtPasser: public RtStreamComponent  {

 public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  // all data that
  RtPasser();

  // id constructor
  //  in
  //   idTemplate is a an instance of RtDataID where the fields are used as a
  //   template to match incomming data to decide if it should be send to
  //   the outputs or not. Empty fields are not matched and filled fields are
  RtPasser(const RtDataID &idTemplate);

  // destructor
  virtual ~RtPasser();

  //*** initialization routines  ***//
  void addOutput(RtOutput *out);

  // process a single acquisition
  void sendToOutputs(RtData *d);

 protected:

  // validation configuration
  bool validateComponentConfig() { return true; }

  // processes as a stream component
  int process(ACE_Message_Block*);

  // vector of outputs to pass the data to
  vector<RtOutput*> outputs;

  // only pass data of this id
  RtDataID passDataID;

};

#endif
