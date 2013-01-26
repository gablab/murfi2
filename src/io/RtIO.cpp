/*=========================================================================
 *  RtIO.cpp defines a base class for input and output operations
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

static char *VERSION = "$Id$";

#include<RtIO.h>

//*** constructors/destructors  ***//

// default constructor
RtIO::RtIO() : isOpen(false) {

}

// destructor
RtIO::~RtIO() {
}

// open and start accepting input
bool RtIO::open(RtConfig &config) {
  isOpen = true;
  deleteable = false;
  return true;
}

// close and clean up
bool RtIO::close() {
  isOpen = false;

  return true;
}

// prints the current time
void RtIO::printNow(ostream &os) {
  now.update();

  os << now.year() << "/" << now.month() << "/" << now.day() << " "
     << now.hour() << ":" << now.minute() << ":" << now.second()
     << "." << now.microsec();
}

// gets the version
//  out:
//   cvs version string for this class
char *RtIO::getVersionString() {
  return VERSION;
}
