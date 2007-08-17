/******************************************************************************
 * RtStreamComponent.h is the header for an abstract class for a
 * processing stream component for a real-time fMRI.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTSTREAMCOMPONENT_H
#define RTSTREAMCOMPONENT_H

#include"RtConfig.h"
#include"RtData.h"

// class declaration
class RtStreamComponent {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtStreamComponent(); 

  // destructor
  virtual ~RtStreamComponent();

  //*** initialization routines  ***//

  // initialize and prepare to run
  //  out:
  //   true (for success) or false
  virtual bool init() = 0;

  //*** operation routines  ***//
  
  // begins execution of a stream component
  //  out:
  //   true (for success) or false
  virtual bool run(RtData &data) = 0;

private:

  //*** private data members  ***//

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


