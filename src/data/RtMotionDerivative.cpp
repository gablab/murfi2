/*=========================================================================
 *  RtMotionDerivative.cpp defines a class to store subject motion
 *  relative to the last acquired volume.
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

#include"RtMotionDerivative.h"

#include "RtDataIDs.h"

//*** constructors/destructors  ***//

RtMotionDerivative::RtMotionDerivative() : RtMotion() {
  ACE_TRACE(("RtMotionDerivative::RtMotionDerivative()"));

  dataID.setModuleID(ID_MOTIONDERIVATIVE);
  dataID.setDataName(NAME_MOTIONDERIVATIVE);
}

// construct with motions
RtMotionDerivative::RtMotionDerivative(RtMotion *lastMotion, RtMotion *thisMotion) {
  ACE_TRACE(("RtMotionDerivative::RtMotionDerivative()"));

  dataID.setModuleID(ID_MOTIONDERIVATIVE);
  dataID.setDataName(NAME_MOTIONDERIVATIVE);

  setMotion(thisMotion->getMotionDimension(TRANSLATION_X) -
            lastMotion->getMotionDimension(TRANSLATION_X),
            thisMotion->getMotionDimension(TRANSLATION_Y) -
            lastMotion->getMotionDimension(TRANSLATION_Y),
            thisMotion->getMotionDimension(TRANSLATION_Z) -
            lastMotion->getMotionDimension(TRANSLATION_Z),
            50 * (thisMotion->getMotionDimension(ROTATION_X) -
                  lastMotion->getMotionDimension(ROTATION_X)),
            50 * (thisMotion->getMotionDimension(ROTATION_Y) -
                  lastMotion->getMotionDimension(ROTATION_Y)),
            50 * (thisMotion->getMotionDimension(ROTATION_Z) -
                  lastMotion->getMotionDimension(ROTATION_Z)));
}

RtMotionDerivative::~RtMotionDerivative() {
}