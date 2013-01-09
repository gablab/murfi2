/******************************************************************************
 * RtLocker.h declares a base class for data locker classes
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-10
 *
 *****************************************************************************/

#if 0

#ifndef RTLOCKER_H
#define RTLOCKER_H


// class declaration
class RtLocker {

 public:

  //*** constructors/destructors  ***//

  // default constructor
  RtLocker();

  // destructor
  virtual ~RtLocker();

  // called when locked data is destructed
  virtual void beingDeleted() {}

 protected:


};

#endif

#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/
