/******************************************************************************
 * RtAnalysor.cpp is the definition of a class for the analysis stream
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtAnalysor.h"

#include"RtPasser.h"

// default constructor
RtAnalysor::RtAnalysor() 
  : RtStreamComponent() {
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
  ACE_TRACE(("RtAnalysor::addModules"));

  // build the list of stream components specified in the config 

  // add a passer module for the preprocessed data
//  Module *passerMod;
//  RtPasser *passer = new RtPasser();
//  ACE_NEW_RETURN(passerMod, Module(ACE_TEXT("preprocessed data passer module"),
//				   passer),-1);
//  RtOutput* display = config.getConductor()->getDisplay();
//  if(display != NULL) {
//    passer->addOutput(display);
//  }
  
  
//  if(this->push(passerMod) == -1) {
//    ACE_ERROR_RETURN((LM_ERROR, 
//		     ACE_TEXT("failed to add passer\n")),-1);
//  }


  return 0;
}

// process a single acquisition
int RtAnalysor::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtAnalysor::process"));

  // pass the message block down to our stream
  RtStream::put(mb);

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


