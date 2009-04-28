/******************************************************************************
 * RtDiff.h is the header for a class that computes the difference
 * between two images
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#ifndef RTDIFF_H
#define RTDIFF_H

#include"RtStreamComponent.h"
#include"RtMRIImage.h"

// class declaration
class RtDiff : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtDiff();

  // destructor
  ~RtDiff();

protected:

  // validate the configuration
  bool validateComponentConfig();

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // pointer to the last data we got 
  RtMRIImage *last;

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


