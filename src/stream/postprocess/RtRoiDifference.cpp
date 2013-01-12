/*=========================================================================
 *  RtRoiDifference.cpp computes the difference between two
 *  activaton sums
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

#include"RtRoiDifference.h"
#include "RtDataIDs.h"

string RtRoiDifference::moduleString(ID_ROIDIFFERENCE);

// default constructor
RtRoiDifference::RtRoiDifference() : RtStreamComponent() {
  componentID = moduleString;
}

// destructor
RtRoiDifference::~RtRoiDifference() {}

// process an option
//  in 
//   name of the option to process
//   val  text of the option node
bool RtRoiDifference::processOption(const string &name, 
					  const string &text,
					  const map<string,string> &attrMap) {

  // look for options
  if(name == "posRoiModuleID") {
    posRoiModuleID = text;
    return true;
  }
  else if(name == "negRoiModuleID") {
    negRoiModuleID = text;
    return true;
  }
  else if(name == "posRoiDataName") {
    posRoiDataName = text;
    return true;
  }
  else if(name == "negRoiDataName") {
    negRoiDataName = text;
    return true;
  }
  else if(name == "posRoiRoiID") {
    posRoiRoiID = text;
    return true;
  }
  else if(name == "negRoiRoiID") {
    negRoiRoiID = text;
    return true;
  }

  return RtStreamComponent::processOption(name,text,attrMap);
}  

// validate config
bool RtRoiDifference::validateComponentConfig() {
  bool result = true;

  if(posRoiModuleID.empty()) {
    cout << "ERROR: posRoiModuleID must be set to compute an roi difference" << endl;
    result = false;
  }

  if(negRoiModuleID.empty()) {
    cout << "ERROR: negRoiModuleID must be set to compute an roi difference" << endl;
    result = false;
  }

  if(posRoiDataName.empty()) {
    cout << "ERROR: posRoiDataName must be set to compute an roi difference" << endl;
    result = false;
  }

  if(negRoiDataName.empty()) {
    cout << "ERROR: negRoiDataName must be set to compute an roi difference" << endl;
    result = false;
  }

  if(posRoiRoiID.empty()) {
    cout << "ERROR: posRoiRoiID must be set to compute an roi difference" << endl;
    result = false;
  }

  if(negRoiRoiID.empty()) {
    cout << "ERROR: negRoiRoiID must be set to compute an roi difference" << endl;
    result = false;
  }

  return result;
}

// process a single acquisition
int RtRoiDifference::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtRoiDifference::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  cout << "PROCESSING " << endl;

  // find the data with the right data IDs
  RtActivation *posact 
    = (RtActivation*) msg->getData(posRoiModuleID,
				   posRoiDataName,
				   posRoiRoiID);
  RtActivation *negact 
    = (RtActivation*) msg->getData(negRoiModuleID,
				   negRoiDataName,
				   negRoiRoiID);

  double p = 0;
  if(posact != NULL) {
    p = posact->getPixel(0);
  }

  double n = 0;
  if(negact != NULL) {
    n = negact->getPixel(0);
  }

 //  if(posact == NULL || negact == NULL) {
//     cout << "activation-sum-difference: couldn't find required rois" << endl;
//     ACE_DEBUG((LM_INFO, "RtRoiDifference::process: pos or neg activation passed is NULL\n"));
//     return 0;
//   }
  
//  ACE_DEBUG((LM_DEBUG, "differencing  activations for image %d\n", 
//	     img->getAcquisitionNum()));
	    
  // create a one element activation image
  RtActivation *diff = new RtActivation(1);

  // setup the data id
  diff->getDataID().setFromInputData(*posact,*this);
  diff->getDataID().setDataName(NAME_ROIDIFF);
  diff->getDataID().setRoiID(posRoiRoiID + "-" + negRoiRoiID);
  

  // compute the difference
  diff->setPixel(0,  p - n);

  cout << diff->getPixel(0) << " = " << p << " - " << n << endl;

  // set the image id for handling
  //diff->addToID("activation-sum-difference");

  setResult(msg, diff);

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


