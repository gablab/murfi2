/******************************************************************************
 * RtStream.cpp defines a class that controls execution of a single data
 * processing stream during a real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtStream.h"

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
bool RtStream::open(RtConfig &config) {

  // create the head and tail by passing them up to the superclass
  ACE_NEW_RETURN(tail, Module(ACE_TEXT("end module"), new EndTask()), -1);
  super::open(NULL, head, tail);

  // add the preprocessing module
  Module *preprocess;
  ACE_NEW_RETURN(preprocess, Module("preprocessing module", 
				    new Preprocessor(config)), -1);
  
  // add the analysis module
  Module *analysis;
  ACE_NEW_RETURN(analysis, Module("analysis module", 
				    new Analysor(config)), -1);
  
  // add the postprocessing module
  Module *postprocess;
  ACE_NEW_RETURN(postprocess, Module("postprocessing module", 
				    new Postprocessor(config)), -1);
  
  
  // push the modules into the processing queue
  if(this->push(postprocess) == -1) {
    ACE_ERROR_RETURN(LM_ERROR, 
		     ACE_TEXT("failed to add postprocessor to stream\n"));
  }

  if(this->push(analysis) == -1) {
    ACE_ERROR_RETURN(LM_ERROR, 
		     ACE_TEXT("failed to add analysis to stream\n"));
  }

  if(this->push(preprocess) == -1) {
    ACE_ERROR_RETURN(LM_ERROR, 
		     ACE_TEXT("failed to add preprocessor\n"));
  }


  return true;
}

// accept new data received from an input
//  in
//   data: data 
void RtStream::setInput(unsigned int code, RtData *data) {
  
  // take action based on the kind of new input
  switch(code) {
  case SCANNER_INPUT_RECEIVED:
    // if this code is from a new scanner image, start the processing
    ACE_Message_Block *mb;
    ACE_NEW_RETURN(mb, ACE_Message_Block(sizeof(RtStreamMessage)), -1);

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
bool RtStream::run(RtData &data) {

  return true;
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


