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
  inputID = "input";
}

// destructor
RtInput::~RtInput() {
  inputID = "input";
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

// set the id string
void RtInput::setID(const string &s) {
  inputID = s;
}

// add a string to the input id
void RtInput::addToID(string add) {
  inputID += add;
}

// get the id string
string RtInput::getID() {
  return inputID;
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


