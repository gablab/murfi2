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
#include"RtMRIImage.h"
#include"RtDataImage.h"

#include <float.h>

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

  // make sure we are configured properly
  bool validateComponentConfig();

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // initialize processing
  bool initINorm(RtMRIImage *img);
  
  // process an option
  //  in 
  //   name of the option to process
  //   attr map bettwen attribute names and values
  virtual bool processOption(const string &name, const string &text, 
			     const map<string,string> &attr);

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


