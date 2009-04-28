/******************************************************************************
 * Conductor.cpp defines a class that oversees and coordinates all
 * operations during a single real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14
 *
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtConductor.h"
#include"RtInputSynth.h"
#include<iostream>

// constructor with config
RtConductor::RtConductor(const RtConfigFmriRun &_config) : ACE_Task_Base(), 
							   configured(false),
							   running(false) {
  ACE_TRACE(("RtConductor::RtConductor"));

  configure(config);
}

// destructor
RtConductor::~RtConductor() {
  ACE_TRACE(("RtConductor::~RtConductor"));

}

//*** initialization routines  ***//

// configure the conductor
//  in
//   configuration
bool RtConductor::configure(const RtConfigFmriRun &_config) {

  // copy the config
  config = _config;

  int curCodeNum;

  // prepare inputs

  // attach code numbers to inputs
  curCodeNum = START_CODE_INPUTS;
  for(vector<RtInput*>::iterator i = inputs.begin(); i != inputs.end();
	curCodeNum++, i++) {
    (*i)->setConductor(this);
    (*i)->setCodeNum(curCodeNum);
  }

  // prepare outputs
  if(config.get("study:log:disabled")==false) {
    if(!outputLog.open(config)) {
      cerr << "ERROR: could not open logfile \""
	   << config.get("study:log:filename") << "\"" << endl;
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
  if(!buildStream()) {
    cerr << "ERROR: failed to build the processing stream for the conductor"
	 << endl;
    return false;
  }


  configured = true;

  // write configuration to the log
  outputLog.writeConfig(config);
  outputLog << "initialization completed at ";
  outputLog.printNow();
  outputLog << "\n";

  return true;
}

// deconfigure the conductor by cleaning up streams
void RtConductor::deconfigure() {

  stream.close();

  // tell everyone that we're done and delete them
  for(vector<RtInput*>::iterator i=inputs.begin(); i != inputs.end(); i++) {
    if((*i)->isDeleteable()) {
      (*i)->close();
      delete (*i);
    }
  }

  for(vector<RtOutput*>::iterator o=outputs.begin(); o != outputs.end(); o++){
    if((*o)->isDeleteable()) {
      (*o)->close();
      delete (*o);
    }
  }

  configured = false;
}

// builds the processing stream 
//  in:
//   config: configuration
//  out:
//   true (for success) or false
bool RtConductor::buildStream() {
  ACE_TRACE(("RtConductor::buildStream"));

  // set the conductor to us
  stream.setStreamConductor(this);

  // open the stream
  if(!stream.configure(config)) {
    return false;
  }

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
    cerr << "WARNING: failed to add input: " << in->getID() << endl;
    return false;
  }
  else {
    in->setActivatable(true);
    in->setDeleteable(true);
  }
  
  inputs.push_back(in);
  cout << "added input: " << in->getID() << endl;
  
  return true;
}

// adds input mode that has already been configured
//  in:
//   out: input object
//  out:
//   true (for success) or false
bool RtConductor::addExistingInput(RtInput *in) {
  ACE_TRACE(("RtConductor::addInput"));

  inputs.push_back(in);

  cout << "added input: " << in->getID() << endl;

  return true;
}

// adds inputs from a vector
//  in:
//   in: vector of input objects
//  out:
//   true (for success) or false
bool RtConductor::addVectorOfInputs(vector<RtInput*> &ins) {
  bool ret = true;
  for(vector<RtInput*>::iterator i = ins.begin(); i != ins.end(); i++) {
    if(!addInput(*i)) {
      ret = false;
    }
  }
  return ret;
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
    cerr << "WARNING: failed to add output: " << out->getID() << endl;
    return false;
  }
  else {
    out->setDeleteable(true);
  }

  outputs.push_back(out);

  cout << "added output: " << out->getID() << endl;

  return true;
}


// adds output mode that has already been configured
//  in:
//   out: output object
//  out:
//   true (for success) or false
bool RtConductor::addExistingOutput(RtOutput *out) {
  ACE_TRACE(("RtConductor::addOutput"));

  outputs.push_back(out);

  cout << "added output: " << out->getID() << endl;

  return true;
}

// adds outputs from a vector
//  in:
//   out: vector of output objects
//  out:
//   true (for success) or false
bool RtConductor::addVectorOfOutputs(vector<RtOutput*> &outs) {
  bool ret = true;
  for(vector<RtOutput*>::iterator i = outs.begin(); i != outs.end(); i++) {
    if(!addOutput(*i)) {
      ret = false;
    }
  }
  return ret;
}

//*** operation routines  ***//

// begins execution of a realtime fMRI session
//  out:
//   true (for success) or false
int RtConductor::svc() {
  ACE_TRACE(("RtConductor::svc"));
 
  if(!configured) {
    cerr << "ERROR: the conductor for this run has not been configured"
 	 << endl;
    return false;
  }
 
  // print start time to log file
  outputLog << "began running at ";
  outputLog.printNow();
  outputLog << "\n";
 
  // start up the threads that listen for input
  for(vector<RtInput*>::iterator i = inputs.begin(); i != inputs.end(); i++) {
    if((*i)->isActivatable()) {
      (*i)->activate();
    }
  }
 
  running = true;
  while(running) { usleep(10000); }

  // wait for all threads to complete
//  for(vector<RtInput*>::iterator i = inputs.begin(); i != inputs.end(); i++) {
//    (*i)->wait();
//  }
//
//  running = false;

  cout << "conductor waiting for stream to complete processing...";
  while(stream.isProcessing()) {
    sleep(10000);
  }
  cout << "done" << endl;


  // print end time to log file
  outputLog << "done running at ";
  outputLog.printNow();
  outputLog << "\n";

  deconfigure();

  return true;
}

// receive a code signaling completetion of data input or processing
void RtConductor::receiveCode(unsigned int code, RtData *data) {
  ACE_TRACE(("RtConductor::receiveCode"));

  // handle based on the thrower
  if(code == SHUTDOWN) {
    cout << "shutdown signal " << code << " received" << endl;
    running = false;
  }
  else if(code == START_CODE_STREAM) { // stream component has data
    //cout << "caught data " << data << " available signal from a stream component" << endl;

    // make data available to all output processes
    for(vector<RtOutput*>::iterator i = outputs.begin(); 
	i != outputs.end(); i++) {
      (*i)->setData(data);
    }

    return;
  }
  else if(code < START_CODE_OUTPUTS) { // this is an input
    //cout << "caught data " << data << " as input to the stream" << endl;

    // let the stream decide if it should spawn a new processing instance 
    stream.setInput(code,data);

    //cout << "sent ready signal" << endl;
  }
  else { // this is an output
    //cout << "caught a ready signal from an output" << endl;

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
//   pointer to the output object (the first with the specified name)
RtOutput *RtConductor::getOutputByName(const string &name) {
  for(vector<RtOutput*>::iterator i = outputs.begin(); i != outputs.end(); i++){
    //cout << (*i)->getID() << "=" << name << endl;
    if((*i)->getID() == name) {
      return *i;
    }
  }

  return NULL;
}

// get all the outputs with a name
//  in
//   name: name of output to get
//  out 
//   vector of pointers to the output objects
vector<RtOutput*> RtConductor::getAllOutputsWithName(const string &name) {
  vector<RtOutput*> outs;
  for(vector<RtOutput*>::iterator i = outputs.begin(); i != outputs.end(); i++){
    //cout << (*i)->getID() << "=" << name << endl;
    if((*i)->getID() == name) {
      outs.push_back(*i);
    }
  }

  return outs;
}

// gets the version
//  out:
//   cvs version string for this class
char *RtConductor::getVersionString() {
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


