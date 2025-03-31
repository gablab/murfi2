/*=========================================================================
 *  RtRoiCombine.cpp is a base class for methods of transforming roi
 *  activations into feedback
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

#include "RtRoiCombine.h"
#include "RtDataIDs.h"
#include "RtElementAccess.h"
#include <math.h>

string RtRoiCombine::moduleString(ID_ROICOMBINE);

// default constructor
RtRoiCombine::RtRoiCombine() : RtStreamComponent(),
                               method(RT_ROI_UNKNOWN),
                               weightsModuleID(ID_INCREMENTALGLM),
                               weightsDataName(NAME_RESIDUAL_MSE) {
  componentID = moduleString;
                               }

// destructor
RtRoiCombine::~RtRoiCombine() {}

// process an option
//  in
//   name of the option to process
//   val  text of the option node
bool RtRoiCombine::processOption(const string &name, const string &text,
                                 const map<string,string> &attrMap) {
  if(name == "method") {
    if(text == "mean") {
      method = RT_ROI_MEAN;
    }
    else if(text == "variance") {
      method = RT_ROI_VARIANCE;
    }
    else if(text == "median") {
      method = RT_ROI_MEDIAN;
    }
    else if(text == "weighted-ave") {
      method = RT_ROI_WEIGHTEDAVE;
    }
    else if(text == "sum") {
      method = RT_ROI_SUM;
    }
    else if(text == "max") {
      method = RT_ROI_MAX;
    }
    else if(text == "min") {
      method = RT_ROI_MIN;
    }
    else {
      cerr << "ERROR: unknown roi combination method: " << text << endl;
      return false;
    }
    return true;
  }
  if(name == "weightsDataName") {
    weightsDataName = text;
  }
  if(name == "weightsModuleID") {
    weightsModuleID = text;
  }

  return RtStreamComponent::processOption(name, text, attrMap);
}

// validate the configuration
bool RtRoiCombine::validateComponentConfig() {
  bool result = true;

  if(method == RT_ROI_UNKNOWN) {
    cerr << "ERROR: roi combination method must be set" << endl;
    result = false;
  }

  return result;
}

// process a single acquisition
int RtRoiCombine::process(ACE_Message_Block *mb) {
  ACE_TRACE(("RtRoiCombine::process"));

  RtStreamMessage *msg = (RtStreamMessage*) mb->rd_ptr();

  // find the data with the right ID
  RtData *dat = msg->getData(inputModuleID,
                             inputDataName,
                             inputRoiID);

  if(dat == NULL) {
    cerr << "RtRoiCombine::process: could not find the data for roi combination"
         << endl;

    if(logOutput) {
      stringstream logs("");
      logs << "RtRoiCombine::process: could not find data for roi combination"
           << endl;
      log(logs);
    }

    return 0;
  }

  RtMaskImage *mask = getMaskFromMessage(*msg);
  if(mask == NULL) {
    cerr << "ERROR: could not find the mask for roi combination" << endl;

    if(logOutput) {
      stringstream logs("");
      logs << "RtRoiCombine::process: could not find the mask for roi "
           << "combination at tr " << dat->getDataID().getTimePoint() << endl;
      log(logs);
    }

    return 0;
  }


  RtActivation *result = NULL;
  RtActivation *weightedAve = NULL;
  switch(method) {
    case RT_ROI_MEAN:
      result = getMean(dat, mask);
      break;
    case RT_ROI_VARIANCE:
      result = getVariance(dat, mask);
      break;
    case RT_ROI_MEDIAN:
      result = getMedian(dat, mask);
      break;
    case RT_ROI_WEIGHTEDAVE:
      weightedAve = (RtActivation*) msg->getData(weightsModuleID,
                                                 weightsDataName,
                                                 inputRoiID);

      if(weightedAve == NULL) {
        cerr << "RtRoiCombine::process: could not find the voxel weights for "
             << "weighted average roi combination" << endl;
        if(logOutput) {
          stringstream logs("");
          logs << "RtRoiCombine::process: could not find the voxel weights for "
               << "weighted average roi combination at tr "
               << dat->getDataID().getTimePoint() << endl;
          log(logs);
        }

        return 0;
      }

      result = getWeightedAve(dat, weightedAve, mask);
      break;
    case RT_ROI_SUM:
      result = getSum(dat, mask);
      break;
    case RT_ROI_MAX:
      result = getMax(dat, mask);
      break;
    case RT_ROI_MIN:
      result = getMin(dat, mask);
      break;
    default:
      result = NULL;
  }

  setResult(msg, result);

  if(print) {
    cout << "RtRoiCombine: " << mask->getDataID().getRoiID()
         << " at tr " << dat->getDataID().getTimePoint()
         << " result is: " << result->getPixel(0) << endl;
  }

  if(logOutput) {
    stringstream logs("");
    logs << "RtRoiCombine: " << mask->getDataID().getRoiID()
         << " at tr " << dat->getDataID().getTimePoint()
         << " result is: " << result->getPixel(0) << endl;
    log(logs);
  }

  return 0;
}

// compute the mean over the roi
RtActivation *RtRoiCombine::getMean(RtData *dat, RtMaskImage *mask) {

  RtActivation *result = new RtActivation(1);
  result->getDataID().setFromInputData(*dat,*this);
  result->getDataID().setDataName(NAME_ROIMEAN);
  result->getDataID().setRoiID(mask->getDataID().getRoiID());


  // compute the sum
  double sum = 0;
  RtElementAccess ac(dat, mask);
  vector<unsigned int> indices = ac.getElementIndices();
  for(vector<unsigned int>::iterator i = indices.begin();
      i != indices.end(); i++) {

    double pix = ac.getDoubleElement(*i);
    if(!std::isnan(pix) && !std::isinf(pix)) {
      sum += pix;
    }
  }

  result->setPixel(0,sum/indices.size());

  return result;
}

// compute the variance over the roi
RtActivation *RtRoiCombine::getVariance(RtData *dat, RtMaskImage *mask) {

  RtActivation *result = new RtActivation(1);
  result->getDataID().setFromInputData(*dat,*this);
  result->getDataID().setDataName(NAME_ROIVAR);
  result->getDataID().setRoiID(mask->getDataID().getRoiID());


  // compute the mean
  double mean = 0;
  RtElementAccess ac(dat, mask);
  vector<unsigned int> indices = ac.getElementIndices();
  for(vector<unsigned int>::iterator i = indices.begin();
      i != indices.end(); i++) {

    double pix = ac.getDoubleElement(*i);
    if(!std::isnan(pix) && !std::isinf(pix)) {
      mean += pix;
    }
  }
  mean /= indices.size();

  // go back through to compute variance
  double sqSum = 0;
  for(vector<unsigned int>::iterator i = indices.begin();
      i != indices.end(); i++) {

    double pix = ac.getDoubleElement(*i);
    if(!std::isnan(pix) && !std::isinf(pix)) {
      sqSum = (pix-mean)*(pix-mean);
    }
  }

  result->setPixel(0,sqSum/(indices.size()-1));

  return result;
}


// compute the median over the roi
RtActivation *RtRoiCombine::getMedian(RtData *dat, RtMaskImage *mask) {

  RtActivation *result = new RtActivation(1);
  result->getDataID().setFromInputData(*dat,*this);
  result->getDataID().setDataName(NAME_ROIMEDIAN);
  result->getDataID().setRoiID(mask->getDataID().getRoiID());

  // fill a vector with the values
  vector<double> roiVals;
  RtElementAccess ac(dat, mask);
  vector<unsigned int> indices = ac.getElementIndices();
  for(vector<unsigned int>::iterator i = indices.begin();
      i != indices.end(); i++) {

    double pix = ac.getDoubleElement(*i);
    if(!std::isnan(pix) && !std::isinf(pix)) {
      roiVals.push_back(pix);
    }
  }

  unsigned int numPix = roiVals.size();
  double medVal = 0;
  if(numPix != 0) {
    sort(roiVals.begin(), roiVals.end());
    medVal = (numPix%2 == 0)
        ? (roiVals[numPix/2] + roiVals[numPix/2-1]) / 2 : roiVals[numPix/2];
  }

  result->setPixel(0,medVal);

  return result;
}


// compute the weighted average over the roi
RtActivation *RtRoiCombine::getWeightedAve(RtData *dat, RtData *weights,
                                           RtMaskImage *mask) {

  RtActivation *result = new RtActivation(1);
  result->getDataID().setFromInputData(*dat,*this);
  result->getDataID().setDataName(NAME_ROIWEIGHTEDAVE);
  result->getDataID().setRoiID(mask->getDataID().getRoiID());

  double weightedSum = 0;
  double summedWeights = 0;
  RtElementAccess ac(dat, mask);
  RtElementAccess wac(weights, mask);
  vector<unsigned int> indices = ac.getElementIndices();
  for(vector<unsigned int>::iterator i = indices.begin();
      i != indices.end(); i++) {

    double pix = ac.getDoubleElement(*i);
    double weight = wac.getDoubleElement(*i);

    if(!std::isnan(pix) && !std::isinf(pix)
       && !std::isnan(weight) && !std::isinf(weight)) {

      weightedSum += weight*pix;
      summedWeights += weight;
    }

    if (dumpAlgoVars && dat->getDataID().getTimePoint() > 2) {
      dumpFile
          << result->getDataID().getTimePoint() << " "
          << *i << " "
          << pix << " "
          << weight << endl;
    }
  }

  if(fabs(weightedSum) < std::numeric_limits<double>::epsilon()
     || fabs(summedWeights) < std::numeric_limits<double>::epsilon()) {
    result->setPixel(0,0.0);
  }
  else {
    result->setPixel(0,weightedSum/summedWeights);
  }

  if (dumpAlgoVars && dat->getDataID().getTimePoint() > 2) {
    dumpFile
        << result->getDataID().getTimePoint() << " "
        << -1 << " "
        << weightedSum << " "
        << summedWeights << endl;
  }

  return result;
}

// compute the sum over the roi
RtActivation *RtRoiCombine::getSum(RtData *dat, RtMaskImage *mask) {

  RtActivation *result = new RtActivation(1);
  result->getDataID().setFromInputData(*dat,*this);
  result->getDataID().setDataName(NAME_ROISUM);
  result->getDataID().setRoiID(mask->getDataID().getRoiID());

  double sum = 0;
  RtElementAccess ac(dat, mask);
  vector<unsigned int> indices = ac.getElementIndices();
  for(vector<unsigned int>::iterator i = indices.begin();
      i != indices.end(); i++) {

    double pix = ac.getDoubleElement(*i);
    if(!std::isnan(pix)
       && !std::isinf(pix)
       ) {
      sum += pix;
    }
  }

  result->setPixel(0,sum);

  return result;
}

// compute the max over the roi
RtActivation *RtRoiCombine::getMax(RtData *dat, RtMaskImage *mask) {

  RtActivation *result = new RtActivation(1);
  result->getDataID().setFromInputData(*dat,*this);
  result->getDataID().setDataName(NAME_ROIMAX);
  result->getDataID().setRoiID(mask->getDataID().getRoiID());

  double max = numeric_limits<double>::min();
  RtElementAccess ac(dat, mask);
  vector<unsigned int> indices = ac.getElementIndices();
  for(vector<unsigned int>::iterator i = indices.begin();
      i != indices.end(); i++) {

    double pix = ac.getDoubleElement(*i);
    if(!std::isnan(pix)
       && !std::isinf(pix)
       && pix < max
       ) {
      max = pix;
    }
  }

  result->setPixel(0,max);

  return result;
}

// compute the min over the roi
RtActivation *RtRoiCombine::getMin(RtData *dat, RtMaskImage *mask) {

  RtActivation *result = new RtActivation(1);
  result->getDataID().setFromInputData(*dat,*this);
  result->getDataID().setDataName(NAME_ROIMIN);
  result->getDataID().setRoiID(mask->getDataID().getRoiID());

  double min = numeric_limits<double>::max();
  RtElementAccess ac(dat, mask);
  vector<unsigned int> indices = ac.getElementIndices();
  for(vector<unsigned int>::iterator i = indices.begin();
      i != indices.end(); i++) {

    double pix = ac.getDoubleElement(*i);
    if(!std::isnan(pix)
       && !std::isinf(pix)
       && pix < min
       ) {
      min = pix;
    }
  }

  result->setPixel(0,min);

  return result;
}

// start a logfile
void RtRoiCombine::startDumpAlgoVarsFile() {
  dumpFile << "started at ";
  printNow(dumpFile);
  dumpFile << endl
           << "time_point "
           << "voxel_index "
           << "current_activation "
           << "weight"
    ;

  dumpFile << "end" << endl;
}
