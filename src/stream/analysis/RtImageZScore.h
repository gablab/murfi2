/******************************************************************************
 * RtImageZScore.h is the header for a class that estimates the
 * instantaneous activation at each voxel in an image using simple z-scores
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-25
 *
 *****************************************************************************/

#ifndef RTIMAGEZSCORE_H
#define RTIMAGEZSCORE_H

#include"RtActivationEstimator.h"
#include"RtDataImage.h"

// class declaration
class RtImageZScore : public RtActivationEstimator {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtImageZScore();

  // destructor
  ~RtImageZScore();

protected:

  // process a single acquisition
  virtual int process(ACE_Message_Block *mb);

  // process a configuration option
  //  in
  //   name of the option to process
  //   val  text of the option node
  virtual bool processOption(const string &name, const string &text);

  // data ids to get the mean and variance images from
  string meanDataID;
  string varDataID;
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


