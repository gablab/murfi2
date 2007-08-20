/******************************************************************************
 * RtOutput.h declares a class that implements output operations 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTOUTPUT_H
#define RTOUTPUT_H

#include"RtIO.h"
#include"RtConfig.h"

// class declaration
class RtOutput : public RtIO {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtOutput(); 

  // destructor
  virtual ~RtOutput();


  // open and start accepting input
  virtual bool open(const RtConfig &config);

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

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


