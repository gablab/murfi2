/*=========================================================================
 *  RtClassifySVM.cpp is the implementation of a class to classify individual
 *  TRs based on an already trained svm model
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

#include "RtDesignMatrix.h"

#include"RtClassifySVM.h"
#include"RtMRIImage.h"
#include"RtActivation.h"
#include"RtDataIDs.h"
#include"RtExperiment.h"
#include"util/timer/timer.h"

#include"debug_levels.h"

extern "C" {
#include<svm_struct_api.h>
}

string RtClassifySVM::moduleString(ID_CLASSIFYSVM);

// default constructor

RtClassifySVM::RtClassifySVM() : RtStreamComponent() {

  componentID = moduleString;

}

// destructor

RtClassifySVM::~RtClassifySVM() {
  if(svmResultsStream.is_open()) {
    svmResultsStream.close();
  }
}

// process an option in name of the option to process val text of the option
// node
bool RtClassifySVM::processOption(const string &name, const string &text,
                                  const map<string, string> &attrMap) {

  // look for known options
  if(name == "svmModelFile") {
    svmModelFile = text;
    return true;
  }

  if(name == "svmResultsFile") {
    svmResultsFile = text;
    return true;
  }

  return RtStreamComponent::processOption(name, text, attrMap);
}

// validate the configuration
bool RtClassifySVM::validateComponentConfig() {
  bool result = true;

  if(svmModelFile.empty()) {
    cerr << "RtClassifySVM::process: svmModelFile is required" << endl;
    result = false;
  }
  else { // read model
    cout << "reading svm model file...";
    if(!readFmriModel(svmModelFile, svmParms)) {
      cout << "error reading model file" << endl;
      result = false;
    }
    else {
      cout << "done" << endl;
    }
  }
  if(result && !svmResultsFile.empty()) {
    svmResultsStream.open(svmResultsFile.c_str());
    if(svmResultsStream.fail()) {
      cerr << "error opening result file " << svmResultsFile << " for writing"
           << endl;
    }
  }

  return result;
}

// process a single acquisition
int RtClassifySVM::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtClassifySVM::process"));

  timer tim;
  if(printTiming) {
    tim.start();
  }

  // get pointer to message
  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtActivation *dat = (RtActivation*) msg->getData(inputModuleID,
                                                   inputDataName,
                                                   inputRoiID);

  // check for validity of data
  if (dat == NULL) {
    cerr << "RtClassifySVM::process: data passed is NULL" << endl;

    if(logOutput) {
      stringstream logs("");
      logs << "RtClassifySVM::process: data passed is NULL" << endl;
      log(logs);
    }

    return 0;
  }

  // get mask from msg
  RtMaskImage *mask = getMaskFromMessage(*msg);

  // check validity of mask
  if (mask == NULL) {
    cerr << "RtClassifySVM::process: mask is NULL" << endl;

    if(logOutput) {
      stringstream logs("");
      logs << "RtClassifySVM::process: mask is NULL at tr "
           << dat->getDataID().getTimePoint() << endl;
      log(logs);
    }

    return 0;
  }

  // allocate data images for the classification
  RtActivation *bestClass = new RtActivation(1);
  RtActivation *classScores = new RtActivation(svmParms.strctParm.num_classes);

  // setup the results data
  bestClass->getDataID().setFromInputData(*dat, *this);
  bestClass->getDataID().setDataName(NAME_CLASSLABEL);
  bestClass->initToNans();

  classScores->getDataID().setFromInputData(*dat, *this);
  classScores->getDataID().setDataName(NAME_CLASSSCORE);
  classScores->initToNans();

  // build a pattern and classify it

  // assign mask voxels to pattern
  PATTERN x = createPattern<double>(1,mask->getNumberOfOnVoxels(),
                                    mask->getNumEl(),
                                    dat->getDataConst(),
                                    mask->getDataConst());


  // classify the pattern
  LABEL l = classify_struct_example(x, &svmParms.model, &svmParms.strctParm);

  // store the classification in our format
  bestClass->setPixel(0, l.cls);

  for(int i = 0; i < svmParms.strctParm.num_classes; i++) {
    classScores->setPixel(i, l.scores[i]);
  }

  setResult(msg, bestClass);
  setResult(msg, classScores);

  deletePattern(x);

  if(printTiming) {
    tim.stop();
    cout << "RtClassifySVM process at tr "
         << dat->getDataID().getTimePoint()
         << " elapsed time: " << tim.elapsed_time()*1000 << "ms"  << endl;
  }

  if(print) {
    cout << "RtClassifySVM: done at tr "
         << dat->getDataID().getTimePoint() << " class " << l.cls << ":";

    for(int i = 0; i < svmParms.strctParm.num_classes; i++) {
      cout << " " << l.scores[i];
    }
    cout << endl;
  }

  if(logOutput) {
    stringstream logs("");
    logs << "RtClassifySVM: done at tr "
         << dat->getDataID().getTimePoint() << " class " << l.cls << endl;
    log(logs);
  }

  if(svmResultsStream.is_open()) {
    svmResultsStream
        << dat->getDataID().getTimePoint() << "\t"
        << l.cls;
    for(int i = 0; i < svmParms.strctParm.num_classes; i++) {
      svmResultsStream << "\t" << l.scores[i];
    }
    svmResultsStream << endl;
  }

  return 0;
}
