/******************************************************************************
 * RtMotion.cpp defines a class to store subject motion
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-12-27
 *
 *****************************************************************************/

#include"RtMotion.h"

//*** constructors/destructors  ***//

RtMotion::RtMotion() : RtData() {
  ACE_TRACE(("RtMRIImage::RtMotion()"));

  dataID.setModuleID("motion");

  motion[TRANSLATION_X] = motion[TRANSLATION_Y] = motion[TRANSLATION_Z] =
      motion[ROTATION_X] = motion[ROTATION_Y] = motion[ROTATION_Z] = 0.0;
}

// construct with motion
RtMotion::RtMotion(double tx, double ty, double tz,
                   double rx, double ry, double rz) {
  ACE_TRACE(("RtMRIImage::RtMotion()"));

  dataID.setModuleID("motion");

  setMotion(tx, ty, tz, rx, ry, rz);
}

RtMotion::~RtMotion() {
}


// set the motion vector
void RtMotion::setMotion(Motion _motion) {
  memcpy(motion, _motion, NUM_MOTION_DIMENSIONS * sizeof (double));
}

// set the motion vector
void RtMotion::setMotion(double tx, double ty, double tz,
                         double rx, double ry, double rz) {
  motion[TRANSLATION_X] = tx;
  motion[TRANSLATION_Y] = ty;
  motion[TRANSLATION_Z] = tz;
  motion[ROTATION_X] = rx;
  motion[ROTATION_Y] = ry;
  motion[ROTATION_Z] = rz;
}

// set a single motion dimension
void RtMotion::setMotionDimension(double m, MotionDimension d) {
  if (d < 0 || d >= NUM_MOTION_DIMENSIONS) {
    return;
  }

  motion[d] = m;
}

// get the motion vector
double* const RtMotion::getMotion() {
  return motion;
}

// get a single motion dimension
double RtMotion::getMotionDimension(MotionDimension d) {
  if (d < 0 || d >= NUM_MOTION_DIMENSIONS) {
    return 0.0;
  }

  return motion[d];
}

// serialize as xml (dummy function)
TiXmlElement* RtMotion::serializeAsXML(TiXmlElement *requestElement) {
  return new TiXmlElement("motion");
}

// unserialize xml (dummy function)

void RtMotion::unserializeXML(TiXmlElement* element) {

}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/
