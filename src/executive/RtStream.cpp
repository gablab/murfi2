/******************************************************************************
 * RtStream.cpp defines a class that controls execution of a single data
 * processing stream during a real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#include"RtStream.h"

// default constructor
RtStream::RtStream() {

}

// destructor
RtStream::~RtStream() {

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
bool RtStream::init() {

  return true;
}


//*** operation routines  ***//
  
// begins stream processing
//  out:
//   true (for success) or false
bool RtStream::run(RtData &data) {

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


