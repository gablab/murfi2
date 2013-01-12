// run svm on fmri data

#include"fmri_svm_train.h"
#include"fmri_svm_common.h"

#include<nifti1_io.h>

extern "C" {
#include"svm_struct_api.h"
}

#include<iostream>
using namespace std;

///// parm reading /////
extern char *optarg;
extern int optind, opterr, optopt;

#include"getopt.h"

int getopt_long(int argc, char * const argv[],
		const char *optstring,
		const struct option *longopts, int *longindex);

// main
int main(int argc, char **argv) {

  Parms parms = parseArgs(argc,argv);
  parms.executeName = argv[0];
  if(!validateParams(parms) || parms.error) {
    cerr << "exit" << endl;
    printUsage(argv[0]);
    return 0;
  }

  cout << "reading in training data " << endl;

  // get the training data
  if(!createTrainingData(parms)) {
    return 1;
  }

  cout << "training model " << endl;

  // train
  svm_learn_struct_joint(parms.sample,
			 &parms.strctParm,
			 &parms.learnParm,
			 &parms.kernelParm,
			 &parms.model,
			 ONESLACK_DUAL_CACHE_ALG);

//  if(!parms.weightFile.empty()) {
//    add_weight_vector_to_linear_model(parms.model.svm_model);
//    float *weights
//      = convertDoubleArrayToFloatArray(parms.model.svm_model->lin_weights,
//				       maxWnum);
//    saveNifti(parms.maskFile, parms.weightFile, weights);
//    delete [] weights;
//  }

  if(!parms.modelFile.empty()) {
    cout << "saving model " << endl;

    writeFmriModel(parms.modelFile, parms);
  }

  return 0;
}


void printUsage(string executename) {
  cout << executename << " [options] -- [svm-light options]" << endl
       << " -t name: train data nifti filename (required)" << endl
       << " -l name: train data label text file (required)" << endl
       << " -m name: mask file (required)" << endl
       << " -c     : do leave one out cross validation" << endl
       << " -s name: model save filename" << endl
       << " -w name: save weights filename" << endl
       << " -v num:  verbosity level" << endl
       << " -?: print usage" << endl << endl
       << "EXAMPLE:" << endl
       << "fmri_svm_train -t fmri_run.nii -l fmri_design.tl \\" << endl
       << "               -m mask.nii -s model.svm -w weights.nii" << endl
       << endl << flush;

  //print_help();
  fflush(stdout);
}

// parse the command line arguments
Parms parseArgs(int argc, char **argv) {
  int opt, option_index = 0;
  char *format;
  Parms p;

  init_train(&p.strctParm, &p.learnParm, &p.kernelParm);

  // loop through input arguments
  bool done = false, crossVal = false;
  int ind = 1;
  for(; !done; ind+=2) {
    opt = getopt_long (argc, argv, "-t:l:m:cs:w:v:?",
		       NULL, &option_index);
    if(opt == -1) {
      break;
    }
    if(opt == ':') {
      printUsage(argv[0]);
      p.error = true;
      return p;
    }

    if(opt == '?') {
      printUsage(argv[0]);
      p.error = true;
      return p;
    }

    switch(opt) {
    case '-':
      done = true;
      break;
    case 't':
      p.exampleFile = optarg;
      break;
    case 'l':
      p.labelFile = optarg;
      break;
    case 'm':
      p.maskFile = optarg;
      break;
    case 'c':
      crossVal = true;
      break;
    case 's':
      p.modelFile = optarg;
      break;
    case 'w':
      p.weightFile = optarg;
      break;
    case 'v':
      struct_verbosity = verbosity = atoi(optarg);
      break;
    case '?':
      printUsage(argv[0]);
      p.error = true;
      return p;
      break;
    }
  }

//  // parse svm-light options
//  read_input_parameters(argc - ind, argv + ind,
//			&p.learnParm, &p.kernelParm);

  if(crossVal) {
    p.learnParm.compute_loo = 1;
  }

  return p;
}

bool validateParams(Parms &p) {
  if(p.exampleFile.empty()) {
    cerr << "example data file ia a required argument" << endl;
    p.error = true;
  }

  if(p.labelFile.empty()) {
    cerr << "label file ia a required argument" << endl;
    p.error = true;
  }

  if(p.maskFile.empty()) {
    cerr << "mask file ia a required argument" << endl;
    p.error = true;
  }

  return !p.error;
}

// read data
bool createTrainingData(Parms &parms) {

  // read the mask
  nifti_image *mask = NULL;

  // read the mask header
  short *maskdat = NULL;
  long numOnVox = 0;

  mask = nifti_image_read(parms.maskFile.c_str(), 0);
  if(mask == NULL) {
    cerr << "could not open " << parms.maskFile
	 << " for reading a mask nifti image" << endl;
    return false;
  }

  // validate type
  if(mask->datatype != DT_SIGNED_SHORT) {
    cout << "only short int masks are currently supported, sorry" << endl;
    return 1;
  }

  // read the mask data
  nifti_image_load(mask);
  maskdat = (short*) mask->data;

  // count the mask voxels
  for(long i = 0; i < mask->nvox; i++) {
    if(maskdat[i]) numOnVox++;
  }
  parms.numWords = numOnVox;

  if(numOnVox < 1) {
    cerr << "no voxels in mask" << endl;
    return false;
  }

  // read data

  // training input
  if(!readSample(parms.exampleFile,
		 parms.labelFile,
		 maskdat,
		 numOnVox,
		 parms.sample)) {
    cerr << "error reading training examples" << endl;
    return false;
  }

  // initialize the model
//  init_struct_model(parms.sample,
//		    &parms.model,
//		    &parms.strctParm,
//		    &parms.learnParm,
//		    &parms.kernelParm);

  return true;
}
