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
  //outputLog = NULL;
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

    if(!addInput(scanimg)) {
      cerr << "ERROR: could not add scanner input" << endl;
    }
  }

  if(config.get("receiveScannerTriggers")==true) {
    RtInputUSBKb *scantrig;
    ACE_NEW_NORETURN(scantrig, RtInputUSBKb);

    if(!addInput(scantrig)) {
      cerr << "ERROR: could not add scanner trigger listener" << endl;
      //exit(1);
    }
  }

  // attach code numbers to inputs
  curCodeNum = START_CODE_INPUTS;
  for(vector<RtInput*>::iterator i = inputs.begin(); i != inputs.end();
	curCodeNum++, i++) {
    (*i)->setConductor(this);
    (*i)->setCodeNum(curCodeNum);
  }

  // prepare outputs

  // display always first, if here
  if(config.get("imageDisplay")==true) {
    RtDisplayImage *dispimg;
    ACE_NEW_NORETURN(dispimg, RtDisplayImage);    

    if(!addOutput(dispimg)) {
      cerr << "ERROR: could not initialize image display" << endl;
    }
  }

  if(config.get("logOutput")==true) {
    //ACE_NEW_NORETURN(outputLog, RtOutputFile);

    if(!addOutput(&outputLog)) {
      cerr << "ERROR: could not open logfile \""
	   << config.get("filename") << "\"" << endl;
    }
  }

  // attach code numbers to outputs
  curCodeNum = START_CODE_OUTPUTS;
  for(vector<RtOutput*>::iterator i = outputs.begin(); i != outputs.end();
	curCodeNum++, i++) {
    (*i)->setConductor(this);
    (*i)->setCodeNum(curCodeNum);
  }

  // build the stream and its components
  //ACE_NEW_NORETURN(stream, RtStream);
  buildStream(config);



}

// destructor
RtConductor::~RtConductor() {

  stream.close();
  //delete stream;

  // tell everyone that we're done and delete them
  for(vector<RtInput*>::iterator i=inputs.begin(); i != inputs.end(); i++) {
    (*i)->close();
    delete (*i);
  }

  for(vector<RtOutput*>::iterator k=outputs.begin(); k != outputs.end(); k++){
    (*k)->close();
    delete (*k);
  }

}

//*** initialization routines  ***//

// builds the processing stream 
//  in:
//   config: configuration
//  out:
//   true (for success) or false
bool RtConductor::buildStream(RtConfig config) {

  // set the conductor to us
  stream.setConductor(this);

  // open the stream
  stream.configure(config);

  return true;
}


// adds input mode
//  in:
//   in: input object
//  out:
//   true (for success) or false
bool RtConductor::addInput(RtInput *in) {
  // open and configure
  if(!in->open(config)) {
    return false;
  }
  else {
    inputs.push_back(in);
  }

  return true;
}

// adds output mode
//  in:
//   out: output object
//  out:
//   true (for success) or false
bool RtConductor::addOutput(RtOutput *out) {
  // open and configure
  if(!out->open(config)) {
    return false;
  }
  else {
    outputs.push_back(out);
  }

  return true;
}

// initialize config and prepare to run
//  out:
//   true (for success) or false
bool RtConductor::init() {

  outputLog.writeConfig(config);

  // initialize output methods
//  for(vector<RtOutput*>::iterator i = outputs.begin(); i != outputs.end(); i++) {
//    (*i)->open(config);
//  }



  outputLog << "initialization completed at ";
  outputLog.printNow();
  outputLog << "\n";

  return true;
}

//*** operation routines  ***//

// begins execution of a realtime fMRI session
//  out:
//   true (for success) or false
bool RtConductor::run() {

  // print startg time to log file
  outputLog << "began running at ";
  outputLog.printNow();
  outputLog << "\n";

  // 

  // start up the threads that listen for input
  for(vector<RtInput*>::iterator i = inputs.begin(); i != inputs.end(); i++) {
    (*i)->activate();
  }

  // start the display
  if(config.get("imageDisplay")==true) {  
    getDisplay()->activate();
  }

  // wait for threads to complete

  // start up the threads that listen for input
  for(vector<RtInput*>::iterator i = inputs.begin(); i != inputs.end(); i++) {
    (*i)->wait();
  }


  // print end time to log file
  outputLog << "done running at ";
  outputLog.printNow();
  outputLog << "\n";

  return true;
}

// receive a code signaling completetion of data input or processing
void RtConductor::receiveCode(unsigned int code, RtData *data) {

  // handle based on the thrower
  if(code == START_CODE_STREAM) { // stream component has data
    cerr << "caught data available signal from a stream component" << endl;

    // make data available to all output processes
    for(vector<RtOutput*>::iterator i = outputs.begin(); 
	i != outputs.end(); i++) {
      (*i)->setData(data);
    }

    return;
  }
  else if(code < START_CODE_OUTPUTS) { // this is an input
    cerr << "caught a ready signal from an input" << endl;

    // let the stream decide if it should spawn a new processing instance 
    stream.setInput(code,data);
  }
  else { // this is an output
    cerr << "caught a ready signal from an output" << endl;

    // dont need to do much here
  }

}


// get the display output
//  out 
//   pointer to the display output object
RtDisplayImage *RtConductor::getDisplay() {
  if(config.get("imageDisplay")==true) {
     return (RtDisplayImage*) (*outputs.begin());
  }
  else {
    return NULL;
  }
}


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


