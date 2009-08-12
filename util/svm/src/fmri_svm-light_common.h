// common functions used for svm training and cleassification on fmri
// data using svm-light for training/classification
//
// Oliver Hinds <ohinds@mit.edu> 2009-07-15

#ifndef FMRI_SVMLIGHT_COMMON
#define FMRI_SVMLIGHT_COMMON

#include<string>

#include<svm_common.h>
#include<svm_learn.h>

using namespace std;

///// parms /////
struct Parms {
  // names self explanatory
  string executeName;

  // preprocessing parms
  string maskFile;          // 3D nifti mask volume

  // examples
  string exampleFile;       // 4D nifti volume
  string labelFile;         // 1D text file
  DOC    **docs;            // svm-light doc array
  double *labels;           // class label array
  long   numExamples;
  long   numWords;

  // training parms
  LEARN_PARM learnParm;     // model training parms
  KERNEL_PARM kernelParm;   // data transformation kernel parms

  // training results
  string modelFile;         // filename to store trained model in
  string weightFile;        // filename to store model weights in 
  string resultFile;        // filename to store test results in 
  MODEL  model;             // svm-light model struct
  double *alpha;            // alpha parm array

  bool   error;             // error flag

  // construct with default vals
  Parms():
    docs(NULL),
    labels(NULL),
    numExamples(0),
    numWords(0),
    alpha(NULL),
    error(false)
  {}

  // destruct
  ~Parms() {
    // train docs
    for(long i = 0; i < numExamples; i++) {
      free_example(docs[i],1);
    }
    delete [] docs;
    delete [] labels;
    delete [] alpha;
  }
};


///// function prototypes /////

// read training/testing examples from a nifti file 
bool readDoc(
	string niiFile, string labFile, short *maskdat, long numOnVox,// in
	DOC ** &docs, double * &labels, long &numExamples);           // out

// save a model in a binary model file 
bool writeFmriModel(string filename, const Parms &p);

// read a model from a binary model file 
bool readFmriModel(string filename, Parms &p);


// util functions
bool representsTrue(const string &s);
float *convertDoubleArrayToFloatArray(double *darr, long len);
bool saveNifti(string maskName, string niiName, float *data);

#endif
