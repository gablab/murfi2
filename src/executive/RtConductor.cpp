/******************************************************************************
 * Conductor.cpp defines a class that oversees and coordinates all
 * operations during a real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#include"RtConductor.h"

//*** constructors/destructors  ***//

// default constructor
RtConductor::RtConductor() {
  // all default config

  
}

// constructor with command line args
RtConductor::RtConductor(int argc, char **argv) {
  // send the arguments to the configuration, check that it was okay
  if(!config.parseArgs(argc,argv)) {
    cerr << "config failed... exit" << endl;
  }

  // open the default io objects

  if(config.getImagesFromScanner) {
    scannerInput = new RtScannerInput(config.dataTcpIpPortNum,
				      config.mosaic);
  }

  if(!config.noLogging) {
    logOutput = new RtLogOutput(config.logFilename);    
  }
}

// destructor
RtConductor::~RtConductor() {

  // tell evryone that we're done
  scannerInput.close();
  logOutput.close();

  for(int i=0; i < inputs.len

  // delete private data
  delete scannerInput;
  delete logOutput;
  delete inputs;
  delete streams;
  delete outputs;
}

//*** initialization routines  ***//

// adds a configuration method
//  in:
//   conf: configuration object 
//  out:
//   true (for success) or false
bool RtConductor::addConfig(RtConfig &conf) {

}

// adds a processing stream to the pipeline
//  in:
//   stream: stream object
//  out:
//   true (for success) or false
bool RtConductor::addStream(RtStream &stream) {

}

// adds input mode
//  in:
//   in: input object
//  out:
//   true (for success) or false
bool RtConductor::addInput(RtInput &in) {

}

// adds output mode
//  in:
//   out: output object
//  out:
//   true (for success) or false
bool RtConductor::addOutput(RtOutput &out) {

}

// initialize config and prepare to run
//  out:
//   true (for success) or false
bool RtConductor::init() {
  
}

// sets the logfile name
//  in:
//   logname: filename for log
void RtConductor::setLogfileName(string logname) {

}

//*** operation routines  ***//

// begins execution of a realtime fMRI session
//  out:
//   true (for success) or false
bool RtConductor::run() {

}

//private:
//
//  //*** private data members  ***//
//
//  // input object that handles reception of scanner triggers for external sync
//  RtInputUSB &scannerInput;
//
//  // output object to log 
//  RtOutputFile &logOutput;
//
//  // configuration object
//  RtConfig config;
//
//  // vector of input objects
//  vector<RtInput&> inputs;
//
//  // vector of stream objects
//  vector<RtStream&> streams;
//
//  // vector of output objects
//  vector<RtOutput&> outputs;
//

// main function for the realtime system
// very simple
int main(int argc, char **args) {

  RtConductor conductor(argc,argv);
  
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


