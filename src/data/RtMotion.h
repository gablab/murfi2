/*=========================================================================
 *  RtMotion.h declares a class to store subject head motion
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

#ifndef RTMOTION_H
#define RTMOTION_H

#include"RtData.h"
#include"tinyxml/tinyxml.h"

typedef enum {
  TRANSLATION_X = 0,
  TRANSLATION_Y,
  TRANSLATION_Z,
  ROTATION_X,
  ROTATION_Y,
  ROTATION_Z,
  NUM_MOTION_DIMENSIONS
} MotionDimension;
typedef double Motion[NUM_MOTION_DIMENSIONS];

class RtMotion : public RtData {
 public:

  //*** constructors/destructors  ***//

  RtMotion();

  // construct with motion
  RtMotion(double tx, double ty, double tz, double rx, double ry, double rz);

  virtual ~RtMotion();

  // serialize as xml (dummy function)
  virtual TiXmlElement* serializeAsXML(TiXmlElement *requestElement);

  // unserialize the xml data
  virtual void unserializeXML(TiXmlElement* element);

  // set the motion vector
  void setMotion(Motion _motion);

  // set the motion vector
  void setMotion(double tx, double ty, double tz,
                 double rx, double ry, double rz);

  // set a single motion dimension
  void setMotionDimension(double m, MotionDimension d);

  // get the motion vector
  double* const getMotion();

  // get a single motion dimension
  double getMotionDimension(MotionDimension md);

 private:

  // motion vector
  Motion motion;
};

#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/
