/*=========================================================================
 *  RtMaskGen.cpp is the implementation of a stream module that generates a mask
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

#include"RtMaskGen.h"
#include"RtDataIDs.h"
#include"RtMaskImage.h"
#include"RtExperiment.h"

string RtMaskGen::moduleString(ID_MASKGEN);

static const string DEFAULT_BET_PARMS("-f 0.5 -g 0 -n -m");

// default constructor
RtMaskGen::RtMaskGen() : RtStreamComponent(),
			 roiID("unset"),
			 threshold(0.125),
			 dynamic(false),
			 useBet(false),
			 betParms(DEFAULT_BET_PARMS),
			 save(true),
			 computingMask(false),
			 maskGen(NULL) {
  componentID = moduleString;
}

// destructor
RtMaskGen::~RtMaskGen() {
}

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node
bool RtMaskGen::processOption(const string &name, const string &text,
			      const map<string,string> &attrMap) {

  // look for known options
  if(name == "roiID") {
    roiID = text;
    return true;
  }
  if(name == "threshold") {
    return RtConfigVal::convert<double>(threshold, text);
  }
  if(name == "useBet") {
    return RtConfigVal::convert<bool>(useBet, text);
  }
  if(name == "betParms") {
    betParms = text;
    return true;
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
bool RtMaskGen::validateComponentConfig() {
  bool result = true;

  if(roiID.empty()) {
    cout << "ERROR: mask-gen requires an roiID" << endl;
    result = false;

  }  
  return result;
}

// process a single acquisition
int RtMaskGen::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtMaskGen::process"));

  // otherwise try to gen the mask
  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();


  // return quickly if we have done our job already
  if(!needsInit && !dynamic && maskGen != NULL) {
    setResult(msg,maskGen);
    return 0;
  }

  // get the current image to set our id by
  RtMRIImage *img = (RtMRIImage*) msg->getCurrentData();

  if(img == NULL) {
    cout << "RtMaskGen::process: image passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtMaskGen::process: image passed is NULL\n"));
    return 0;
  }

  // allocate a new mask image 
  maskGen = new RtMaskImage(*img);
  maskGen->getDataID().setFromInputData(*img,*this);

  maskGen->getDataID().setTimePoint(DATAID_NUM_UNSET_VALUE);
  maskGen->getDataID().setModuleID(ID_MASK);
  maskGen->getDataID().setDataName(NAME_MASK);
  maskGen->getDataID().setRoiID(roiID);

  // if we are getting a brain mask from bet write the first image and
  // wait for bet to finish before we read it back in and continue
  if(useBet) {
    if(!computingMask) {
      // execute commands to make the mask
      maskJobID = RtFSLInterface::makeBrainMask(
			getExperimentConfig().getSeriesRefVolFilename(
				   img->getDataID().getSeriesNum()), 
			maskGen->getFilename(),
			betParms);
	
      computingMask = true;
      
      return 0;
    }
    else if(computingMask 
	    && RtFSLInterface::getJobStatus(maskJobID) == FSL_JOB_FINISHED) {

      computingMask = false;
      maskGen->load();
    }
    else if(computingMask 
	    && RtFSLInterface::getJobStatus(maskJobID) == FSL_JOB_ERROR) {	
      computingMask = false;
      cerr << "RtIntensityNorm: error computing brain mask. guessing by threshold" << endl;
      maskGen->initByMeanIntensityThreshold(*img, threshold);
    }
    else {
      cerr << "not done computing mask..." << endl;
      return 0;
    }
  }
  else {
    maskGen->initByMeanIntensityThreshold(*img, threshold);
  }

  maskGen->updateOnVoxelIndices();

  setResult(msg,maskGen);

  // save to a file if we should
  if(needsInit && save) {
    maskGen->setFilename(getExperimentConfig().getSeriesMaskFilename(
                   img->getDataID().getSeriesNum(),roiID));
    maskGen->save();

    if(logOutput) {
      stringstream logs("");
      logs << "maskgen: produced mask " << roiID << " of " 
	   << maskGen->getNumberOfOnVoxels() << " voxels and saved to "
	   << maskGen->getFilename() << endl;
      log(logs);
    }
  }

  needsInit = false;

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

