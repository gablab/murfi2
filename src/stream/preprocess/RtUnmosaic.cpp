/*=========================================================================
 *  RtUnmosaic.cpp is the implementation of a class that converts
 *  from a flat mosaic representation into a 3D volume
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

#include"RtUnmosaic.h"
#include"RtDataIDs.h"

string RtUnmosaic::moduleString(ID_UNMOSAIC);

// default constructor
RtUnmosaic::RtUnmosaic() : RtStreamComponent() {
  componentID = moduleString;
}

// destructor
RtUnmosaic::~RtUnmosaic() {
  //cout << "destroyed" << endl;
}

// validate the configuration
bool RtUnmosaic::validateComponentConfig() {
  bool result = true;

  return result;
}

// process a single acquisition
int RtUnmosaic::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtUnmosaic::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *img = (RtMRIImage*) msg->getCurrentData();

  if(img == NULL) {
    cout << "RtUnmosaic::process: image passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtUnmosaic::process: image passed is NULL\n"));
    return 0;
  }

  // allocate a new data image for the variance
  RtMRIImage *unmosaic = new RtMRIImage(*img);
  unmosaic->getDataID().setFromInputData(*img,*this);

  unmosaic->unmosaic();

  // set the image id for handling
  //unmosaic->addToID("unmosaic");

  setResult(msg,unmosaic);

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

