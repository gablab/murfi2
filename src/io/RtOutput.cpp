/******************************************************************************
 * RtOutput.cpp defines a class that implements output operations 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtOutput.h"

// default constructor
RtOutput::RtOutput() {

} 

// destructor
RtOutput::~RtOutput() {

}

// init
bool RtOutput::open(const RtConfig &config) {
  if(!RtIO::open(config)) {
    return false;
  }

  return true;
}

// gets the version
//  out:
//   cvs version string for this class
char *RtOutput::getVersionString() {
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


