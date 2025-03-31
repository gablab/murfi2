/*=========================================================================
 *  RtMaskLoad.cpp is the implementation of a stream module that loads a mask
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

#include"RtMaskLoad.h"
#include"RtDataIDs.h"
#include"RtMaskImage.h"
#include"RtActivation.h"
#include"RtExperiment.h"

string RtMaskLoad::moduleString(ID_MASKLOAD);

// default constructor
RtMaskLoad::RtMaskLoad() : RtStreamComponent(),
                           roiID(""),
                           loadType(LOAD_MASK), // Default to loading standard mask
                           align(true),
                           mosaic(false),
                           unmosaic(true),
                           flipLR(false),
                           dynamic(false),
                           save(true),
                           loadedData(NULL) {
  componentID = moduleString;
                           }

// destructor
RtMaskLoad::~RtMaskLoad() {
}

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node
bool RtMaskLoad::processOption(const string &name, const string &text,
                               const map<string,string> &attrMap) {

  // look for known options
  if(name == "filename") {
    filename = text;
    return true;
  }
  if(name == "roiID") {
    roiID = text;
    return true;
  }
  if(name == "loadType") {
    if(text == "mask") {
      loadType = LOAD_MASK;
    }
    else if(text == "activation") {
      loadType = LOAD_ACTIVATION;
    }
    else {
      cerr << "ERROR: unknown loadType: " << text
           << " specified for mask-load. Use 'mask' or 'activation'." << endl;
      return false;
    }
    return true;
  }
  if(name == "align") {
    return RtConfigVal::convert<bool>(align, text);
  }
  if(name == "mosaic") {
    return RtConfigVal::convert<bool>(mosaic, text);
  }
  if(name == "unmosaic") {
    return RtConfigVal::convert<bool>(unmosaic, text);
  }
  if(name == "flipLR") {
    return RtConfigVal::convert<bool>(flipLR, text);
  }
  if(name == "dynamic") {
    return RtConfigVal::convert<bool>(dynamic, text);
  }
  if(name == "save") {
    return RtConfigVal::convert<bool>(save, text);
  }

  return RtStreamComponent::processOption(name, text, attrMap);
}

// validate the configuration
bool RtMaskLoad::validateComponentConfig() {
  bool result = true;

  if(filename.empty()) {
    cout << "ERROR: mask-load requires a filename" << endl;
    result = false;
  }
  if(roiID.empty()) {
    cout << "ERROR: mask-load requires an roiID" << endl;
    result = false;
  }

  return result;
}


// process a single acquisition
int RtMaskLoad::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtMaskLoad::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // return quickly if we have done our job already (and not dynamic)
  if(!needsInit && !dynamic && loadedData != NULL) {
    setResult(msg, loadedData);
    return 0;
  }

  // get the current image to set our id by
  RtMRIImage *img = (RtMRIImage*) msg->getCurrentData();

  if(img == NULL) {
    cout << "RtMaskLoad::process: image passed is NULL" << endl;
    ACE_DEBUG((LM_INFO, "RtMaskLoad::process: image passed is NULL\n"));
    return 0;
  }

  // --- Allocate based on loadType ---
  RtMaskImage* maskImage = NULL;
  RtActivation* activationImage = NULL;
  string dataName = NAME_MASK;

  if(loadType == LOAD_MASK) {
    maskImage = new RtMaskImage(*img);
    loadedData = maskImage;
    dataName = NAME_MASK;
    if(logOutput) { log("RtMaskLoad: Loading as RtMaskImage (short)."); }
  } else if(loadType == LOAD_ACTIVATION) {
    activationImage = new RtActivation(*img); // Use RtActivation constructor
    loadedData = activationImage;
    dataName = NAME_ACTIVATION;
    if(logOutput) { log("RtMaskLoad: Loading as RtActivation (double)."); }
  } else {
    cerr << "ERROR: unknown loadType: " << loadType
         << " specified for mask-load. Use 'mask' or 'activation'." << endl;
    return 1;
  }

  // --- Set common properties ---
  loadedData->getDataID().setFromInputData(*img,*this);
  loadedData->getDataID().setTimePoint(DATAID_NUM_UNSET_VALUE);
  loadedData->getDataID().setModuleID(ID_MASK); // Keep module ID as MASK
  loadedData->getDataID().setDataName(dataName);
  loadedData->getDataID().setRoiID(roiID);

  string fullPath = getExperimentConfig().get("study:maskDir").str() + filename;
  // We need to set the filename on the specific type for load() to work
  if(maskImage) maskImage->setFilename(fullPath);
  if(activationImage) activationImage->setFilename(fullPath);

  // --- Alignment ---
  unsigned int seriesNum = loadedData->getDataID().getSeriesNum();
  if(align) {
    string transformedFilename = getExperimentConfig().getSeriesXfmOutputFilename(
                                    seriesNum, fullPath);

    FslJobID status
        = RtFSLInterface::applyTransform(
            getExperimentConfig().getSeriesRefVolFilename(seriesNum),
            getExperimentConfig().get("study:xfm:referenceVol"),
            fullPath,
            transformedFilename,
            getExperimentConfig().getSeriesXfmFilename(seriesNum),
            true);

    if(status == FSL_JOB_FINISHED) {
      // Update the filename to the transformed version for loading
      if(maskImage) maskImage->setFilename(transformedFilename);
      if(activationImage) activationImage->setFilename(transformedFilename);
      fullPath = transformedFilename; // Update fullPath for logging
    }
    else {
      cerr << "error aligning mask/activation, just using unaligned version." << endl;
      if(logOutput) {
        stringstream logs("");
        logs << "error aligning file " << fullPath
             << " , just using unaligned version." << endl;
        log(logs);
      }
    }
  }

  // --- Load the file ---
  bool loadSuccess = false;
  if(maskImage) loadSuccess = maskImage->load();
  if(activationImage) loadSuccess = activationImage->load();

  if(!loadSuccess) {
    cerr << "error loading mask/activation file " << fullPath << endl;
    if(logOutput) {
      stringstream logs("");
      logs << "error loading file from " << fullPath << endl;
      log(logs);
    }
    delete loadedData; // Clean up allocated memory
    loadedData = NULL;
    return 0;
  }

  if(maskImage) {
      if(mosaic) maskImage->mosaic();
      if(unmosaic) maskImage->unmosaic();
      if(flipLR) maskImage->flipLR();
  }
  if(activationImage) {
      if(mosaic) activationImage->mosaic();
      if(unmosaic) activationImage->unmosaic();
      if(flipLR) activationImage->flipLR();
  }

  // --- Set result and potentially save ---
  setResult(msg, loadedData);

  // Save only if it's the first time (needsInit) and save is true
  if(needsInit && save) {
    string savePath = getExperimentConfig().getSeriesMaskFilename(seriesNum, roiID);
    bool saveSuccess = false;
    if(maskImage) {
        maskImage->setFilename(savePath);
        saveSuccess = maskImage->save();
    }
    if(activationImage) {
        activationImage->setFilename(savePath);
        saveSuccess = activationImage->save();
    }

    if(logOutput) {
      stringstream logs("");
      unsigned int numVox = 0;
      // Use correctly typed pointers now
      if(maskImage) {
          numVox = maskImage->getNumberOfOnVoxels();
      }
      if(activationImage) {
          // RtActivation doesn't have getNumberOfOnVoxels.
          // Reporting total elements loaded instead.
          numVox = activationImage->getNumEl();
      }

      logs << "maskload: loaded " << roiID << " ("
           << (loadType == LOAD_MASK ? "mask" : "activation")
           << ") of " << numVox << " voxels";
      if(saveSuccess) logs << " and saved to " << savePath;
      else logs << " (save failed or not requested)";
      logs << endl;
      log(logs);
    }
  }

  needsInit = false;

  return 0;
}
