/******************************************************************************
 * RtSingleImageCor.h is the header for a class that estimates the
 * activation at each voxel in a single image using a GLM-based
 * correlation analysis based on Hinds, et al., 2008
 *
 * Oliver Hinds <ohinds@mit.edu> 2008-01-16
 *
 *****************************************************************************/

#ifndef RTSINGLEIMAGEECOR_H
#define RTSINGLEIMAGEECOR_H

#include<vnl/vnl_vector.h>
#include<vnl/vnl_matrix.h>

#include"RtIncrementalGLM.h"
#include"RtLeastSquaresSolve.h"
#include"RtTypes.h"

#include<vector>
using namespace std;

// class declaration

class RtSingleImageCor : public RtIncrementalGLM {
public:

    static string moduleString;

    //*** constructors/destructors  ***//

    // default constructor
    RtSingleImageCor();

    // destructor
    ~RtSingleImageCor();

protected:

    // deallocate internal data
    void freeEstErrSum();

    // process a single acquisition
    virtual int process(ACE_Message_Block *mb);

    // process an option
    //  in
    //   name of the option to process
    //   attr map bettwen attribute names and values
    virtual bool processOption(const string &name, const string &text,
                               const map<string, string> &attr);

    // make sure we are configured properly
    bool validateComponentConfig();

    // initialize the estimation algorithm for a particular image size
    // in
    //  first acquired image to use as a template for parameter inits
    void initEstimation(RtMRIImage &dat, RtMaskImage *mask);

    // start a logfile
    virtual void startDumpAlgoVarsFile();

    //// parameters of the activation estimation algorithm

    // whether to include the condition mean in the stat computation
    bool includeConditionMean;

    //// error estimation params

    // error norm method
    Norm errorNorm;

    // store the per pixel sum of absolute error for the single image model fit
    RtActivation *estErrSum;

    // number of timepoints
  unsigned int numTimepoints;

    // number of data actually in the error estimate so far
    int numDataPointsCount;

    // the amount of data to use in the estimation of the error
    int numDataPointsForErrEst;

    // whether errors should only be estimated in baseline
    bool onlyEstErrInBaseline;

    bool dumpAlgoVars;
    string dumpAlgoVarsFilename;
    ofstream dumpFile;
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


