/******************************************************************************
 * RtStream.cpp defines a class that controls execution of a single data
 * processing stream during a real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtStream.h"

// default constructor
RtStream::RtStream() {

}

// destructor
RtStream::~RtStream() {
  close();
}


//*** initialization routines  ***//
  
// adds a component to the processing pipeline
//  in:
//   component: component object
//  out:
//   true (for success) or false
bool RtStream::addComponent(RtStreamComponent &component) {

  return true;
}


// initialize stream and prepare to run
//  out:
//   true (for success) or false
bool RtStream::open(RtConfig &config) {

  return true;
}

// accept new data received from an input
//  in
//   data: data 
void RtStream::setInput(unsigned int code, RtData *data) {

}


//*** operation routines  ***//
  
// begins stream processing
//  out:
//   true (for success) or false
bool RtStream::run(RtData &data) {

  return true;
}


// gets the version
//  out:
//   cvs version string for this class
char *RtStream::getVersionString() {
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


