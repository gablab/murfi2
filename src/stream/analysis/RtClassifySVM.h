/*=========================================================================
 *  RtClassifySVM.h is the header for a class that 
 * 
 * ****************************************************************************/
#ifndef RTCLASSIFYSVM_H_
#define RTCLASSIFYSVM_H_

#include"RtStreamComponent.h"

#include"fmri_svm_common.h"
#include<fstream>

// class declaration

class RtClassifySVM : public RtStreamComponent {
public:
  static string moduleString;

  // default constructor
  RtClassifySVM();

  // destructor
  ~RtClassifySVM();

protected:

  // initialize the estimation algorithm for a particular image size
  //void initEstimation(RtMRIImage &dat, RtMaskImage *mask);

  // process an option in name of the option to process val text of the
  // option node
  virtual bool processOption(const string &name, const string &text, 
			     const map<string, string> &attrMap);

  // make sure we are configured properly
  virtual bool validateComponentConfig();

  // process a single acquisition
  virtual int process(ACE_Message_Block *mb);

  // file to read the svm model from
  string svmModelFile;

  // file to save the predictions to
  string svmResultsFile;
  ofstream svmResultsStream;

  // svm parameters
  Parms svmParms;

  

};

#endif /*RTCLASSIFYSVM_H_*/

/*****************************************************************************
 * $Source:  $
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/
