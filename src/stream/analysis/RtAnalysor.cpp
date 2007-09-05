/******************************************************************************
 * RtAnalysor.cpp is the definition of a class for a preprocessing stream
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtAnalysor.h"

// default constructor
RtAnalysor::RtAnalysor() 
  : RtStreamComponent(), RtStream() {
}

// destructor
RtAnalysor::~RtAnalysor() {

}

//*** initialization routines  ***//


// add modules
//  in
//   config
//  out
//   success failure
int RtAnalysor::addModules(RtConfig &config) {
  // build the list of stream components specified in the config 

  return 0;
}

// process a single acquisition
int RtAnalysor::process(RtStreamMessage *msg) {
  cerr << "analyzing" << endl;

  return 0;
}

// get the version
//  out: char array that represents the cvs version
char *RtAnalysor::getVersionString() {
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


