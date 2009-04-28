/******************************************************************************
 * RtProcessor.h is the header for a class for a processing stream
 * component for a real-time fMRI.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTPROCESSOR_H
#define RTPROCESSOR_H

#include"RtStreamComponent.h"
#include"RtData.h"
#include"RtPasser.h"

// class declaration
class RtProcessor : public RtStreamComponent, public RtStream {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtProcessor();

  // destructor
  ~RtProcessor();

  //*** initialization routines  ***//

  // initialize stream and prepare to run
  //  out:
  //   true (for success) or false
  bool configure(RtConfig &config);

  // adds all modules to the stream
  //  in
  //   config: configuration info
  bool addModules(RtConfig &config);

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString() const;

  

protected:

  // pure virtual for validation of component configuration
  bool validateComponentConfig();

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


