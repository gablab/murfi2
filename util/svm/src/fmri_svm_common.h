// common functions used for svm training and cleassification on fmri
// data using svm-multiclass for training/classification
//
// Oliver Hinds <ohinds@mit.edu> 2009-07-15

#ifndef FMRI_SVM_COMMON
#define FMRI_SVM_COMMON

#include<string>

extern "C" {
#include<svm_struct_api_types.h>
#include<svm_struct/svm_struct_common.h>
#include<svm_struct/svm_struct_learn.h>
}

using namespace std;

///// parms /////
struct Parms {
  // names self explanatory
  string executeName;

  // preprocessing parms
  string maskFile;            // 3D nifti mask volume

  // examples
  string exampleFile;         // 4D nifti volume
  string labelFile;           // 1D text file
  SAMPLE sample;              // input data
  long   numWords;

  // training parms
  STRUCT_LEARN_PARM strctParm; // model training parms
  LEARN_PARM learnParm;        // model training parms
  KERNEL_PARM kernelParm;      // data transformation kernel parms

  // training results
  string modelFile;            // filename to store trained model in
  string weightFile;           // filename to store model weights in
  string resultFile;           // filename to store test results in
  STRUCTMODEL model;           // svm-struct model 
  double *alpha;               // alpha parm array

  bool   error;                // error flag

  // construct with default vals
  Parms():
    numWords(0),
    alpha(NULL),
    error(false) {
      
      sample.n = 0;
      sample.examples = NULL;

      model.svm_model = new MODEL;
  }

  // destruct
  ~Parms() {
    // train docs
    for(long i = 0; i < sample.n; i++) {
      free_example(sample.examples[i].x.doc,1);
    }
    delete [] sample.examples;
    delete [] alpha;
    free_model(model.svm_model,1);
  }
};


///// function prototypes /////

// read training/testing examples from a nifti file
bool readSample(
	string niiFile, string labFile, short *maskdat, long numOnVox,// in
	SAMPLE &sample);                                              // out

// create a single pattern from input data
template<class T>
PATTERN createPattern(unsigned int curDoc, 
		      unsigned int numOnVox, 
		      unsigned int totNumVox,
		      const T*     data,
		      const short* maskData);

// delete a single pattern
void deletePattern(PATTERN &pat);

// a model in a binary model file
bool writeFmriModel(string filename, const Parms &p);

// read a model from a binary model file
bool readFmriModel(string filename, Parms &p);


// util functions
bool representsTrue(const string &s);
float *convertDoubleArrayToFloatArray(double *darr, long len);
bool saveNifti(string maskName, string niiName, float *data);

#endif
