/******************************************************************************
 * RtConductor.h is the header for a class that oversees and coordinates all
 * operations during a real-time fMRI session.
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14 
 * 
 *****************************************************************************/

#ifndef RTCONDUCTOR_H
#define RTCONDUCTOR_H

#include"ace/Task.h"

#include<vector>
#include<sstream>

#include"RtConfigFmriRun.h"
#include"RtInput.h"
#include"RtInputScannerImages.h"
#include"RtOutput.h"
#include"RtOutputFile.h"
#include"RtStream.h"
#include"RtCode.h"

using namespace std;

// class declaration
class RtConductor : public ACE_Task_Base {

public:

  //*** constructors/destructors  ***//
  
  // default constructor
  // haha, you can't call this
  RtConductor() : ACE_Task_Base(), configured(false), running(false) {}; 

  // constructor with config
  explicit RtConductor(const RtConfigFmriRun &conf); 

  // destructor
  virtual ~RtConductor();

  //*** initialization routines  ***//

  // retreive configuration
  RtConfig &getConfig() { return config; }
 
  // initialize config and prepare to run
  // NOTE: this function must me called before each call to run(), but its
  // called from the constructor taking a config class, too.
  //  in:
  //   configuration for this run
  //  out:
  //   true (for success) or false
  bool configure(const RtConfigFmriRun &_config);

  // after a run, clean up and close streams
  void deconfigure();

  // adds input mode
  //  in:
  //   in: input object
  //  out:
  //   true (for success) or false
  bool addInput(RtInput *in);

  // adds input mode that has already been configured
  //  in:
  //   out: input object
  //  out:
  //   true (for success) or false
  bool addExistingInput(RtInput *in);

   // adds a vector of inputs
  //  in:
  //   in: vector of input object
  //  out:
  //   true (for success) or false
  bool addVectorOfInputs(vector<RtInput*> &ins);

  // adds output mode
  //  in:
  //   out: output object
  //  out:
  //   true (for success) or false
  bool addOutput(RtOutput *out);


  // adds output mode that has already been configured
  //  in:
  //   out: output object
  //  out:
  //   true (for success) or false
  bool addExistingOutput(RtOutput *out);

  // adds outputs from a vector
  //  in:
  //   out: vector of output objects
  //  out:
  //   true (for success) or false
  bool addVectorOfOutputs(vector<RtOutput*> &outs);

  //*** operation routines  ***//
  
  // begins execution of a realtime fMRI session
  //  out:
  //   0 (for success) or error code
  int svc();

  // get whether execution is happening
  bool isRunning() { return running; }

  //*** callback entries for threads ***//

  // receive a code signaling completetion of data input or processing
  void receiveCode(unsigned int code, RtData *data);

  // write to the log file
  void log(const string &s);

  // write to the log file
  void log(stringstream &s);

  //** gets **//

  // get an input by its name
  //  in
  //   name: name of input to get
  //  out 
  //   pointer to the input object
  RtInput *getInputByName(const string &name);

  // get an output by its name
  //  in
  //   name: name of output to get
  //  out 
  //   pointer to the output object
  RtOutput *getOutputByName(const string &name);

  // get all the outputs with a name
  //  in
  //   name: name of output to get
  //  out 
  //   vector of pointers to the output objects
  vector<RtOutput*> getAllOutputsWithName(const string &name);

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

protected:

  //*** methods ***//

  // builds the processing stream 
  //  out:
  //   true (for success) or false
  bool buildStream();

  //*** members ***//

  // configuration object
  RtConfigFmriRun config;

  // the data processing stream
  RtStream stream;

  // output object to log 
  RtOutputFile outputLog;

  // these vectors store the objects that handle io
  // note that their indexing is related to the code number they throw when 
  // processing is complete. the index is related by the START_CODE_?????
  // static class members
  
  // vector of input objects
  vector<RtInput*> inputs;

  // vector of output objects
  vector<RtOutput*> outputs;

  // whether we are ready to run
  bool configured;

  // whether we are running
  bool running;

private:

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


