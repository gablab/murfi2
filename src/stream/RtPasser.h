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
  RtPasser();

  // string with id constructor
  //  in
  //   dataID is a string that constrains what type of data we send, if its
  //   empty we send all the data
  RtPasser(string _dataID);

  // char* with id constructor
  //  in
  //   dataID is a string that constrains what type of data we send, if its
  //   empty we send all the data
  RtPasser(char *_dataID);

  // destructor
  ~RtPasser();

  //*** initialization routines  ***//
  void addOutput(RtOutput *out);

protected:

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // vector of outputs to pass the data to
  vector<RtOutput*> outputs;

  // only pass data of this id
  string dataID;
  
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


