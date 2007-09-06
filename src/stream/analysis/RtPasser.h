/******************************************************************************
 * RtPasser.h is the header for a class that passes data from an
 * analysis stream to output
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
  RtPasser() : RtStreamComponent() {}

  // destructor
  ~RtPasser() {}

  //*** initialization routines  ***//
  void addOutput(RtOutput *out) {
    outputs.push_back(out);
  }

protected:

  // process a single acquisition
  int process(ACE_Message_Block *mb) {
    RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

    for(vector<RtOutput*>::iterator i = outputs.begin(); i != outputs.end(); i++) {
      (*i)->setData(msg->data);
    }

    return 0;
  }

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


