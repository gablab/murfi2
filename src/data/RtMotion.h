/******************************************************************************
 * RtMotion.h declares a class to store subject head motion
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-10-08
 *
 *****************************************************************************/

#ifndef RTMOTION_H
#define RTMOTION_H

#include"RtData.h"
#include"tinyxml/tinyxml.h"

static const unsigned int NUM_MOTION_DIMENSIONS = 6;
typedef enum {
  TRANSLATION_X = 0,
  TRANSLATION_Y,
  TRANSLATION_Z,
  ROTATION_X,  
  ROTATION_Y,  
  ROTATION_Z  
} MotionDimension; 
typedef double Motion[NUM_MOTION_DIMENSIONS];

// class declaration
class RtMotion : public RtData {

public:

  //*** constructors/destructors  ***//

  // default constructor
  RtMotion();

  // construct with motion
  RtMotion(double tx, double ty, double tz, double rx, double ry, double rz);

  // destructor
  virtual ~RtMotion();

  // serialize as xml (dummy function)
  virtual TiXmlElement* serializeAsXML();

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


