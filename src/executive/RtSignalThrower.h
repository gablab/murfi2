/******************************************************************************
 * RtSignalThrower.h is the header for an abstract class that 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTSIGNALTHROWER_H
#define RTSIGNALTHROWER_H

#include"RtStreamComponent.h"

class RtSignalThrower {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtSignalThrower() {
    sigNum = SIGUNUSED;
  }
  
  // constructor w/ signum
  RtSignalThrower(int _sigNum) : sigNum(_sigNum) {

  }

  // destructor
  virtual ~RtSignalThrower() {

  }

  // set the signal number to throw
  void setSigNum(int _sigNum) {
    sigNum = _sigNum;
  }

  // get the signal number
  int getSigNum() {
    return sigNum;
  }

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString() {
    return "$Id$";
  }

protected:

  //*** private data members  ***//

  // signal number to be thrown
  int sigNum;

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


