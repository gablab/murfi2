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

// add a single module to the module stack
//  in
//   type: name of the module type to add
//   out: optional output to pass the result of this module to
//   text: optional text to be associated with the module
void RtPreprocessor::addSingleModule(const string &type, 
				     const RtOutput *out, 
				     const string &text) {
  Module *mod;
  RtStreamComponent *sc = NULL;

  // switch amongst module types
  // ALL NEW MODULES MUST BE REGISTERED HERE
  if(type == RtPasser::moduleString) { // for original data passer only
    // empty
  }
  else if(type == RtDiff::moduleString) { // voxel time difference
    ACE_NEW_NORETURN(sc, RtDiff());
  }
  else if(type == RtVar::moduleString) { // voxel time variance
    ACE_NEW_NORETURN(sc, RtVar());
  }

  // create and add the module
  ACE_NEW_NORETURN(mod, Module(ACE_TEXT(text),sc));
  addMod.push(mod);
      
  // add the output
  if(out != NULL) {
    ACE_NEW_NORETURN(sc, RtPasser(sc == NULL 
				  ? ID_SCANNERIMG : sc->moduleString));
    sc->addOutput(out);
    ACE_NEW_NORETURN(mod, Module(ACE_TEXT(text),sc));
    addMod.push(mod);
  }  
}

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

  RtOutput* display = config.getConductor()->getDisplay();

  // get the node for preprocessing 
  TiXmlNode *preprocNode = config.getNode("preprocessor");
  TiXmlElement *childElmt;

  // if no preprocessing was specified, just pass the data
  if(preprocNode == NULL || preprocNode->Type() != TiXmlNode::ELEMENT) {

  }
  else { // iterate over modules, adding them in order
    string pass, name;
    bool val;
    TiXmlElement *preprocElmt = (TiXmlElement*) preprocNode;

    // if the passer attribute is set to true for the preproc, add it
    if(TIXML_SUCCESS == preprocElmt->QueryValueAttribute("pass", &pass) 
       && RtConfigVal::convert<bool>(val,pass) && val) {
      addSingleModule(RtPasser::moduleString, display, "original data passer");
    }

    TiXmlNode *child = 0;
    while((child = preprocElmt->IterateChildren("module", child))) {
      if(child->Type() != TiXmlNode::ELEMENT) continue;

      childElmt = (TiXmlElement*) child;

      // send
      if(TIXML_SUCCESS == childElmt->QueryValueAttribute("name", &name)) {
	// send if differently if we should pass the result or not
	if(TIXML_SUCCESS == childElmt->QueryValueAttribute("pass", &pass) 
	   && RtConfigVal::convert<bool>(val,pass) && val) {
	  addSingleModule(name, display);
	}
	else
	  addSingleModule(name);
      }
    }

//     // add a passer module for the original data
//     Module *passerMod;
//     RtPasser *passer = new RtPasser(ID_SCANNERIMG);
//     addMod.push(passerMod);
  
//     if(display != NULL) {
//       passer->addOutput(display);
//     }


//     // add a module to compute the difference between the last two acquisitions
//     Module *diffMod;
//     RtDiff *diff = new RtDiff();
//     ACE_NEW_RETURN(diffMod, Module(ACE_TEXT("image difference module"),
// 				   diff),-1);
//     diff->setPersistent(false);
//     addMod.push(diffMod);


//     // add a passer module for the differenced data
//     Module *diffPasserMod;
//     RtPasser *diffPasser = new RtPasser(ID_DIFFIMG);
//     ACE_NEW_RETURN(diffPasserMod, Module(ACE_TEXT("diff data passer module"),
// 					 diffPasser),-1);
//     addMod.push(diffPasserMod);
  
//     if(display != NULL) {
//       diffPasser->addOutput(display);
//     }

//     // add a module to compute the variance over the acquisisions
//     Module *varMod;
//     RtVar *var = new RtVar();
//     ACE_NEW_RETURN(varMod, Module(ACE_TEXT("image variance module"),
// 				  var),-1);
//     var->setPersistent(false);
//     addMod.push(varMod);


//     // add a passer module for the variance
//     Module *varPasserMod;
//     RtPasser *varPasser = new RtPasser(ID_VARIMG);
//     ACE_NEW_RETURN(varPasserMod, Module(ACE_TEXT("var data passer module"),
// 					varPasser),-1);
//     addMod.push(varPasserMod);
  
//     if(display != NULL) {
//       varPasser->addOutput(display);
//     }

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


