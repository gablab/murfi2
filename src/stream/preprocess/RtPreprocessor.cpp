/******************************************************************************
 * RtPreprocessor.cpp is the definition of a class for a preprocessing stream
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtPreprocessor.h"
#include"RtDiff.h"
#include"RtVar.h"

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


  // make sure the stack is empty
  while(!addMod.empty()) {
    addMod.pop();
  }

  RtOutput* display = config.getConductor()->getDisplay();

//  // add a passer module for the original data
//  Module *passerMod;
//  RtPasser *passer = new RtPasser();
//  ACE_NEW_RETURN(passerMod, Module(ACE_TEXT("original data passer module"),
//				   passer),-1);
//  addMod.push(passerMod);
//  
//  if(display != NULL) {
//    passer->addOutput(display);
//  }
  

  // add a module to compute the difference between the last two acquisitions
  Module *diffMod;
  RtDiff *diff = new RtDiff();
  ACE_NEW_RETURN(diffMod, Module(ACE_TEXT("image difference module"),
				   diff),-1);
  diff->setPersistent(false);
  addMod.push(diffMod);


  // add a passer module for the differenced data
  Module *diffPasserMod;
  RtPasser *diffPasser = new RtPasser();
  ACE_NEW_RETURN(diffPasserMod, Module(ACE_TEXT("diff data passer module"),
				   diffPasser),-1);
  addMod.push(diffPasserMod);
  
  if(display != NULL) {
    diffPasser->addOutput(display);
  }

  // add a module to compute the variance over the acquisisions
  Module *varMod;
  RtVar *var = new RtVar();
  ACE_NEW_RETURN(varMod, Module(ACE_TEXT("image variance module"),
				   var),-1);
  var->setPersistent(false);
  addMod.push(varMod);


  // add a passer module for the variance
//  Module *varPasserMod;
//  RtPasser *varPasser = new RtPasser();
//  ACE_NEW_RETURN(varPasserMod, Module(ACE_TEXT("var data passer module"),
//				   varPasser),-1);
//  addMod.push(varPasserMod);
//  
//  if(display != NULL) {
//    varPasser->addOutput(display);
//  }
//

  // all modules are now added to the stream
  pushAllModules();


  return 0;
}

// process a single acquisition
int RtPreprocessor::process(ACE_Message_Block *mb) {
  ACE_TRACE((LM_TRACE, "RtPreprocessor::process"));


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


