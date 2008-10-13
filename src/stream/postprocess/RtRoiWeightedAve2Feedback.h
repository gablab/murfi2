/******************************************************************************
 * RtRoiWeightedAve2Feedback.h is the header for a class that computes the
 * weighted average over voxels in an activation map 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#ifndef RTROIWEIGHTEDAVE2FEEDBACK_H
#define RTROIWEIGHTEDAVE2FEEDBACK_H

#include"RtRoi2Feedback.h"

// scopic alexsid: #define WIN32
#ifdef WIN32
    // MS VS2005 provides _isnan function not isnan
    // as it is in *nix system. So add this fix.
    #ifndef isnan
        #define isnan(x) _isnan(x)
    #endif
#endif

// class declaration
class RtRoiWeightedAve2Feedback : public RtRoi2Feedback {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtRoiWeightedAve2Feedback();

  // destructor
  ~RtRoiWeightedAve2Feedback();

protected:

  // process an option
  //  in
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text,
			     const map<string,string> &attr);
  

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // data ID for the voxel weights
  string weightModuleID;

  // data name for the voxel weights
  string weightDataName;
  
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


