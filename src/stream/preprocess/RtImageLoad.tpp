/*=========================================================================
 *  RtImageLoad.cpp is the implementation of a stream module that loads a mask
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

#include"RtImageLoad.h"
#include"RtDataIDs.h"
#include"RtMaskImage.h"
#include"RtExperiment.h"

template<class T>
string RtImageLoad<T>::moduleString(ID_MASKLOAD);

// default constructor
template<class T>
RtImageLoad<T>::RtImageLoad() : RtStreamComponent(),
                                roiID(""),
                                align(true),
                                mosaic(false),
                                unmosaic(true),
                                flipLR(false),
                                dynamic(false),
                                save(true),
                                imageLoad(NULL) {
  componentID = moduleString;
}

// destructor
template<class T>
RtImageLoad<T>::~RtImageLoad() {
}

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node
template<class T>
bool RtImageLoad<T>::processOption(const string &name, const string &text,
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
template<class T>
bool RtImageLoad<T>::validateComponentConfig() {
  bool result = true;

  if(filename.empty()) {
    cout << "ERROR: image-load requires a filename" << endl;
    result = false;
  }
  if(roiID.empty()) {
    cout << "ERROR: image-load requires an roiID" << endl;
    result = false;
  }

  return result;
}


// process a single acquisition
template<class T>
int RtImageLoad<T>::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtImageLoad::process"));

  // otherwise try to load the mask
  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // return quickly if we are have done our job already
  if(!needsInit && !dynamic && imageLoad != NULL) {
    setResult(msg,imageLoad);
    return 0;
  }

  // get the current image to set our id by
  RtMRIImage *img = (RtMRIImage*) msg->getCurrentData();

  if(img == NULL) {
    cout << "RtImageLoad::process: image passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtImageLoad::process: image passed is NULL\n"));
    return 0;
  }

  // allocate a new mask image
  imageLoad = new T(*img);
  imageLoad->getDataID().setFromInputData(*img,*this);

  imageLoad->getDataID().setTimePoint(DATAID_NUM_UNSET_VALUE);
  imageLoad->getDataID().setModuleID(ID_MASK);
  imageLoad->getDataID().setDataName(NAME_MASK);
  imageLoad->getDataID().setRoiID(roiID);
  imageLoad->setFilename(
    getExperimentConfig().get("study:maskDir").str() + filename);

  // align mask before reading it in?
  unsigned int seriesNum = imageLoad->getDataID().getSeriesNum();
  if(align) {
    FslJobID status = RtFSLInterface::applyTransform(
      getExperimentConfig().getSeriesRefVolFilename(seriesNum),
      getExperimentConfig().get("study:xfm:referenceVol"),
      imageLoad->getFilename(),
      getExperimentConfig().getSeriesXfmOutputFilename(
        seriesNum,imageLoad->getFilename()),
      getExperimentConfig().getSeriesXfmFilename(seriesNum),
      true);
    if(status == FSL_JOB_FINISHED) {
      imageLoad->setFilename(getExperimentConfig().getSeriesXfmOutputFilename(
        seriesNum,
        imageLoad->getFilename()));
    }
    else {
      cerr << "error aligning mask, just using unaligned version." << endl;

      if(logOutput) {
        stringstream logs("");
        logs << "error aligning mask " << imageLoad->getFilename()
             << " , just using unaligned version." << endl;
        log(logs);
      }
    }
  }

  // try to load the file
  if(!imageLoad->load()) {
    cerr << "error loading mask file " << imageLoad->getFilename() << endl;

    if(logOutput) {
      stringstream logs("");
      logs << "error loading mask file from " << imageLoad->getFilename()
           << endl;
      log(logs);
    }

    return 0;
  }

  // mosaic if we need to
  if(mosaic) {
    imageLoad->mosaic();
  }

  // unmosaic if we need to
  if(unmosaic) {
    imageLoad->unmosaic();
  }

  // flip if we need to
  if(flipLR) {
    imageLoad->flipLR();
  }

  setResult(msg,imageLoad);

  // save to a file if we should
  if(needsInit && save) {
    imageLoad->setFilename(getExperimentConfig().getSeriesMaskFilename(seriesNum,
                                                                       roiID));
    imageLoad->save();

    if(logOutput) {
      stringstream logs("");
      logs << "maskload: loaded mask " << roiID << " of "
           << imageLoad->getNumberOfOnVoxels() << " voxels and saved to "
           << imageLoad->getFilename() << endl;
      log(logs);
    }

  }

  needsInit = false;

  return 0;
}
