/******************************************************************************
 * Conductor.cpp defines a class that oversees and coordinates all
 * operations during a real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14
 *
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtConductor.h"
#include"RtOutputSocket.h"
#include<iostream>

//*** constructors/destructors  ***//

// default constructor
//RtConductor::RtConductor() {
//}

// constructor with command line args
RtConductor::RtConductor(int argc, char **argv) {
  ACE_TRACE(("RtConductor::RtConductor"));

  int curCodeNum;

  // set us as the conductor for the config
  config.setConductor(this);

  // send the arguments to the configuration, check that it was okay
  if(!config.parseArgs(argc,argv) || !config.validateConfig()) {
    cerr << "config failed... exit" << endl;
    exit(1);
  }


  // prepare inputs

  if(config.get("scanner:receiveImages")==true) {
    RtInputScannerImages *scanimg;
    ACE_NEW_NORETURN(scanimg, RtInputScannerImages);

    if(!addInput(scanimg)) {
      cerr << "ERROR: could not add scanner input" << endl;
    }
  }

  if(config.get("receiveTriggers")==true) {
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
  if(config.get("display:image")==true) {
    RtDisplayImage *dispimg;
    ACE_NEW_NORETURN(dispimg, RtDisplayImage);    

    if(!addOutput(dispimg)) {
      cerr << "ERROR: could not initialize image display" << endl;
    }
  }

  // open output socket
  if(config.get("socket:host")==true) {
    RtOutputSocket *sock;
    ACE_NEW_NORETURN(sock, RtOutputSocket);    

    if(!addOutput(sock)) {
      cerr << "ERROR: could not initialize output socket" << endl;
    }
  }

  if(config.get("info:log:disabled")==false) {
    if(!outputLog.open(config)) {
      cerr << "ERROR: could not open logfile \""
	   << config.get("info:log:logFilename") << "\"" << endl;
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
  buildStream(config);



}

// destructor
RtConductor::~RtConductor() {
  ACE_TRACE(("RtConductor::~RtConductor"));

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
  ACE_TRACE(("RtConductor::buildStream"));

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
  ACE_TRACE(("RtConductor::addInput"));

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
  ACE_TRACE(("RtConductor::addOutput"));

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
  ACE_TRACE(("RtConductor::init"));

  outputLog.writeConfig(config);


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
  ACE_TRACE(("RtConductor::run"));

  // print startg time to log file
  outputLog << "began running at ";
  outputLog.printNow();
  outputLog << "\n";

  // start the display
  if(config.get("display:image")==true) {  
    getDisplay()->activate();
  }

  // start up the threads that listen for input
  for(vector<RtInput*>::iterator i = inputs.begin(); i != inputs.end(); i++) {
    (*i)->activate();
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
  ACE_TRACE(("RtConductor::receiveCode"));

  // handle based on the thrower
  if(code == START_CODE_STREAM) { // stream component has data
    cout << "caught data " << data << " available signal from a stream component" << endl;

    // make data available to all output processes
    for(vector<RtOutput*>::iterator i = outputs.begin(); 
	i != outputs.end(); i++) {
      (*i)->setData(data);
    }

    return;
  }
  else if(code < START_CODE_OUTPUTS) { // this is an input
    cout << "caught a ready signal from an input " << endl;

    // let the stream decide if it should spawn a new processing instance 
    stream.setInput(code,data);

    cout << "sent ready signal" << endl;
  }
  else { // this is an output
    cout << "caught a ready signal from an output" << endl;

    // dont need to do much here
  }

}

// write to the log file
void RtConductor::log(const string &s) {
  outputLog.write(s);
}

// write to the log file
void RtConductor::log(stringstream &s) {
  outputLog.write(s);
}


// get the display output
//  out 
//   pointer to the display output object
RtDisplayImage *RtConductor::getDisplay() {
  if(config.get("display:image")==true) {
     return (RtDisplayImage*) (*outputs.begin());
  }
  else {
    return NULL;
  }
}

// get an input by its name
//  in
//   name: name of input to get
//  out 
//   pointer to the input object
RtInput *RtConductor::getInputByName(const string &name) {
  for(vector<RtInput*>::iterator i = inputs.begin(); i != inputs.end(); i++) {
    if((*i)->getID() == name) {
      return *i;
    }
  }
  
  return NULL;
}

// get an output by its name
//  in
//   name: name of output to get
//  out 
//   pointer to the output object
RtOutput *RtConductor::getOutputByName(const string &name) {
  for(vector<RtOutput*>::iterator i = outputs.begin(); i != outputs.end(); i++){
    if((*i)->getID() == name) {
      return *i;
    }
  }

  return NULL;
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
  ACE_TRACE(("ACE_TMAIN"));


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


