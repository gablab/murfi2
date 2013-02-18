/*=========================================================================
 *  RtInputUSB.h declares a class that implements usb keyboard input operations
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

#ifndef RTINPUTUSBKB_H
#define RTINPUTUSBKB_H

#include"RtInput.h"

// class declaration
class RtInputUSBKb : public RtInput {

 public:

  //*** constructors/destructors  ***//

  // default constructor
  RtInputUSBKb();

  // destructor
  virtual ~RtInputUSBKb();

  // open and start accepting input
  bool init(const RtConfig &config);

  // close and clean up
  bool close();
};

#endif
