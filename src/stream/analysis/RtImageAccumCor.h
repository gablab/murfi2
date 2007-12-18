/******************************************************************************
 * RtImageAccumCor.h is the header for a class that estimates the
 * instantaneous activation at each voxel in an image using a sliding
 * window correlation analysis similar to Gembris 2000
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-25
 *
 *****************************************************************************/

#ifndef RTIMAGEACCUMCOR_H
#define RTIMAGEACCUMCOR_H

#include<vnl/vnl_vector.h>
#include"RtActivationEstimator.h"
#include"RtDataImage.h"

// class declaration
class RtImageAccumCor : public RtActivationEstimator {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtImageAccumCor();

  // destructor
  virtual ~RtImageAccumCor();

protected:

  // process a single acquisition
  virtual int process(ACE_Message_Block *mb);

  // process an option
  //  in 
  //   name of the option to process
  //   val  text of the option node
  virtual bool processOption(const string &name, const string &text);

  bool needsInit;

  // mask
  RtMRIImage mask;
  
   
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


