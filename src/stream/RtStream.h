/*=========================================================================
 *  RtStream.h is the header for a class that controls execution of a
 *  single data processing stream during a real-time fMRI session.
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

#ifndef RTSTREAM_H
#define RTSTREAM_H

#include<vector>
#include<stack>

#include"ace/Stream.h"

#include"RtConfig.h"
#include"RtOutput.h"

#include<set>

class RtConductor;
class RtStreamComponent;

using namespace std;

// class declaration
class RtStream : public ACE_Stream<ACE_MT_SYNCH> {

 public:

  //*** constructors/destructors  ***//

  // constructor
  RtStream();

  // destructor
  virtual ~RtStream();

  //*** initialization routines  ***//

  // set the conductor for this stream
  //  in
  //   conductor pointer
  void setStreamConductor(RtConductor *_conductor);

  // initialize stream and prepare to run
  //  out:
  //   true (for success) or false
  bool configure(RtConfig &config);

  // whether this stream is still processing
  //  out:
  //   true  or false
  bool isProcessing() { return !openMsgs.empty(); };

  // adds all modules to the stream
  //  in
  //   config: configuration info
  //  out
  //   true for success
  virtual bool addModules(RtConfig &config);

  // adds outputs to a stream component (needs to be here so that we have
  // access to the conductor to get pointers to the outputs)
  //  in
  //   config: configuration info
  virtual void addOutputsToComponent(RtStreamComponent *sc,
                                     vector<string> &outNames);

  // build a vector of ids of outputs from an xml node's children
  //  in
  //   module element
  //  out
  //   vector of string output ids
  void buildOutputNames(TiXmlElement *module, vector<string> &names);

  //*** operation routines  ***//

  // accept new data received from an input
  //  in
  //   data: data
  virtual void setInput(unsigned int code, RtData *data);

  // adds all 'module' nodes that are children of the passed node as modules
  // for the stream
  //  in
  //   elmt: xml element
  void addModulesFromNode(TiXmlElement *elmt, RtConfig *config);

  // build a stream component
  //  in
  //   type: name of the module type to add
  //   out: optional output to pass the result of this module to
  //   text: optional text to be associated with the module
  RtStreamComponent *buildStreamComponent(const string &type,
                                          const string &text = "");

  // add a single module to the stack
  //  in
  //   stream component to be added
  void addSingleModule(RtStreamComponent *sc);

  // adds all the modules on the stack to the processing stream
  // the module stack is emtpty after this executes
  //  out
  //   true for success
  bool pushAllModules();

  // get the version
  //  out: char array that represents the cvs version
  virtual char *getVersionString();

 protected:

  // superclass
  typedef ACE_Stream<ACE_MT_SYNCH> super;

  // processing module
  typedef ACE_Module<ACE_MT_SYNCH> Module;

  // pointer to conductor
  RtConductor *streamConductor;

  // stack of moules to be added
  stack<Module*> addMod;

  // map of messages and the completion status
  set<ACE_Message_Block*> openMsgs;
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
