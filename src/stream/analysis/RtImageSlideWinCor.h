/******************************************************************************
 * RtImageSlideWinCor.h is the header for a class that estimates the
 * instantaneous activation at each voxel in an image using a sliding
 * window correlation analysis similar to Gembris 2000
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-25
 *
 *****************************************************************************/

#ifndef RTIMAGESLIDEWINCOR_H
#define RTIMAGESLIDEWINCOR_H

#include"RtStreamComponent.h"
#include"RtDataImage.h"

// class declaration
class RtImageSlideWinCor : public RtStreamComponent {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtImageSlideWinCor();

  // destructor
  ~RtImageSlideWinCor();

protected:

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // process an option
  //  in 
  //   name of the option to process
  //   val  text of the option node
  //virtual bool processOption(const string &name, const string &text);

  bool needsInit;

  // length of window
  unsigned int windowLen;

  // vector of pointers to last <windowLen> images
  vector<RtMRIImage*> windowData;

  // number of timepoints so far
  unsigned int numTimepoints;

  // regressor vector
  vector<double> regressor;

  // mask
  RtMRIImage mask;
  
  // subsidiary variables
  RtActivation a, c, e;
  double b, d;
   
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


