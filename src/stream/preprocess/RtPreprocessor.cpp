/******************************************************************************
 * RtPreprocessor.cpp is the definition of a class for a preprocessing stream
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtPreprocessor.h"
#include"RtDataIDs.h"
#include"RtDiff.h"
#include"RtVar.h"

#include"tinyxml/tinyxml.h"

// default constructor
RtPreprocessor::RtPreprocessor() 
  : RtStreamComponent() {
  id = "RtPreprocessor";
}

// destructor
RtPreprocessor::~RtPreprocessor() {
  
}

//*** initialization routines  ***//

// add modules based on the preprocessor configuration
//  in
//   config
//  out
//   success or failure
int RtPreprocessor::addModules(RtConfig &config) {
  ACE_TRACE(("RtPreprocessor::addModules"));

  // make sure the stack is empty
  while(!addMod.empty()) {
    addMod.pop();
  }

  // get the node for preprocessing 
  TiXmlNode *preprocNode = config.getNode("preprocessor");


  // if no preprocessing was specified, just pass the data
  if(preprocNode == NULL || preprocNode->Type() != TiXmlNode::ELEMENT) {
    addSingleModule(RtPasser::moduleString, "original data passer")
      ->addOutput(config.getConductor()->getDisplay());
  }
  else { // find modules in the node and add each
    addModulesFromNode((TiXmlElement*) preprocNode);
  }
    // all modules are now added to the stream
  pushAllModules();


  return 0;
}

// process a single acquisition
int RtPreprocessor::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtPreprocessor::process"));


  // pass the message block down to our stream
  RtStream::put(mb);
  

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


