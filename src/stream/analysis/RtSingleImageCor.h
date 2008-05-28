/******************************************************************************
 * RtSingleImageCor.h is the header for a class that estimates the
 * activation at each voxel in a single image using a GLM-based
 * correlation analysis based on Hids, et al., 2008
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-01-16
 *
 *****************************************************************************/

#ifndef RTSINGLEIMAGEECOR_H
#define RTSINGLEIMAGEECOR_H

#include<vnl/vnl_vector.h>
#include<vnl/vnl_matrix.h>

#include"RtActivationEstimator.h"
#include"RtLeastSquaresSolve.h"

#include<vector>
using namespace std;

// class declaration
class RtSingleImageCor : public RtActivationEstimator {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtSingleImageCor();

  // destructor
  ~RtSingleImageCor();

protected:

  // process a single acquisition
  virtual int process(ACE_Message_Block *mb);

  // process an option
  //  in 
  //   name of the option to process
  //   val  text of the option node
  virtual bool processOption(const string &name, const string &text);

  // initialize the estimation algorithm for a particular image size
  // in
  //  first acquired image to use as a template for parameter inits
  void initEstimation(RtMRIImage &image);

  // start a logfile 
  virtual void startDumpAlgoVarsFile();

  //// parameters of the activation estimation algorithm
 
  // number of data points to process
  unsigned int numData;

  // one solver for each voxel 
  RtLeastSquaresSolve **solvers;

  //// error estimation params

  // store the per pixel sum of absolute error for the single image model fit
  RtActivation *absEstErrSum;

  // number of data actually in the error estimate so far
  int numDataPointsInErrEst;

  // the amount of data to use in the estimation of the error
  int numDataPointsForErrEst;

  // whether errors should only be estimated in baseline
  bool onlyEstErrInBaseline;
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


