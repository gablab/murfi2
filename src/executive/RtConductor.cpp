/******************************************************************************
 * Conductor.cpp defines a class that oversees and coordinates all
 * operations during a real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14
 *
 *****************************************************************************/

static char *VERSION = "$Id$";

#include"RtConductor.h"
#include"RtInfoServer.h"
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

 /* if(config.get("receiveTriggers")==true) {
    RtInputUSBKb *scantrig;
    ACE_NEW_NORETURN(scantrig, RtInputUSBKb);

    if(!addInput(scantrig)) {
      cerr << "ERROR: could not add scanner trigger listener" << endl;
    }
  }*/

  // attach code numbers to inputs
  curCodeNum = START_CODE_INPUTS;
  for(vector<RtInput*>::iterator i = inputs.begin(); i != inputs.end();
	curCodeNum++, i++) {
    (*i)->setConductor(this);
    (*i)->setCodeNum(curCodeNum);
  }

  // prepare outputs

#ifndef USE_FRONTEND
  // display always first, if here
  if(config.get("display:image")==true) {
    RtDisplayImage *dispimg;
    ACE_NEW_NORETURN(dispimg, RtDisplayImage);    

    if(!addOutput(dispimg)) {
      cerr << "ERROR: could not initialize image display" << endl;
    }
  }
#endif

  //// open output socket
  //if(config.get("infoserver:port")==true) {
  //  RtInfoServer *infoserver;
  //  ACE_NEW_NORETURN(infoserver, RtInfoServer);    

  //  if(!addOutput(infoserver)) {
  //    cerr << "ERROR: could not initialize info server" << endl;
  //  }
  //}

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
  stream.setStreamConductor(this);

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
    return false;
  }
  else {
    outputs.push_back(out);
  }

  cout << "added " << out->getID() << " as output " << out << endl;

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

// initialize config and prepare to run
//  out:
//   true (for success) or false
bool RtConductor::init() {
  ACE_TRACE(("RtConductor::init"));

  outputLog.writeConfig(config);
  RtExperiment::setConfig(&config);

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

  // start the info server
  //if(config.isSet("infoserver:port")) {  
  //  getInfoServer()->activate();
  //}

  // start up the threads that listen for input
  for(vector<RtInput*>::iterator i = inputs.begin(); i != inputs.end(); i++) {
    (*i)->activate();
  }

  // start the display
  // DIRTY HACK TO LET DISPLAY RUN IN MAIN THREAD
  //if(config.get("display:image")==true) {  
    RtDisplay* out = getDisplay();
    out->svc();
  //  //getDisplay()->activate();
  //}
  //else {
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
    //cout << "caught data " << data << " available signal from a stream component" << endl;

    // make data available to all output processes
    for(vector<RtOutput*>::iterator i = outputs.begin(); 
	i != outputs.end(); i++) {
      (*i)->setData(data);
    }

    return;
  }
  else if(code < START_CODE_OUTPUTS) { // this is an input
    //cout << "caught a ready signal from an input " << endl;

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


// get the display output
//  out 
//   pointer to the display output object
RtDisplay *RtConductor::getDisplay() {
//  if(config.get("display:image")==true) {
     return (RtDisplay*) (*outputs.begin());
//  }
//  else {
//    return NULL;
//  }
}

// get the info server
//  out 
//   pointer to the infoserver object
RtInfoServer *RtConductor::getInfoServer() {
  return (RtInfoServer*) getOutputByName("output:infoserver");
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

// get data store map
//  out
//   pointer to data store map
RtDataStore *RtConductor::getDataStore() {
  //TODO put in checks?
  return &dataStore;
}

// gets the version
//  out:
//   cvs version string for this class
char *RtConductor::getVersionString() {
  return VERSION;
}

// NOTE: when we build app using front end 
// then main hasn't be defined several times
#ifndef USE_FRONTEND

// main function for the realtime system
// very simple
int ACE_TMAIN(int argc, char **args) {
  ACE_TRACE(("ACE_TMAIN"));


  RtConductor conductor(argc,args);

  conductor.init();

  conductor.run();

  return 0;
}

#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


