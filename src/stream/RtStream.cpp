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
  
// adds a component to the processing pipeline
//  in:
//   component: component object
//  out:
//   true (for success) or false
//bool RtStream::addComponent(RtStreamComponent &component) {
//
//  return true;
//}


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

// adds modules to the stream
//  in
//   config: configuration info
int RtStream::addModules(RtConfig &config) {
  // add the preprocessing module
  Module *preprocMod;
  RtPreprocessor *preproc = new RtPreprocessor();
  ACE_NEW_RETURN(preprocMod, Module(ACE_TEXT("preprocessing module"),
				    preproc),-1);
  preproc->configure(config);

  // add the analysis module
  Module *analysMod;
  RtAnalysor *analys = new RtAnalysor();
  ACE_NEW_RETURN(analysMod, Module("analysis module", analys), -1);
  analys->configure(config);
  
  // add the postprocessing module
  Module *postprocMod;
  RtPostprocessor *postproc = new RtPostprocessor();
  ACE_NEW_RETURN(postprocMod, Module("postprocessing module", postproc), -1);
  postproc->configure(config);
  
  
  // push the modules into the processing queue
  if(this->push(postprocMod) == -1) {
    ACE_ERROR_RETURN((LM_ERROR, 
		     ACE_TEXT("failed to add postprocessor to stream\n")),-1);
  }

  if(this->push(analysMod) == -1) {
    ACE_ERROR_RETURN((LM_ERROR, 
		     ACE_TEXT("failed to add analysis to stream\n")),-1);
  }

  if(this->push(preprocMod) == -1) {
    ACE_ERROR_RETURN((LM_ERROR, 
		     ACE_TEXT("failed to add preprocessor\n")),-1);
  }

  return 0;
}

// accept new data received from an input
//  in
//   data: data 
void RtStream::setInput(unsigned int code, RtData *data) {
  
  // take action based on the kind of new input
  switch(code) {
  case SCANNER_IMAGE_RECEIVED:
    // if this code is from a new scanner image, start the processing
    ACE_Message_Block *mb;
    ACE_NEW_NORETURN(mb, ACE_Message_Block(sizeof(RtStreamMessage)));

    RtStreamMessage *msg = (RtStreamMessage*) mb->wr_ptr();
    mb->wr_ptr(sizeof(RtStreamMessage));

    msg->conductor = conductor;
    msg->data = data;

    ACE_DEBUG((LM_DEBUG, ACE_TEXT("starting stream for new data\n")));

    this->put(mb);

    break;
  }
}


//*** operation routines  ***//
  
// begins stream processing
//  out:
//   true (for success) or false
//bool RtStream::run(RtData &data) {
//
//  return true;
//}


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


