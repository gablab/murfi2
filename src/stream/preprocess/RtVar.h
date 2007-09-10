/******************************************************************************
 * RtVar.h is the header for a class that computes the variance over a
 * set of images
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#ifndef RTVAR_H
#define RTVAR_H

#include"RtStreamComponent.h"
#include"RtDataImage.h"

// class declaration
class RtVar : public RtStreamComponent {

public:

  //*** constructors/destructors  ***//

  // default constructor
  RtVar();

  // destructor
  ~RtVar();

protected:

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  unsigned int numTimePoints;

  // saves the mean image up to t-1
  RtDataImage mean;
  vector<RtDataImage*> hist;

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


