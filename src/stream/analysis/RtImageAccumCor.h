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

#include<vnl_vector.h>
#include"RtStreamComponent.h"
#include"RtDataImage.h"

// class declaration
class RtImageAccumCor : public RtActivationEstimator {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtImageAccumCor();

  // destructor
  ~RtImageAccumCor();

protected:

  // process a single acquisition
  int process(ACE_Message_Block *mb);

  // process an option
  //  in 
  //   name of the option to process
  //   val  text of the option node
  virtual bool processOption(const string &name, const string &text);

  bool needsInit;

  // length of window
  unsigned int windowLen;

  // vector of pointers to last <windowLen> images
  vector<RtMRIImage*> windowData;

  // number of timepoints
  unsigned int numMeas;        // total expected
  unsigned int numTimepoints;  // so far

  // regressor vector
  vnl_vector<double> regressor;

  // trend vectors
  unsigned int numTrends; 
  vnl_matrix<double> trends;

  // mask
  RtMRIImage mask;
  
  // subsidiary variables
  vnl_vector<double> f;
  vnl_vector<double> g;
  vnl_vector<double> h;
  vnl_vector<double> z;

  vnl_matrix<double> C;
  vnl_vector<double> c;
   
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


