/*=========================================================================
 *  RtDiff.h is the header for a class that computes the difference
 *  between two images
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

#include"RtDiff.h"

#include"RtDataIDs.h"

#define absdiff(a,b) (a > b ? a-b : b-a)

string RtDiff::moduleString(ID_TEMPDIFF);

// default constructor
RtDiff::RtDiff() : RtStreamComponent() {
  componentID = moduleString;

  last = NULL;
}

// destructor
RtDiff::~RtDiff() {}

// validate the configuration
bool RtDiff::validateComponentConfig() {
  return true;
}

// process a single acquisition
int RtDiff::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtDiff::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  RtMRIImage *img = (RtMRIImage*)msg->getCurrentData();

  if(img == NULL) {
    ACE_DEBUG((LM_INFO, "RtDiff:process: image passed is NULL\n"));
    return 0;
  }

  if(last == NULL
     || last->getDataID().getSeriesNum() != img->getDataID().getSeriesNum()) {
    ACE_DEBUG((LM_DEBUG, "differencing found first image\n"));

    last = img;
    return 0;
  }

  // validate sizes
  if(img->getNumPix() != last->getNumPix()) {
    ACE_DEBUG((LM_INFO,
               "RtDiff:process: last image is different size than this one\n"));
    return -1;
  }

  // allocate a new data image for the difference
  RtMRIImage *diff = new RtMRIImage(*img);

  diff->getDataID().setFromInputData(*img,*this);
  diff->getDataID().setDataName(NAME_TEMPDIFF_IMG);


  // compute the absolute difference
  for(unsigned int i = 0; i < img->getNumPix(); i++) {
    unsigned short d = absdiff(img->getPixel(i),last->getPixel(i));

    diff->setPixel(i, d);
  }

  // store this as last image
  last = img;

  setResult(msg, diff);

  return 0;
}
