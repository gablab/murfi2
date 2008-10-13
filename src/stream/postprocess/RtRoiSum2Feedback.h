/******************************************************************************
 * RtRoiSum2Feedback.h is the header for a class that computes the sum over
 * voxels in an activation map 
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-05
 *
 *****************************************************************************/

#ifndef RTROISUM2FEEDBACK_H
#define RTROISUM2FEEDBACK_H

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
class RtRoiSum2Feedback : public RtRoi2Feedback {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtRoiSum2Feedback();

  // destructor
  ~RtRoiSum2Feedback();

protected:

  // process a single acquisition
  int process(ACE_Message_Block *mb);

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


