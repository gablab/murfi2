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
  //   val  text of the option node
  virtual bool processOption(const string &name, const string &text);

  // which voxels to normalize
  RtMaskImage mask;
  double maskIntensityThreshold;

  // methods for making the mask

  // use bet
  bool makeBETMask;  // default
  bool computingMask;
  string maskScript;
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


