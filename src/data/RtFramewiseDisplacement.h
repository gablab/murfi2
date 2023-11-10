/*=========================================================================
 *  RtFramewiseDisplacement.h declares a class to store subject head
 *  motion relative to the previous frame.
 *
 *  Copyright 2007-2023, the MURFI dev team.
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

#ifndef RTFRAMEWISEDISPLACEMENT_H
#define RTFRAMEWISEDISPLACEMENT_H

#include"RtData.h"
#include"RtMotion.h"
#include"tinyxml/tinyxml.h"

class RtFramewiseDisplacement : public RtData {
 public:

  //*** constructors/destructors  ***//

  RtFramewiseDisplacement();

  // construct with past two motion objects
  RtFramewiseDisplacement(RtMotion *lastMotion, RtMotion *thisMotion);

  virtual ~RtFramewiseDisplacement();

  // serialize as xml (dummy function)
  virtual TiXmlElement* serializeAsXML(TiXmlElement *requestElement);

  // unserialize the xml data
  virtual void unserializeXML(TiXmlElement* element);

  // get the displacement
  double getDisplacement();

 private:

  double displacement;
};

#endif
