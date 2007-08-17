/******************************************************************************
 * RtInputUSB.cpp defines a class that implements usb keyboard input
 * operations
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#include"RtInputUSBKb.h"

//*** constructors/destructors  ***//

// default constructor
RtInputUSBKb::RtInputUSBKb() {

}

// destructor
RtInputUSBKb::~RtInputUSBKb() {

}

// open and start accepting input
bool RtInputUSBKb::open() {
  return true;
}

// close and clean up
bool RtInputUSBKb::close() {
  return true;
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


