/*=========================================================================
 *  RtStreamComponent.h is the header for an abstract class for a
 *  processing stream component for a real-time fMRI.
 *
 *  Copyright 2007-2013, the MURFI dev team.
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/

#ifndef RTSTREAMCOMPONENT_H
#define RTSTREAMCOMPONENT_H

#include"ace/Task.h"
#include"RtOutput.h"

#include"RtStream.h"
#include"RtExperiment.h"
#include"RtStreamMessage.h"
#include"RtConfig.h"
#include"RtConductor.h"
#include"RtData.h"

#include<map>
#include<set>
#include<fstream>
using namespace std;

// NOTES FOR SUBCLASSING:
// when this class is subclassed it should get its own
//   static string moduleString;
// then this string should be defined at the top of the source file and id
// should be assigned to moduleString in all constructors. this rather
// cumbersome process is required to maintain the ability to retreive
// component specific id strings both statically and dynamically. sorry.

class RtPasser;

// class declaration

class RtStreamComponent : public ACE_Task<ACE_MT_SYNCH>, public RtOutput {
 public:

  //*** constructors/destructors  ***//

  // default constructor
  RtStreamComponent();

  // destructor
  virtual ~RtStreamComponent();

  //*** initialization routines  ***//

  // configure this stream component
  //  in
  //   configuration
  virtual bool init(TiXmlElement *module, RtConfig *config,
                    RtConductor *conductor);


  // adds an output to receive the data of this stream component
  //  in
  //   output to add
  virtual void addOutput(RtOutput *out, const RtDataID &dataId = RtDataID());

  // adds outputs from a vector
  //  in:
  //   out: vector of output objects
  //  out:
  //   true (for success) or false
  virtual void addVectorOfOutputs(vector<RtOutput*> &outs);

  // initialize and run thread
  //  out:
  //   0 (for success) or -1 (failure)
  virtual int open(void * = 0);

  // send data when we're done
  //  in
  //   msg: message to send
  //   to:  timeout
  virtual int put(ACE_Message_Block *msg, ACE_Time_Value *to);

  // close a stream component
  //  in
  //   flags: flags to tell us who called
  virtual int close(u_long flags);

  // run the stream
  virtual int svc();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString() const;

  // gets whether this component is disabled
  bool getDisabled() const;

  // gets the id for this stream component
  string getID() const;

  // gets the data id for the input
  string getInputModuleID() const;

  // gets the data name for the input
  string getInputDataName() const;

  // gets the roi id for the input
  string getInputRoiID() const;

  // gets the mask roi id
  string getMaskRoiID() const;

  // module name for config
  static string moduleString;

 protected:

  typedef ACE_Task<ACE_MT_SYNCH> super;

  // call the next processing step
  virtual int nextStep(ACE_Message_Block *mb);

  // process the configuration: only use this for cross module or global config
  // that is not available in the xml node for this stream component
  //  in
  //   config class
  virtual bool processConfig(RtConfig &config);

  // process an option
  //  in
  //   name of the option to process
  //   val  text of the option node
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text,
                             const map<string, string> &attr);


  // pure virtual for validation of component configuration
  virtual bool validateComponentConfig() = 0;

  // retreive the data with inputModuleID, inputDataName, and input RoiID
  // from the stream message

  template<class T>
  RtDataImage<T> *getDataFromMessage(const RtStreamMessage &msg) {
    // find the data with the right id
    RtDataImage<T> *img
        = (RtDataImage<T>*) msg.getData(inputModuleID,
                                        inputDataName,
                                        inputRoiID);

    if (img == NULL) {
      cerr << "WARNING: RtRoiCombine couldn't find " << inputModuleID << ":"
           << inputDataName << ":" << inputRoiID << endl;
      return NULL;
    }

    return img;
  }

  // retreive the mask with maskRoiID from the stream message
  RtMaskImage *getMaskFromMessage(const RtStreamMessage &msg);

  // pure virtual for implementation of real processing
  virtual int process(ACE_Message_Block *mb) = 0;

  // pass any results to outputs
  virtual void passData(RtData* data);

  // store any results to data store
  //  in: pointer to data to store
  virtual void storeData(RtData* data);

  // sets the latest result of processing
  //  in
  //   data result
  virtual void setResult(RtStreamMessage *msg, RtData *data);

  // start a logfile
  virtual void startDumpAlgoVarsFile();

  // whether this compoenent is disabled
  bool disabled;

  // whether to print info on results to log file
  bool logOutput;

  // whether to print info on results to command line
  bool print;

  // whether this compoenent should print info bout computation times
  bool printTiming;

  // whether we've done initialization
  bool needsInit;

  // passer to send the results of our computation to outputs
  RtPasser *passer;

  // pointer to the conductor controlling execution
  RtConductor *conductor;

  // whether the result should be placed on the message data
  bool putResultOnMessage;

  // whether the result should be made the current data on the message
  bool makeCurrentData;

  // id string
  string componentID; // for the module id

  // input data
  string inputModuleID;
  string inputDataName;
  string inputRoiID;

  // mask
  string maskRoiID;

  // file to log processing results to
  ofstream ofile;
  string outFilename;

  // create mutex for datastore lockdown
  ACE_Mutex mut;

  // for logging the analysis process
  bool dumpAlgoVars;
  string dumpAlgoVarsFilename;
  ofstream dumpFile;
};

class RtEndTask : public RtStreamComponent {
 public:

  RtEndTask(set<ACE_Message_Block*> *_openMsgs = NULL,
            bool _isProcessor = false);

 protected:

  bool validateComponentConfig() {
    return true;
  }

  int process(ACE_Message_Block* mb);

  int nextStep(ACE_Message_Block *) {

    return 0;
  }

  set<ACE_Message_Block*> *openMsgs;

  // end processing marker
  RtMRIImage endTaskData;
  bool isProcessor;

};


#endif
