/******************************************************************************
 * RtInput.cpp defines a class that implements input operations 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtInput.h"

// default constructor
RtInput::RtInput() {

}

// destructor
RtInput::~RtInput() {

}

// init
bool RtInput::open(const RtConfig &config) {
  // set up the callback
  
  return true;
}


// gets the version
//  out:
//   cvs version string for this class
char *RtInput::getVersionString() {
  return VERSION;
}

// callback to receive the input
// sigc::signal<void, RtData&> callback;
  

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


