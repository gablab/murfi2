/*=========================================================================
 *  RtVar.h is the implementation of a class that computes the variance
 *  of a set of images
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

#include"RtVar.h"
#include"RtDataIDs.h"

#define absdiff(a,b) (a > b ? a-b : b-a)

string RtVar::moduleString(ID_TEMPVAR);

// default constructor
RtVar::RtVar() : RtStreamComponent() {
  numTimePoints = 0;
  componentID = moduleString;

  meanModuleID = ID_TEMPMEAN;
  meanDataName = NAME_TEMPMEAN_IMG;
}

// destructor
RtVar::~RtVar() {
}

// process a configuration option
//  in
//   name of the option to process
//   val  text of the option node
bool RtVar::processOption(const string &name, const string &text,
                          const map<string,string> &attrMap) {

  // look for known options
  if(name == "meanModuleID") {
    meanModuleID = text;
    return true;
  }
  if(name == "meanDataName") {
    meanDataName = text;
    return true;
  }

  return RtStreamComponent::processOption(name, text, attrMap);
}

// validate the configuration
bool RtVar::validateComponentConfig() {
  bool result = true;

  return result;
}

// process a single acquisition
int RtVar::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtVar::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // get the current image to operate on
  RtMRIImage *img = (RtMRIImage*)msg->getCurrentData();

  if(img == NULL) {
    cout << "RtVar:process: image passed is NULL" << endl;

    ACE_DEBUG((LM_INFO, "RtVar:process: image passed is NULL\n"));
    return 0;
  }

  // get the mean
  //cout << "getting mean data with id " << meanDataID << endl;
  RtMRIImage *mean = (RtMRIImage*)msg->getData(meanModuleID,meanDataName);

  if(mean == NULL) {
    cout << "RtVar::process: mean image not found" << endl;

    ACE_DEBUG((LM_INFO, "RtVar::process: mean image not found\n"));
    return 0;
  }


  if(numTimePoints == 0 || mean->getDataID().getSeriesNum()
     != img->getDataID().getSeriesNum()) {
    ACE_DEBUG((LM_DEBUG, "var found first image\n"));

    varnum.setInfo(*img);

    for(unsigned int i = 0; i < varnum.getNumPix(); i++) {
      varnum.setPixel(i,0.0);
    }

    numTimePoints++;

    return 0;
  }

  // validate sizes
  if(img->getNumPix() != mean->getNumPix()) {
    ACE_DEBUG((LM_INFO,
               "RtVar:process: last image is different size than this one\n"));
    return -1;
  }

  // save the data
  numTimePoints++;


  // allocate a new data image for the variance
  RtMRIImage *var = new RtMRIImage(*img);
  var->getDataID().setFromInputData(*img,*this);
  var->getDataID().setDataName(NAME_TEMPVAR_IMG);


  // update the mean and variance numerator due to west (1979) for each voxel
  for(unsigned int i = 0; i < img->getNumPix(); i++) {
    // trickery to allow temp negative values
    int pixmean = (int) mean->getPixel(i);
    int thispix = (int) img->getPixel(i);

    int newmean = pixmean
        + (int) rint( (thispix-pixmean) / (double)numTimePoints);
    mean->setPixel(i, (unsigned short) newmean);

    double pixvarnum = varnum.getPixel(i);
    double newvarnum = pixvarnum
        + (numTimePoints-1) * (thispix - pixmean)
        * (thispix - pixmean) / (double) numTimePoints;
    varnum.setPixel(i,newvarnum);

    var->setPixel(i, (unsigned short) (newvarnum / (double)(numTimePoints-1)));
  }

  setResult(msg,var);

  return 0;
}
