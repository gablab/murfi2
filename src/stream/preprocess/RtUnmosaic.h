/******************************************************************************
 * RtUnmosaic.h is the header for a class that converts from a flat mosaiced
 * representation to a 3D volume
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-08-04
 *
 *****************************************************************************/

#ifndef RTUNMOSAIC_H
#define RTUNMOSAIC_H

#include"RtStreamComponent.h"
#include"RtDataImage.h"

// class declaration
class RtUnmosaic : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtUnmosaic();

  // destructor
  ~RtUnmosaic();

protected:

  // process a single acquisition
  int process(ACE_Message_Block *mb);

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


