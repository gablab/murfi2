/******************************************************************************
 * RtInputUSB.h declares a class that implements usb keyboard input operations 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTINPUTUSBKB_H
#define RTINPUTUSBKB_H

#include"RtInput.h"

// class declaration
class RtInputUSBKb : public RtInput {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtInputUSBKb(); 

  // destructor
  virtual ~RtInputUSBKb();

  // open and start accepting input
  bool init(const RtConfig &config);

  // close and clean up
  bool close();

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


