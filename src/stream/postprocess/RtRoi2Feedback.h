/******************************************************************************
 * RtRoi2Feedback.h is the header for a base class for combining roi
 * activations to result in a feedback signal
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-09-15
 *
 *****************************************************************************/

#ifndef RTROI2FEEDBACK_H
#define RTROI2FEEDBACK_H

#include"RtActivationEstimator.h"
#include"RtActivation.h"

// scopic alexsid: #define WIN32
#ifdef WIN32
    // MS VS2005 provides _isnan function not isnan
    // as it is in *nix system. So add this fix.
    #ifndef isnan
        #define isnan(x) _isnan(x)
    #endif
#endif

// class declaration
class RtRoi2Feedback : public RtActivationEstimator {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtRoi2Feedback();

  // destructor
  ~RtRoi2Feedback();

protected:

  // process an option
  //  in
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text,
			     const map<string,string> &attr);

  // process a single acquisition (pure virtual)
  virtual int process(ACE_Message_Block *mb) = 0;

  // data ID for the activation we will combine
  string activationModuleID;

  // data name for the activation we will combine
  string activationDataName;

  // roi ID for the activation we will combine
  string activationRoiID;

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


