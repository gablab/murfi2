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

  // initialize the baseline mean estimation parameters to match an image
  void initBaselineMeans(RtMRIImage *img);

  // deallocate all baseline mean related images
  void deleteBaselineMeans();

  //// parameters of the activation estimation algorithm

  // threshold on each condition vector below which will be considered the
  // baseline for that condition. necessary because of convolution with hrf.
  // TODO: consider having a global baseline in which the baseline is
  // considered as only when all condition vectors are below some threshold
  vnl_vector<double> baselineThreshold; // numConditions in length
  
  //// auxiliary variables

  // mean activation for each baseline condition
  vector<RtActivation*> baselineMeans; // numConditions in length

  // number of samples we have so far in the current baseline for each condition
  vnl_vector<unsigned int> numBaselineTimepoints; // numConditions in length
 
  // number of data points to process
  unsigned int numData;

  // one solver for each voxel 
  RtLeastSquaresSolve **solvers;

  // store the per pixel sum of squared error for the single image model fit
  RtActivation *estErrSumSq;
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


