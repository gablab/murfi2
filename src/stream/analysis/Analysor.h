/******************************************************************************
 * RtAnalysor.h is the header for a class for a processing stream
 * component for a real-time fMRI.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTANALYSOR_H
#define RTANALYSOR_H

#include"RtStreamComponent.h"
#include"RtData.h"

// class declaration
class RtAnalysor : public RtStreamComponent, public RtStream {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtAnalysor();

  // destructor
  ~RtAnalysor();

  //*** initialization routines  ***//

  // initialize components
  //  out:
  //   0 (for success) or -1 (failure)
  bool open(RtConfig &config);

  // initialize thread and run
  //  out:
  //   0 (for success) or -1 (failure)
  int open();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();


protected:

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


