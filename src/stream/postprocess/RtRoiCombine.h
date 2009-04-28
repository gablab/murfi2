/******************************************************************************
 * RtRoiCombine.h is the header for a base class for combining roi
 * activations to result in a feedback signal
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-09-15
 *
 *****************************************************************************/

#ifndef RTROICOMBINE_H
#define RTROICOMBINE_H

#include"RtStreamComponent.h"

// scopic alexsid: #define WIN32
#ifdef WIN32
    // MS VS2005 provides _isnan function not isnan
    // as it is in *nix system. So add this fix.
    #ifndef isnan
        #define isnan(x) _isnan(x)
    #endif
#endif

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
  ~RtRoiCombine();

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

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


