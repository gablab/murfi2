/*=========================================================================
 *  RtInputUSB.cpp defines a class that implements usb keyboard input
 *  operations
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

#include"RtInputUSBKb.h"

//*** constructors/destructors  ***//

// default constructor
RtInputUSBKb::RtInputUSBKb() {
  addToID(":usbkb");
}

// destructor
RtInputUSBKb::~RtInputUSBKb() {

}

// open and start accepting input
bool RtInputUSBKb::init(const RtConfig &config) {
  if(!RtInputUSBKb::init(config)) {
    return false;
  }

  return true;
}

// close and clean up
bool RtInputUSBKb::close() {
  return true;
}
