/******************************************************************************
 * Conductor.cpp defines a class that oversees and coordinates all
 * operations during a real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14
 *
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtConductor.h"
#include<iostream>

//*** constructors/destructors  ***//

// default constructor
RtConductor::RtConductor() {
  // all default config
  outputLog = NULL;
}

// constructor with command line args
RtConductor::RtConductor(int argc, char **argv) {
  int curCodeNum;

  // set us as the conductor for the config
  config.setConductor(this);

  // send the arguments to the configuration, check that it was okay
  if(!config.parseArgs(argc,argv) || !config.validateConfig()) {
    cerr << "config failed... exit" << endl;
    exit(1);
  }

  // prepare inputs

  if(config.get("receiveScannerImages")==true) {
    RtInputScannerImages *scanimg;
    ACE_NEW_NORETURN(scanimg, RtInputScannerImages);

    // open and configure
    if(!scanimg->open(config)) {
      cerr << "ERROR: could not establish scanner image listener" << endl;
      exit(1);
    }
    else {
      inputs.push_back(scanimg);
    }
  }

  if(config.get("receiveScannerTriggers")==true) {
    RtInputUSBKb *scantrig;
    ACE_NEW_NORETURN(scantrig, RtInputUSBKb);

    // open and configure
    if(!scantrig->open(config)) {
      cerr << "ERROR: could not establish scanner trigger listener" << endl;
      exit(1);
    }
    else {
      inputs.push_back(scantrig);
    }
  }

  // attach signal numbers to inputs
  curCodeNum = START_CODE_INPUTS;
  for(vector<RtInput*>::iterator i = inputs.begin(); i != inputs.end();
	curCodeNum++, i++) {
    (*i)->setConductor(this);
    (*i)->setCodeNum(curCodeNum);
    //ACE_Reactor::instance()->register_handler(curCodeNum, this);
  }


  // prepare streams

  // attach signal numbers to streams
  curCodeNum = START_CODE_STREAMS;
  for(vector<RtStream*>::iterator i = streams.begin(); i != streams.end();
	curCodeNum++, i++) {
    (*i)->setConductor(this);
    (*i)->setCodeNum(curCodeNum);
    //ACE_Reactor::instance()->register_handler(curCodeNum, this);
  }


  // prepare outputs

  if(config.get("logOutput")==true) {
    ACE_NEW_NORETURN(outputLog, RtOutputFile);

    if(!outputLog->open(config)) {
      cerr << "ERROR: could not open logfile \""
	   << config.get("filename") << "\"" << endl;
      exit(1);
    }
    else {
      outputs.push_back(outputLog);
    }
  }

  if(config.get("imageDisplay")==true) {
    RtDisplayImage *dispimg;
    ACE_NEW_NORETURN(dispimg, RtDisplayImage);    

    if(!dispimg->init(config)) {
      cerr << "ERROR: could not initialize image display" << endl;
      exit(1);
    }
    else {
      outputs.push_back(dispimg);
    }
  }

  // attach signal numbers to outputs
  curCodeNum = START_CODE_OUTPUTS;
  for(vector<RtOutput*>::iterator i = outputs.begin(); i != outputs.end();
	curCodeNum++, i++) {
    (*i)->setConductor(this);
    (*i)->setCodeNum(curCodeNum);
    //ACE_Reactor::instance()->register_handler(curCodeNum, this);
  }


  // connect components that need connecting
//  if(config.get("imageDisplay")==true
//     && config.get("receiveScannerImages")==true) {
//    scannerImageInput.setHandler(displayImage);
//  }

}

// destructor
RtConductor::~RtConductor() {

  // tell everyone that we're done and delete them
  for(vector<RtInput*>::iterator i=inputs.begin(); i != inputs.end(); i++) {
    (*i)->close();
    delete (*i);
  }

  for(vector<RtStream*>::iterator j=streams.begin(); j != streams.end(); j++){
    (*j)->close();
    delete (*j);
  }

  for(vector<RtOutput*>::iterator k=outputs.begin(); k != outputs.end(); k++){
    (*k)->close();
    delete (*k);
  }

}

//*** initialization routines  ***//

// adds a processing stream to the pipeline
//  in:
//   stream: stream object
//  out:
//   true (for success) or false
bool RtConductor::addStream(RtStream &stream) {

  return true;
}

// adds input mode
//  in:
//   in: input object
//  out:
//   true (for success) or false
bool RtConductor::addInput(RtInput &in) {

  return true;
}

// adds output mode
//  in:
//   out: output object
//  out:
//   true (for success) or false
bool RtConductor::addOutput(RtOutput &out) {

  return true;
}

// initialize config and prepare to run
//  out:
//   true (for success) or false
bool RtConductor::init() {

  (*outputLog).writeConfig(config);

  (*outputLog) << "initialization completed at ";
  (*outputLog).printNow();
  (*outputLog) << "\n";

  return true;
}

//*** operation routines  ***//

// begins execution of a realtime fMRI session
//  out:
//   true (for success) or false
bool RtConductor::run() {

  // print start time to log file
  (*outputLog) << "began running at ";
  (*outputLog).printNow();
  (*outputLog) << "\n";



  // start up the threads that listen for input
  for(vector<RtInput*>::iterator i = inputs.begin(); i != inputs.end(); i++) {
    (*i)->activate();
  }

  // start the display
//  if(config.get("imageDisplay")==true) {
//    displayImage.activate();
//  }

  // wait for threads to complete

  // start up the threads that listen for input
  for(vector<RtInput*>::iterator i = inputs.begin(); i != inputs.end(); i++) {
    (*i)->wait();
  }

//  if(config.get("receiveScannerImages")==true) {
//    scannerImageInput.wait();
//  }

  // run the event loop
  //ACE_Reactor::instance()->run_reactor_event_loop();

  return true;
}

// receive a code signaling completetion of data input or processing
void RtConductor::receiveCode(unsigned int code) {

  // NEED TO MUTEX THIS!

  // handle based on the thrower
  if(code < START_CODE_INPUTS) { // shouldn't get here
    cerr << "WARNING: caught unknown signal " << code << endl;
    return;
  }
  else if(code < START_CODE_STREAMS) { // this is an input
    cerr << "caught a ready signal from an input" << endl;
  }
  else if(code < START_CODE_OUTPUTS) { // this is a stream
    cerr << "caught a ready signal from a stream" << endl;

  }
  else { // this is an output
    cerr << "caught a ready signal from an output" << endl;
  }

}

//// handle signals appropriately
//// this method handles errors as well as signals related to normal operation
//int RtConductor::handle_signal(int sigNum, siginfo_t *sInfo,
//			       ucontext_t *uContext) {
//  // look for signals that are errors
//  switch(sigNum) {
//  case SIGHUP:
//  case SIGINT:
//  case SIGQUIT:
//  case SIGILL:
//  case SIGABRT:
//  case SIGBUS:
//  case SIGFPE:
//  case SIGKILL:
//  case SIGSEGV:
//  case SIGALRM:
//  case SIGTERM:
//  case SIGSTKFLT:
//  case SIGPWR:
//  case SIGSYS:
//
//    // stop the loop and call for cleanup
//    ACE_Reactor::instance()->end_reactor_event_loop();
//
//    return 1;
//  }
//
//  // we threw the signal under normal operation, store it in our queue and
//  // notify the exception handler
//  sigQueue.push(sigNum);
//  ACE_Reactor::instance()->notify(this);
//
//  return 0;
//}
//
//// handle completetion events
//int RtConductor::handle_input(ACE_HANDLE handle) {
//  
//  return 0;
//}
//
//// handle exceptions appropriately
//// note that handles errors as well as signals related to normal operation
//// we are notify()ed of exceptions when streams are done or when new data is
//// available
//int RtConductor::handle_exception(ACE_HANDLE handle) {
//  // pop off the queue
//  unsigned int sigNum = sigQueue.front();
//  sigQueue.pop();
//
//  // handle based on the thrower
//  if(sigNum < START_CODE_INPUTS) { // shouldn't get here
//    cerr << "WARNING: caught unknown signal " << sigNum << endl;
//    return 1;
//  }
//  else if(sigNum < START_CODE_STREAMS) { // this is an input
//    cerr << "caught a ready signal from an input" << endl;
//  }
//  else if(sigNum < START_CODE_OUTPUTS) { // this is a stream
//    cerr << "caught a ready signal from a stream" << endl;
//
//  }
//  else { // this is an output
//    cerr << "caught a ready signal from an output" << endl;
//  }
//
//  return 0;
//}
//
//// handle exit of the process
//int RtConductor::handle_exit(ACE_Process *proc) {
//  // print end time to log file
//  (*outputLog) << "done running at ";
//  (*outputLog).printNow();
//  (*outputLog) << "\n";
//
//  return 0;
//}
//

// gets the version
//  out:
//   cvs version string for this class
char *RtConductor::getVersionString() {
  return VERSION;
}

// main function for the realtime system
// very simple
int ACE_TMAIN(int argc, char **args) {

  RtConductor conductor(argc,args);

  conductor.init();
  conductor.run();

  return 0;
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


