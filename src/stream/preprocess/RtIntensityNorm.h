/******************************************************************************
 * RtIntensityNorm.h is the header for a class that normalizes the intensity
 * of subsequent images in a timeseries to match the global signal of the
 * first.
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-03-15
 *
 *****************************************************************************/

#ifndef RTINTENSITYNORM_H
#define RTINTENSITYNORM_H

#include"RtStreamComponent.h"
#include"RtMaskImage.h"
#include"RtDataImage.h"

#include"RtFSLInterface.h"

#include <float.h>
#include "add_functions.h"

// scopic alexsid: #define WIN32
#ifdef WIN32
    // MS VS2005 provides _isnan function not isnan
    // as it is in *nix system. So add this fix.
    #ifndef isnan
        #define isnan(x) _isnan(x)
    #endif
#endif

// class declaration
class RtIntensityNorm : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtIntensityNorm();

  // destructor
  ~RtIntensityNorm();

protected:

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // process an option
  //  in 
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text, 
			     const map<string,string> &attr);

  // which voxels to normalize
  RtMaskImage mask;
  string maskFilename;
  double maskIntensityThreshold;

  // methods for making the mask

  // use fsl BET for brain mask
  bool makeBETMask;  // default
  bool computingMask;
  FslJobID maskJobID;
  string betParms;
  
  // dont use bet
  double meanIntensity; // intensity to normalize to
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


