/******************************************************************************
 * c++ wrapper for gentleman's least squares computation
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-03-31 
 * 
 *****************************************************************************/

#include<cstdlib>

class RtLeastSquaresSolve {

public:

  // constructors
  RtLeastSquaresSolve();

  // with the number of columns in the design
  RtLeastSquaresSolve(unsigned int _numCols);

  // with the number of columns in the design
  RtLeastSquaresSolve(unsigned int _numCols, unsigned int _numMeas);

  // destructor
  ~RtLeastSquaresSolve();

  // include a new observation in the estimate
  // in:
  //  yelem  observation vector at this timepoint
  //  xrow   design matrix row (must be p elements long)
  //  weight of this observation
  bool include(double *yelem, double *yelem, double weight);

  // regress using the model with data up to this point
  // in:
  //  meas is the index of the observation to regress on
  // out:
  //  vector of betas
  double *regress(unsigned int meas);

  // compute and return the squared error residual for each column
  // in:
  //  meas is the index of the observation to regress on
  // out:
  //  residual 
  double *getSquaredError(unsigned int meas);

  // compute and return the total squared error
  // in:
  //  meas is the index of the observation to regress on
  // out:
  //  residual 
  double getTotalSquaredError(unsigned int meas);

protected:

  // init all the variables
  void init();
  
  unsigned int numMeas; // number of observations at each time point
  unsigned int numCols; // number of columns in the design matrix
  double **d;           // entries of the diagonal scaling matrix D
  double **rbar;        // unrolled superdiagonal entries of R.
                        // the Cholesky decomp is D^(1/2)*R
  double **thetabar;    // D^(1/2)*thetabar is the orthogonal coefficients
  double *sserr;        // sum of the squared error
  
};

