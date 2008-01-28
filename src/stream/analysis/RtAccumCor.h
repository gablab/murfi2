/******************************************************************************
 * RtAccumCor.h is the header for a class that estimates the
 * instantaneous activation at each voxel in an image using a sliding
 * window correlation analysis similar to Gembris 2000
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-25
 *
 *****************************************************************************/

#ifndef RTACCUMCOR_H
#define RTACCUMCOR_H

#include<vnl/vnl_vector.h>
#include<vnl/vnl_matrix.h>

#include"RtActivationEstimator.h"

// class declaration
class RtAccumCor : public RtActivationEstimator {

public:

  static string moduleString;

  //*** constructors/destructors  ***//

  // default constructor
  RtAccumCor();

  // destructor
  ~RtAccumCor();

protected:

  // process a single acquisition
  virtual int process(ACE_Message_Block *mb);

  // process an option
  //  in 
  //   name of the option to process
  //   val  text of the option node
  virtual bool processOption(const string &name, const string &text);

  // get a datapoint by index
  //virtual unsigned short getDatapoint(RtData &data, unsigned int index) = 0;

  // if we should reinit
  bool needsInit;

  // number of data points to process
  unsigned int numData;

  // subsidiary variables
  vnl_vector<double> *f;
  vnl_vector<double> *g;
  vnl_vector<double> *h;
  vnl_vector<double> *z;

  vnl_matrix<double> *C;
  vnl_matrix<double> *c;

  // mask image
  RtMRIImage *mask;

  // whether to save the result as a mask
  bool saveResultAsMask;

  // filename to save mask to
  string maskFilename;
   
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


