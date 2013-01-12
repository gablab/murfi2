/*=========================================================================
 *  RtData.cpp defines a base class for data classes
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

#include"RtData.h"

  
// default constructor
RtData::RtData() : numEl(0), elType(RT_UNKNOWN_TYPE) {}

// destructor
RtData::~RtData() {
}

// gets the id string for this data
//  out
//   id string
RtDataID &RtData::getDataID() {
  return dataID;
}

// gets the id string for this data
//  out
//   id string
RtDataID RtData::getDataID() const {
  return dataID;
}

// set the id string for this data
//  in
//   string to set the id to
void RtData::setDataID(const RtDataID &id) {
  dataID = id;
}


// get the number of elements in this datatype
unsigned int RtData::getNumEl() const {
  return numEl;
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


