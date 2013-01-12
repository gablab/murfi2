/*=========================================================================
 *  RtEvent.cpp defines a class for an activation image
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

#include"RtEvent.h"

//*** constructors/destructors  ***//

// default constructor

RtEvent::RtEvent() : RtData() {
  ACE_TRACE(("RtMRIImage::RtEvent()"));

  dataID.setModuleID("event");

  tr = -1;
}

// destructor

RtEvent::~RtEvent() {
}

// set the tr for this event

void RtEvent::setTR(int _tr) {
  tr = _tr;
}

// get the tr for this event

int RtEvent::getTR() const {
  return tr;
}

// serialize as xml (dummy function)

TiXmlElement* RtEvent::serializeAsXML(TiXmlElement *requestElement) {
  return new TiXmlElement("event");
}

// unserialize xml (dummy function)

void RtEvent::unserializeXML(TiXmlElement* element) {

}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/
