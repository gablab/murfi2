/*=========================================================================
 *  RtProcessor.cpp is the definition of a class for a processing stream
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

static char *VERSION = "$Id$";

#include"RtProcessor.h"
#include"RtDataIDs.h"
#include"RtDiff.h"
#include"RtVar.h"

#include"tinyxml/tinyxml.h"

// default constructor
RtProcessor::RtProcessor()
    : RtStreamComponent() {
  setID("RtProcessor");
}

// destructor
RtProcessor::~RtProcessor() {

}

//*** initialization routines  ***//


// initialize stream and prepare to run
//  out:
//   true (for success) or false
bool RtProcessor::configure(RtConfig &config) {

  Module *head = 0, *tail = 0;

  // create the head and tail by passing them up to the superclass
  ACE_NEW_RETURN(tail, Module(ACE_TEXT("end module"),
                              new RtEndTask(&openMsgs, true)), -1);
  RtStream::open(NULL, head, tail);

  return addModules(config);
}

// add modules based on the processor configuration
//  in
//   config
//  out
//   success or failure
bool RtProcessor::addModules(RtConfig &config) {
  ACE_TRACE(("RtProcessor::addModules"));

  // get the node for preprocessing
  TiXmlNode *procNode = config.getNode("processor");

  // check for preprocessor node (backward compatibility)
  if(procNode == NULL) {
    procNode = config.getNode("preprocessor");
  }

  // if no preprocessing was specified, just pass the data
  if(procNode == NULL || procNode->Type() != TiXmlNode::ELEMENT) {
    RtStreamComponent *sc
        = buildStreamComponent(RtPasser::moduleString, "original data passer");
    addSingleModule(sc);
  }
  else { // find modules in the node and add each
    addModulesFromNode((TiXmlElement*) procNode, &config);
  }
  // all modules are now added to the stream
  pushAllModules();

  return true;
}

// validate config
bool RtProcessor::validateComponentConfig() {
  return true;
}

// process a single acquisition
int RtProcessor::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtProcessor::process"));

  // pass the message block down to our stream
  RtStream::put(mb);

  return 0;
}

// get the version
//  out: char array that represents the cvs version
char *RtProcessor::getVersionString() const {
  return VERSION;
}
