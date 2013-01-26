/*=========================================================================
 *  RtRoiCombine.h is the header for a base class for combining roi
 *  activations to result in a feedback signal
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

#ifndef RTROICOMBINE_H
#define RTROICOMBINE_H

#include"RtStreamComponent.h"

// how can we combine the data
enum RtRoiCombineMethod {
  RT_ROI_UNKNOWN,
  RT_ROI_MEAN,
  RT_ROI_VARIANCE,
  RT_ROI_MEDIAN,
  RT_ROI_WEIGHTEDAVE,
  RT_ROI_SUM,
  RT_ROI_MAX,
  RT_ROI_MIN
};


// class declaration
class RtRoiCombine : public RtStreamComponent {

 public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtRoiCombine();

  // destructor
  virtual ~RtRoiCombine();

 protected:

  // process an option
  //  in
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text,
                             const map<string,string> &attr);

  // make sure we are configured properly
  bool validateComponentConfig();

  // process a single acquisition (pure virtual)
  virtual int process(ACE_Message_Block *mb);

  // compute the mean over the roi
  RtActivation *getMean(RtData *dat, RtMaskImage *mask);

  // compute the variance over the roi
  RtActivation *getVariance(RtData *dat, RtMaskImage *mask);

  // compute the median over the roi
  RtActivation *getMedian(RtData *dat, RtMaskImage *mask);

  // compute the weighted average over the roi
  RtActivation *getWeightedAve(RtData *dat, RtData *weights, RtMaskImage *mask);

  // compute the sum over the roi
  RtActivation *getSum(RtData *dat, RtMaskImage *mask);

  // compute the max over the roi
  RtActivation *getMax(RtData *dat, RtMaskImage *mask);

  // compute the min over the roi
  RtActivation *getMin(RtData *dat, RtMaskImage *mask);

  // data
  RtRoiCombineMethod method;

  // if its a weighted average we have to retreive the weights
  string weightsModuleID;
  string weightsDataName;

};

#endif
