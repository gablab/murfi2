/*=========================================================================
 *  RtInputSynth.h declares a class that sythesizes input  operations
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

#ifndef RTINPUTSYNTH_H
#define RTINPUTSYNTH_H

#include"RtInput.h"
#include"RtConfig.h"

// controls input operations to receive scanner images
class RtInputSynth : public RtInput {

 public:

  //*** constructors/destructors  ***//

  // default constructor
  RtInputSynth() : tr(2), numTrs(100) {
    addToID(":synth");
  };

  // destructor
  virtual ~RtInputSynth();

  // open and start accepting input
  virtual bool open(RtConfig &config);

  // close and clean up
  bool close();

  // run the scanner input
  virtual int svc();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

 private:

  // time between "inputs" in seconds
  float tr;

  // number of inputs to generate
  unsigned int numTrs;
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
