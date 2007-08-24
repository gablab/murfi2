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

  
}

// constructor with command line args
RtConductor::RtConductor(int argc, char **argv) {
  // set us as the conductor for the config
  config.setConductor(this);

  // send the arguments to the configuration, check that it was okay
  if(!config.parseArgs(argc,argv) || !config.validateConfig()) {
    cerr << "config failed... exit" << endl;
    exit(1);
  }

  // open io 
  if(config.get("receiveScannerImages")==true 
     && !scannerImageInput.open(config)) {
    cerr << "ERROR: could not establish scanner image listener" << endl;
    exit(1);
  }

  if(config.get("receiveScannerTriggers")==true 
     && !scannerTriggerInput.open(config)) {
    cerr << "ERROR: could not establish scanner trigger listener" << endl;
    exit(1);
  }
  
  if(config.get("logOutput")==true 
     && !logOutput.open(config)) {
    cerr << "ERROR: could not open logfile \"" 
	 << config.get("filename") << "\"" << endl;
    exit(1);
  }

}

// destructor
RtConductor::~RtConductor() {

  // tell evryone that we're done
  scannerTriggerInput.close();
  scannerImageInput.close();
  logOutput.close();

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

  logOutput.writeConfig(config);

  logOutput << "initialization completed at ";
  logOutput.printNow();
  logOutput << "\n";

  return true;
}

//*** operation routines  ***//

// begins execution of a realtime fMRI session
//  out:
//   true (for success) or false
bool RtConductor::run() {

  // print start time to log file
  logOutput << "began running at ";
  logOutput.printNow();
  logOutput << "\n";

  
  


  // print end time to log file
  logOutput << "done running at ";
  logOutput.printNow();
  logOutput << "\n";

  return true;
}


// gets the version
//  out:
//   cvs version string for this class
char *RtConductor::getVersionString() {
  return VERSION;
}

// main function for the realtime system
// very simple
int main(int argc, char **args) {

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


