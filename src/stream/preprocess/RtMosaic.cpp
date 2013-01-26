/*=========================================================================
 *  RtMosaic.cpp is the implementation of a class that converts a volume into
 *  a flat mosaic representation
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

#include"RtMosaic.h"
#include"RtDataIDs.h"

string RtMosaic::moduleString(ID_MOSAIC);

// default constructor
RtMosaic::RtMosaic() : RtStreamComponent() {
  componentID = moduleString;
}

// destructor
RtMosaic::~RtMosaic() {
}

// validate the configuration
bool RtMosaic::validateComponentConfig() {
  bool result = true;


  return result;
}

// process a single acquisition
int RtMosaic::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtMosaic::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *img = (RtMRIImage*) msg->getCurrentData();

  if(img == NULL) {
    cout << "RtMosaic::process: image passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtMosaic::process: image passed is NULL\n"));
    return 0;
  }

  // allocate a new data image for the variance
  RtMRIImage *mosaic = new RtMRIImage(*img);
  mosaic->getDataID().setFromInputData(*img,*this);

  mosaic->mosaic();

  setResult(msg,mosaic);

  if(print) {
    cout << "RtMosaic: done at tr "
         << img->getDataID().getTimePoint() << endl;
  }

  return 0;
}
