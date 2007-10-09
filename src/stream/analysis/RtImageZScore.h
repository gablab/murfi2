/******************************************************************************
 * RtImageZScore.h is the header for a class that estimates the
 * instantaneous activation at each voxel in an image using simple z-scores
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-25
 *
 *****************************************************************************/

#ifndef RTIMAGEACTIVATION_H
#define RTIMAGEACTIVATION_H

#include"RtStreamComponent.h"
#include"RtDataImage.h"

// class declaration
class RtImageZScore : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtImageZScore();

  // destructor
  ~RtImageZScore();

protected:

  // process a single acquisition
  int process(ACE_Message_Block *mb);
  
  // threshold for z-scores
  double threshold;
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


