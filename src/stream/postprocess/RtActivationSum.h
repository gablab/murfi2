/******************************************************************************
 * RtActivationSum.h is the header for a class that computes the sum over
 * voxels in an activation map 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#ifndef RTACTIVATIONSUM_H
#define RTACTIVATIONSUM_H

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
class RtActivationSum : public RtActivationEstimator {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtActivationSum();

  // destructor
  ~RtActivationSum();

protected:

  // process an option
  //  in 
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text, 
			     const map<string,string> &attr);

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // data ID for the activation we will sum
  string activationID;

  // roi ID for the activation we will sum
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


