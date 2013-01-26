/*=========================================================================
 *  RtEvent.h declares a class for an event image
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

#ifndef RTEVENT_H
#define RTEVENT_H

#include"RtData.h"
#include"tinyxml/tinyxml.h"

#include "ace/Log_Msg.h"


// class declaration

class RtEvent : public RtData {
 public:

  //*** constructors/destructors  ***//

  // default constructor
  RtEvent();

  // construct from data
  RtEvent(const RtData &dat);

  // destructor
  virtual ~RtEvent();

  // serialize as xml (dummy function)
  virtual TiXmlElement* serializeAsXML(TiXmlElement *requestElement);

  // unserialize the xml data
  virtual void unserializeXML(TiXmlElement* element);

  // set the tr for this event
  void setTR(int _tr);

  // get the tr for this event
  int getTR() const;

 protected:

  int tr;
};

#endif
