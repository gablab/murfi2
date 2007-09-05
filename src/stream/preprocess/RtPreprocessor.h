/******************************************************************************
 * RtPreprocessor.h is the header for a class for a processing stream
 * component for a real-time fMRI.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTPREPROCESSOR_H
#define RTPREPROCESSOR_H

#include"RtStreamComponent.h"
#include"RtData.h"

// class declaration
class RtPreprocessor : public RtStreamComponent, public RtStream {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtPreprocessor();

  // destructor
  ~RtPreprocessor();

  //*** initialization routines  ***//

  // adds all modules to the stream
  //  in
  //   config: configuration info
  int addModules(RtConfig &config);

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();


protected:

  // process a single acquisition
  int process(RtStreamMessage *msg);

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


