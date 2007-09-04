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
#include<queue>

//#include"ace/Reactor.h"
//#include"ace/Event_Handler.h"

#include"RtConfig.h"
#include"RtDisplayImage.h"
#include"RtInput.h"
#include"RtInputScannerImages.h"
#include"RtInputUSBKb.h"
#include"RtOutput.h"
#include"RtOutputFile.h"
#include"RtStream.h"
#include"RtCode.h"

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
 
  // initialize config and prepare to run
  //  out:
  //   true (for success) or false
  bool init();

  //*** operation routines  ***//
  
  // begins execution of a realtime fMRI session
  //  out:
  //   true (for success) or false
  bool run();

  // receive a code signaling completetion of data input or processing
  void receiveCode(unsigned int code, RtData *data);

//  // handle signals appropriately
//  // this method handles errors as well as signals related to normal operation
//  int handle_signal(int sigNum, siginfo_t *sInfo=0, ucontext_t *uContext=0);
//
//  // handle completetion events
//  int handle_input(ACE_HANDLE handle = ACE_INVALID_HANDLE);
//
//  // handle exceptions appropriately
//  // note that handles errors as well as signals related to normal operation
//  // we are notify()ed of exceptions when streams are done or when new data is
//  // available
//  int handle_exception(ACE_HANDLE handle = ACE_INVALID_HANDLE);
//
//  // handle exit of the process
//  int handle_exit(ACE_Process *proc);

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

protected:

  //*** methods ***//

  // builds the processing stream 
  //  in:
  //   config: configuration
  //  out:
  //   true (for success) or false
  bool buildStream(RtConfig config);

  // adds input mode
  //  in:
  //   in: input object
  //  out:
  //   true (for success) or false
  bool addInput(RtInput *in);

  // adds output mode
  //  in:
  //   out: output object
  //  out:
  //   true (for success) or false
  bool addOutput(RtOutput *out);

  //*** members ***//

  // configuration object
  RtConfig config;

  // the data processing stream
  RtStream stream;
  static const unsigned int START_CODE_STREAM = 0;

  // output object to log 
  RtOutputFile* outputLog;

  // these vectors store the objects that handle io
  // note that their indexing is related to the code number they throw when 
  // processing is complete. the index is related by the START_CODE_?????
  // static class members
  
  // vector of input objects
  vector<RtInput*> inputs;
  static const unsigned int START_CODE_INPUTS = 1;

  // vector of output objects
  vector<RtOutput*> outputs;
  static const unsigned int START_CODE_OUTPUTS = 32768;

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


