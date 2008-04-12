/******************************************************************************
 * RtStream.cpp defines a class that controls execution of a single data
 * processing stream during a real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtStream.h"
#include"RtCode.h"
#include"RtPreprocessor.h"
#include"RtAnalysor.h"
#include"RtPostprocessor.h"

#include"RtDataIDs.h"

// default constructor
RtStream::RtStream() : super() {
  
}

// destructor
RtStream::~RtStream() {
  close();
}


//*** initialization routines  ***//

// set the conductor for this stream
//  in
//   conductor pointer
void RtStream::setConductor(RtConductor *_conductor) {
  conductor = _conductor;
}
  

// initialize stream and prepare to run
//  out:
//   true (for success) or false
int RtStream::configure(RtConfig &config) {
  
  Module *head = 0, *tail = 0;

  // create the head and tail by passing them up to the superclass
  ACE_NEW_RETURN(tail, Module(ACE_TEXT("end module"), new RtEndTask()), -1);
  super::open(NULL, head, tail);

  return addModules(config);
}

// exhaustive list of individual processing modules
#include"RtDiff.h"
#include"RtMean.h"
#include"RtVar.h"
#include"RtIntensityNorm.h"
#include"RtPasser.h"
#include"RtImageZScore.h"
#include"RtImageSlideWinCor.h"
#include"RtAccumCor.h"
#include"RtIncrementalGLM.h"
#include"RtSingleImageCor.h"
#include"RtFluctuationMonitor.h"
#include"RtActivationSum.h"
#include"RtActivationSumDifference.h"

// add a single module to the module stack
//  in
//   type: name of the module type to add
//   out: optional output to pass the result of this module to
//   text: optional text to be associated with the module
RtStreamComponent *RtStream::addSingleModule(const string &type, 
					     const string &text) {
  Module *mod;
  RtStreamComponent *sc = NULL;

  // switch amongst module types
  // ALL MODULES MUST BE REGISTERED HERE
  // remember to modify RtDataIDs.h to add the new data id
  if(type == RtPasser::moduleString) { // for original data passer only
    ACE_NEW_NORETURN(sc, RtPasser());
  }
  else if(type == RtDiff::moduleString) { // voxel time difference
    ACE_NEW_NORETURN(sc, RtDiff());
  }
  else if(type == RtMean::moduleString) { // voxel time mean
    ACE_NEW_NORETURN(sc, RtMean());
  }
  else if(type == RtVar::moduleString) { // voxel time variance
    ACE_NEW_NORETURN(sc, RtVar());
  }
  else if(type == RtIntensityNorm::moduleString) { // intensity normalization
    ACE_NEW_NORETURN(sc, RtIntensityNorm());
  }
  else if(type == RtImageZScore::moduleString) { // voxel z score
    ACE_NEW_NORETURN(sc, RtImageZScore());
  }
  else if(type == RtImageSlideWinCor::moduleString) { // voxel sliding window correlation
    ACE_NEW_NORETURN(sc, RtImageSlideWinCor());
  }
  else if(type == RtAccumCor::moduleString) { // voxel accumulative correlation
    ACE_NEW_NORETURN(sc, RtAccumCor());
  }
  else if(type == RtIncrementalGLM::moduleString) { // voxel incremental GLM
    ACE_NEW_NORETURN(sc, RtIncrementalGLM());
  }
  else if(type == RtSingleImageCor::moduleString) { // voxel single image correlation
    ACE_NEW_NORETURN(sc, RtSingleImageCor());
  }
  else if(type == RtFluctuationMonitor::moduleString) { // fluctuation monitor
    ACE_NEW_NORETURN(sc, RtFluctuationMonitor());
  }
  else if(type == RtActivationSum::moduleString) { // sum image activation
    ACE_NEW_NORETURN(sc, RtActivationSum());
  }
  else if(type == RtActivationSumDifference::moduleString) { // activation sum diff
    ACE_NEW_NORETURN(sc, RtActivationSumDifference());
  }

  // create and add the module
  ACE_NEW_NORETURN(mod, Module(ACE_TEXT(text.c_str()),sc));
  addMod.push(mod);

  return sc;
}

// adds modules to the stream
//  in
//   config: configuration info
int RtStream::addModules(RtConfig &config) {
  ACE_TRACE(("RtStream::addModules"));

  // add the preprocessing module
  Module *preprocMod;
  RtPreprocessor *preproc = new RtPreprocessor();
  ACE_NEW_RETURN(preprocMod, Module(ACE_TEXT("preprocessing module"),
				    preproc),-1);

  preproc->setConductor(conductor);
  preproc->configure(config);

  // add the analysis module
//  Module *analysMod;
//  RtAnalysor *analys = new RtAnalysor();
//  ACE_NEW_RETURN(analysMod, Module("analysis module", analys), -1);
//  analys->configure(config);
//  
//  // add the postprocessing module
//  Module *postprocMod;
//  RtPostprocessor *postproc = new RtPostprocessor();
//  ACE_NEW_RETURN(postprocMod, Module("postprocessing module", postproc), -1);
//  postproc->configure(config);
//  
//  
//  // push the modules into the processing queue
//  if(this->push(postprocMod) == -1) {
//    ACE_ERROR_RETURN((LM_ERROR, 
//		     ACE_TEXT("failed to add postprocessor to stream\n")),-1);
//  }
//
//  if(this->push(analysMod) == -1) {
//    ACE_ERROR_RETURN((LM_ERROR, 
//		     ACE_TEXT("failed to add analysis to stream\n")),-1);
//  }
//
  if(this->push(preprocMod) == -1) {
    ACE_ERROR_RETURN((LM_ERROR, 
		     ACE_TEXT("failed to add preprocessor\n")),-1);
  }

  return 0;
}

// adds all 'module' nodes that are children of the passed node as modules
// for the stream
//  in
//   elmt: xml element
void RtStream::addModulesFromNode(TiXmlElement *elmt, RtConfig *config) {
  string name;
  RtStreamComponent *sc;
  TiXmlElement *childElmt;

  // build the outputs at the top level
  vector<string> topLevelOutNames;
  buildOutputNames(elmt,topLevelOutNames);
  if(!topLevelOutNames.empty()) {
    sc = addSingleModule(RtPasser::moduleString);
    addOutputsToComponent(sc,topLevelOutNames);
  }


  TiXmlNode *child = 0;
  while((child = elmt->IterateChildren("module", child))) {
    if(child->Type() != TiXmlNode::ELEMENT) continue;

    childElmt = (TiXmlElement*) child;

    // send
    if(TIXML_SUCCESS == childElmt->QueryValueAttribute("name", &name)) {
      // add the component
      sc = addSingleModule(name);
      if(sc == NULL) {
	cout << "failed to add module " << name << endl;
	continue;
      }

      cout << "added module " << sc->getID()  << " == " << name << endl;

      // allow the stream component to configure itself
      sc->init(childElmt, config);

      // build the outputs
      vector<string> outNames;
      buildOutputNames(childElmt,outNames);
      addOutputsToComponent(sc,outNames);
    }
  }

}


// adds outputs to a stream component (needs to be here so that we have
// access to the conductor to get pointers to the outputs)
//  in 
//   sc: stream component to add the outputs to
void RtStream::addOutputsToComponent(RtStreamComponent *sc, 
				    vector<string> &outNames) {
  for(vector<string>::iterator i = outNames.begin(); i != outNames.end(); i++) {
    //cout<< "adding output " << *i << " to " << sc->getID() << endl;
    sc->addOutput(conductor->getOutputByName(*i));
  }
}


// build a vector of ids of outputs from an xml node's children
//  in
//   module element
//  out 
//   vector of string output ids
void RtStream::buildOutputNames(TiXmlElement *module, vector<string> &names) {
  TiXmlNode *child = 0;
  while((child = module->IterateChildren("output", child))) {
    if(child->Type() != TiXmlNode::ELEMENT) continue;

    // extract the text from this element
    // CONSIDER USING CHILD ITERATORS HERE
    string outid = "output:";
    outid += ((TiXmlElement*)child)->GetText();
    names.push_back(outid);
  }  
}

// accept new data received from an input
//  in
//   data: data 
void RtStream::setInput(unsigned int code, RtData *data) {
  ACE_TRACE(("RtStream::setInput"));
  
  // take action based on the kind of new input
  switch(code) {
  case SCANNER_IMAGE_RECEIVED:

    // if this code is from a new scanner image, start the processing
    ACE_Message_Block *mb;
    ACE_NEW_NORETURN(mb, ACE_Message_Block(sizeof(RtStreamMessage)));


    RtStreamMessage *msg;
    ACE_NEW_NORETURN(msg, RtStreamMessage());

    msg->init(conductor);
    msg->addData(data);

    memcpy(mb->wr_ptr(), msg, sizeof(RtStreamMessage));
    mb->wr_ptr(sizeof(RtStreamMessage));
    
    //cerr << "got data " << data << " and put it in a msg " << msg << endl;

    // RtStreamMessage *m = (RtStreamMessage*) mb->rd_ptr();
    //cerr << "retrieve data from msg gives " << m->getCurrentData() << endl;

    this->put(mb);

    break;
  }
}


// adds all the modules on the stack to the processing stream
// the module stack is emtpty after this executes
//  out
//   -1 for error, 0 on success
int RtStream::pushAllModules() {
  ACE_TRACE(("RtStream::pushAllModules"));

  // add all the modules from the stack
  while(!addMod.empty()) {
    if(this->push(addMod.top()) == -1) {
      ACE_ERROR_RETURN((LM_ERROR, ACE_TEXT("failed to add a module\n")),-1);
    }
    
    addMod.pop();
  }
  
  return 0;
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


