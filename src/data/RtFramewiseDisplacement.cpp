/*=========================================================================
 *  RtFramewiseDisplacement.cpp defines a class to store subject
 *  motion relative to the last frame.
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

#include"RtFramewiseDisplacement.h"

#include "RtDataIDs.h"

//*** constructors/destructors  ***//

RtFramewiseDisplacement::RtFramewiseDisplacement() : RtData() {
  ACE_TRACE(("RtData::RtFramewiseDisplacement()"));

  dataID.setModuleID(ID_FRAMEWISE_DISPLACEMENT);
  dataID.setDataName(NAME_FRAMEWISE_DISPLACEMENT);

  displacement = 0.0;
}

// construct with the last two motion objects
RtFramewiseDisplacement::RtFramewiseDisplacement(RtMotion *lastMotion,
                                                 RtMotion *thisMotion) {
  ACE_TRACE(("RtData::RtFramewiseDisplacement()"));

  dataID.setModuleID(ID_FRAMEWISE_DISPLACEMENT);
  dataID.setDataName(NAME_FRAMEWISE_DISPLACEMENT);

  displacement =
    abs(lastMotion->getMotionDimension(TRANSLATION_X) -
        thisMotion->getMotionDimension(TRANSLATION_X)) +
    abs(lastMotion->getMotionDimension(TRANSLATION_Y) -
        thisMotion->getMotionDimension(TRANSLATION_Y)) +
    abs(lastMotion->getMotionDimension(TRANSLATION_Z) -
        thisMotion->getMotionDimension(TRANSLATION_Z)) +
    50 * abs(lastMotion->getMotionDimension(ROTATION_X) -
             thisMotion->getMotionDimension(ROTATION_X)) +
    50 * abs(lastMotion->getMotionDimension(ROTATION_Y) -
             thisMotion->getMotionDimension(ROTATION_Y)) +
    50 * abs(lastMotion->getMotionDimension(ROTATION_Z) -
             thisMotion->getMotionDimension(ROTATION_Z));
}

RtFramewiseDisplacement::~RtFramewiseDisplacement() {
}

// get the displacement
double RtFramewiseDisplacement::getDisplacement() {
  return displacement;
}

// serialize as xml (dummy function)
TiXmlElement*
RtFramewiseDisplacement::serializeAsXML(TiXmlElement *requestElement) {
  return new TiXmlElement("framewise-displacement");
}

// unserialize xml (dummy function)
void RtFramewiseDisplacement::unserializeXML(TiXmlElement* element) {
}
