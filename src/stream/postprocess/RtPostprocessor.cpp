/******************************************************************************
 * RtPostprocessor.cpp is the definition of a class for a postprocessing stream
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtPostprocessor.h"

// default constructor
RtPostprocessor::RtPostprocessor() 
  : RtStreamComponent() {
}

// destructor
RtPostprocessor::~RtPostprocessor() {

}

//*** initialization routines  ***//

// add modules
//  in
//   config
//  out
//   success failure
int RtPostprocessor::addModules(RtConfig &config) {
  ACE_TRACE(("RtPostprocessor::addModules"));

  // build the list of stream components specified in the config 

  return 0;
}


// process a single acquisition
int RtPostprocessor::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtPostprocessor::process"));

  return 0;
}

// get the version
//  out: char array that repostsents the cvs version
char *RtPostprocessor::getVersionString() {
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


