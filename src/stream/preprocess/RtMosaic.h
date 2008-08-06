/******************************************************************************
 * RtMosaic.h is the header for a class that converts a volume to a flat
 * mosaic represtation
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-08-04
 *
 *****************************************************************************/

#ifndef RTMOSAIC_H
#define RTMOSAIC_H

#include"RtStreamComponent.h"
#include"RtDataImage.h"

// class declaration
class RtMosaic : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtMosaic();

  // destructor
  ~RtMosaic();

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


