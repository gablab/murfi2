/******************************************************************************
 * RtIO.h declares a base class for input and output operations 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTIO_H
#define RTIO_H

#include"RtCode.h"
#include"RtConfig.h"

// class declaration
class RtIO : public RtCode {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtIO(); 

  // destructor
  virtual ~RtIO();

  //*** initialization routines  ***//
  
  // opens an io object and initialize it
  //  out:
  //   true (for success) or false
  virtual bool open(const RtConfig &config);

  // closes an io object
  //  out:
  //   true (for success) or false
  virtual bool close();
  
  // gets the version
  //  out:
  //   cvs version string for this class
  virtual char *getVersionString();
  
protected:


  //*** private data members  ***//

  bool isOpen;

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


