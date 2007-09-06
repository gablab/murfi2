/******************************************************************************
 * RtPostprocessor.h is the header for a class for a processing stream
 * component for a real-time fMRI.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTPOSTPROCESSOR_H
#define RTPOSTPROCESSOR_H

#include"RtStreamComponent.h"
#include"RtData.h"

// class declaration
class RtPostprocessor : public RtStreamComponent, public RtStream {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtPostprocessor();

  // destructor
  ~RtPostprocessor();

  //*** initialization routines  ***//

  // adds all modules to the stream
  //  in
  //   config: configuration info
  int addModules(RtConfig &config);

  // get the version
  //  out: char array that repostsents the cvs version
  virtual char *getVersionString();


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


