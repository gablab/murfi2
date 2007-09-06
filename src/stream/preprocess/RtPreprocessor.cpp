/******************************************************************************
 * RtPreprocessor.cpp is the definition of a class for a preprocessing stream
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtPreprocessor.h"

// default constructor
RtPreprocessor::RtPreprocessor() 
  : RtStreamComponent() {
}

// destructor
RtPreprocessor::~RtPreprocessor() {

}

//*** initialization routines  ***//

// add modules
//  in
//   config
//  out
//   success failure
int RtPreprocessor::addModules(RtConfig &config) {
  // build the list of stream components specified in the config 

  return 0;
}

// process a single acquisition
int RtPreprocessor::process(ACE_Message_Block *mb) {
  cerr << "preprocessing" << endl;

  return 0;
}

// get the version
//  out: char array that represents the cvs version
char *RtPreprocessor::getVersionString() {
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


