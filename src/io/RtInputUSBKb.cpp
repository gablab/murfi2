/******************************************************************************
 * RtInputUSB.cpp defines a class that implements usb keyboard input
 * operations
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtInputUSBKb.h"

//*** constructors/destructors  ***//

// default constructor
RtInputUSBKb::RtInputUSBKb() {
  id += ":usbkb";
}

// destructor
RtInputUSBKb::~RtInputUSBKb() {

}

// open and start accepting input
bool RtInputUSBKb::init(const RtConfig &config) {
  if(!RtInputUSBKb::init(config)) {
    return false;
  }

  return true;
}

// close and clean up
bool RtInputUSBKb::close() {
  return true;
}

// gets the version
//  out:
//   cvs version string for this class
char *RtInputUSBKb::getVersionString() {
  return VERSION;
}


/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


