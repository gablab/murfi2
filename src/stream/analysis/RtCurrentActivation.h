/*=========================================================================
 *  RtCurrentActivation.h is the header for a class that 
 * 
 * ****************************************************************************/
#ifndef RTCURRENTACTIVATION_H_
#define RTCURRENTACTIVATION_H_

#include"RtStreamComponent.h"

// class declaration

class RtCurrentActivation : public RtStreamComponent {
public:
    static string moduleString;

    // default constructor
    RtCurrentActivation();

    // destructor
    ~RtCurrentActivation();

protected:

    // initialize the estimation algorithm for a particular image size
    void initEstimation(RtMRIImage &dat, RtMaskImage *mask);

    // process an option in name of the option to process val  text of the option node
    virtual bool processOption(const string &name, const string &text, const map<string, string> &attrMap);

    // make sure we are configured properly
    virtual bool validateComponentConfig();

    // process a single acquisition
    virtual int process(ACE_Message_Block *mb);

    // start a logfile
    virtual void startDumpAlgoVarsFile();

    // data ID for the activation we want
    string modelFitModuleID;

    // roi ID for the activation we want
    string modelFitRoiID;

    // if we should reinit
    bool needsInit;

    // design matrix
    RtDesignMatrix design;

    // number of data actually in the error estimate so far
    unsigned int numDataPointsForErrEst;

    // for post-numDataPointsInErrEst residual storage
    RtActivation *steadyStateResidual;

  // whether to save the image result
  bool saveResult;
};
#endif /*RTCURRENTACTIVATION_H_*/

/*****************************************************************************
 * $Source:  $
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/
