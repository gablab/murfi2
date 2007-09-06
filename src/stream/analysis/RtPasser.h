/******************************************************************************
 * RtPasser.h is the header for a class that passes data from an
 * analysis stream to an output
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#ifndef RTPASSER_H
#define RTPASSER_H

#include"RtStreamComponent.h"
#include"RtData.h"

// class declaration
class RtPasser : public RtStreamComponent {

public:

  //*** constructors/destructors  ***//

  // default constructor
  RtPasser();

  // destructor
  ~RtPasser();

  //*** initialization routines  ***//

protected:

  // process a single acquisition
  int process(RtStreamMessage *msg);

  // vector of outputs to pass the data to
  vector<RtOutput*> outputs;

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


