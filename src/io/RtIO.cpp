/******************************************************************************
 * RtIO.cpp defines a base class for input and output operations 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/
static char *VERSION = "$Id$";

#include<RtIO.h>

//*** constructors/destructors  ***//

// default constructor
RtIO::RtIO() : isOpen(false) {

}

// destructor
RtIO::~RtIO() {
}

// open and start accepting input
bool RtIO::open(const RtConfig &config) {
  isOpen = true;

  return true;
}

// close and clean up
bool RtIO::close() {
  isOpen = false;

  return true;
}


// gets the version
//  out:
//   cvs version string for this class
char *RtIO::getVersionString() {
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


