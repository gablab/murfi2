/*=========================================================================
 *  RtMean.h is the implementation of a class that computes the mean
 *  of a set of images incrementally
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

#include"RtMean.h"
#include"RtDataIDs.h"

#define absdiff(a,b) (a > b ? a-b : b-a)

string RtMean::moduleString(ID_TEMPMEAN);

// default constructor
RtMean::RtMean() : RtStreamComponent() {
  componentID = moduleString;

  numTimePoints = 0;
}

// destructor
RtMean::~RtMean() {
}

// validate the configuration
bool RtMean::validateComponentConfig() {
  bool result = true;

  return result;
}

// process a single acquisition
int RtMean::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtMean::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *img = (RtMRIImage*)msg->getCurrentData();

  if(img == NULL) {
    cout << "RtMean::process: image passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtMean::process: image passed is NULL\n"));
    return 0;
  }

  if(numTimePoints == 0 || storemean.getDataID().getSeriesNum()
     != img->getDataID().getSeriesNum()) {
    ACE_DEBUG((LM_DEBUG, "mean found first image\n"));
    storemean = (*img);
  }

  // validate sizes
  if(img->getNumPix() != storemean.getNumPix()) {
    ACE_DEBUG((LM_INFO, "RtMean::process: storemean is different size "
               << "than this one\n"));
    return -1;
  }

  // save the data
  numTimePoints++;


  // allocate a new data image for the variance
  RtMRIImage *mean = new RtMRIImage(*img);

  mean->getDataID().setFromInputData(*img,*this);
  mean->getDataID().setDataName(NAME_TEMPMEAN_IMG);

  // update the mean and variance numerator due to west (1979) for each voxel
  for(unsigned int i = 0; i < img->getNumPix(); i++) {
    // trickery to allow temp negative values
    int pixmean = (int) storemean.getPixel(i);
    int thispix = (int) img->getPixel(i);

    int newmean = pixmean
        + (int) rint( (thispix-pixmean) / (double)numTimePoints);
    storemean.setPixel(i, (unsigned short) newmean);
    mean->setPixel(i, (unsigned short) newmean);
  }

  // set the image id for handling
  setResult(msg,mean);

  return 0;
}
