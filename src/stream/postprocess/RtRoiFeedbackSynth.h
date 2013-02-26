/*=========================================================================
 *  RtRoiFeedbackSynth.h is the header for a class that synthesizes roi
 *  activations
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

#ifndef RTROIFEEDBACKSYNTH_H
#define RTROIFEEDBACKSYNTH_H

#include"RtStreamComponent.h"

// class declaration
class RtRoiFeedbackSynth : public RtStreamComponent {

 public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtRoiFeedbackSynth();

  // destructor
  virtual ~RtRoiFeedbackSynth();

 protected:

  // process an option
  //  in
  //   name of the option to process
  //   val  text of the option node
  bool processOption(const string &name,
                     const string &text,
                     const map<string,string> &attrMap);

  // validate config
  bool validateComponentConfig();

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  unsigned int numTimepoints;
  float mean;
  float sd;

};

#endif
