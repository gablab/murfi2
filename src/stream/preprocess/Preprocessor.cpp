/******************************************************************************
 * RtPreprocessor.cpp is the definition of a class for a preprocessing stream
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtPreProcessor.h"

// default constructor
RtPreprocessor::RtPreprocessor() : RtStream(), RtStreamComponent() {

}

// destructor
RtPreprocessor::~RtPreprocessor() {

}

//*** initialization routines  ***//

// initialize components
//  out:
//    success failure
bool RtPreprocessor::open(RtConfig &config) {
  // build the list of stream components specified in the config 
}

// get the version
//  out: char array that represents the cvs version
virtual char *RtPreprocessor::getVersionString() {
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


