/******************************************************************************
 * RtConductor.h is the header for a class that oversees and coordinates all
 * operations during a real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTCONDUCTOR_H
#define RTCONDUCTOR_H

#include<vector>

#include"RtConfig.h"
#include"RtInput.h"
#include"RtInputScannerImages.h"
#include"RtInputUSBKb.h"
#include"RtOutput.h"
#include"RtOutputFile.h"
#include"RtStream.h"

using namespace std;

// class declaration
class RtConductor {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  RtConductor(); 

  // constructor with command line args
  RtConductor(int argc, char **argv); 

  // destructor
  virtual ~RtConductor();

  //*** initialization routines  ***//
  
  // adds a processing stream to the pipeline
  //  in:
  //   stream: stream object
  //  out:
  //   true (for success) or false
  bool addStream(RtStream &stream);

  // adds input mode
  //  in:
  //   in: input object
  //  out:
  //   true (for success) or false
  bool addInput(RtInput &in);

  // adds output mode
  //  in:
  //   out: output object
  //  out:
  //   true (for success) or false
  bool addOutput(RtOutput &out);

  // initialize config and prepare to run
  //  out:
  //   true (for success) or false
  bool init();

  //*** operation routines  ***//
  
  // begins execution of a realtime fMRI session
  //  out:
  //   true (for success) or false
  bool run();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

private:

  //*** private data members  ***//

  // input object that handles reception of scanner images
  RtInputScannerImages scannerImageInput;

  // input object that handles reception of scanner triggers for external sync
  RtInputUSBKb scannerTriggerInput;

  // output object to log 
  RtOutputFile logOutput;

  // configuration object
  RtConfig config;

  // vector of input objects
  vector<RtInput> inputs;

  // vector of stream objects
  vector<RtStream> streams;

  // vector of output objects
  vector<RtOutput> outputs;

};

#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


