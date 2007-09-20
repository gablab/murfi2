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
  id = "input";
}

// destructor
RtInput::~RtInput() {
  id = "input";
}

// init
bool RtInput::open(RtConfig &config) {
  // set up the callback
  
  return true;
}


// closes both the io object and task base
//  out:
//   true (for success) or false
bool RtInput::close() {

  // close super classes
  ACE_Task_Base::close();

  return RtIO::close();
}


// gets the version
//  out:
//   cvs version string for this class
char *RtInput::getVersionString() {
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


