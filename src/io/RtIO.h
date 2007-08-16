/******************************************************************************
 * RtIO.h declares a base class for input and output operations 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTIO_H
#define RTIO_H


// class declaration
class RtIO {

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
  virtual bool open() = 0;

  // closes an io object
  //  out:
  //   true (for success) or false
  virtual bool close() = 0;
  
private:


  //*** private data members  ***//

  bool open;

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


