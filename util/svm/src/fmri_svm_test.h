// classify fmri data using a model trained using svm-light

#ifndef FMRI_SVMLIGHT_TEST
#define FMRI_SVMLIGHT_TEST

#include"fmri_svm_common.h"

// read files, perform preprocessing
bool createTestingData(Parms &parms);

// command line parm manipulation
void printUsage(string executename);
Parms parseArgs(int argc, char **argv);
bool validateParams(Parms &p);

#endif
