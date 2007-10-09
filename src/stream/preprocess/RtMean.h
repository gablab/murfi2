/******************************************************************************
 * RtMean.h is the header for a class that computes the mean over a
 * set of images. incrementally
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#ifndef RTMEAN_H
#define RTMEAN_H

#include"RtStreamComponent.h"
#include"RtDataImage.h"

// class declaration
class RtMean : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtMean();

  // destructor
  ~RtMean();

protected:

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  unsigned int numTimePoints;

  // saves the mean image up to t-1
  RtMRIImage storemean;
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


