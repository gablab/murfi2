// run svm on fmri data
//
// Oliver Hinds <ohinds@mit.edu> 2009-07-15

#include<fstream>
#include<iostream>
#include<vector>
#include<nifti1_io.h>
#include<svm_common.h>

using namespace std;

///// parms /////
struct Parms {
  // names self explanatory
  string executeName;

  string maskFile;

  string trainFile;
  string trainLabFile;
  DOC    **trainDoc;
  double *trainLabel;
  int    numTrain;

  string testFile;
  string testLabFile;
  DOC    **testDoc;
  double *testLabel;
  int    numTrain;

  string modelFile;
  MODEL  *model;
  int    crossValN;

  bool   error;

  parms():
    trainDoc(NULL),
    trainLabel(NULL),
    testDoc(NULL),
    testLabel(NULL),
    model(NULL),
    crossValN(0),
    error(false);
  {}
};


///// parm reading /////
extern char *optarg;
extern int optind, opterr, optopt;

#include"getopt.h"

int getopt_long(int argc, char * const argv[],
		const char *optstring,
		const struct option *longopts, int *longindex);

void printUsage() {
  cout << executename << " [options]" << endl
       << " -t name: train data nifti filename (required)" << endl
       << " -l name: train data label text file (required)" << endl
       << " -T name: test data nifti filename" << endl
       << " -L name: test data label text file" << endl
       << " -m name: mask file" << endl
       << " -n num : do n-fold cross validation" << endl
       << " -s name: model save filename" << endl
       << " -?: print usage" << endl
       << endl;
}


// parse the command line arguments
Parms parseArgs(int argc, char **argv) {
  int opt, option_index = 0;
  char *format;
  Parms p;

  // loop through input arguments
  while(1) {
    opt = getopt_long (argc, argv, "t:l:T:L:m:n:s:v?",
		       NULL, &option_index);
    if(opt == -1) {
      break;
    }
    if(opt == ':') {
      printUsage();
      p.error = true;
      return p;
    }

    if(opt == '?') {
      printUsage();
      p.error = true;
      return p;
    }

    switch(opt) {
    case 't':
      p.trainFile = optarg;
      break;
    case 'l':
      p.trainLabFile = optarg;
      break;
    case 'T':
      p.testFile = optarg;
      break;
    case 'L':
      p.testLabFile = optarg;
      break;
    case 'm':
      p.maskFile = optarg;
      break;
    case 'n':
      p.crossValN = atoi(optarg);
      break;
    case 's':
      p.modelFile = optarg;
      break;
    case '?':
      printUsage();
      p.error = true;
      return p;
      break;
    }
  }

  return p;
}

bool validateParams(Params &p) {
  if(p.trainFile.empty()) {
    cerr << "training data file ia a required argument" << endl;
    p.error = true;
  }

  if(p.trainLabFile.empty()) {
    cerr << "training label file ia a required argument" << endl;
    p.error = true;
  }

  return !p.error;
}

// read data
void read_nifti_docs(Params &parms) {

  // read the nifti header
  nifti_image *img = nifti_image_read(parms.trainFile.c_str(), 0);
  if(img == NULL) {
    cerr << "could not open " << parms.trainFile 
	 << " for reading a nifti image" << endl;
    return 1;
  }

  // get and validate dimensions
  int nvox = img->nx * img->ny * img->nz;
  int nt = img->nt;

  if(nvox < 1 || nt < 1) {
    cerr << "no voxels in train data volume" << endl;
    return 1;
  }

  // validate type
  bool isshort = false;
  bool isfloat = false;
  switch(img->datatype) {
  case DT_SIGNED_SHORT:
    isshort = true;
    break;
  case DT_FLOAT:
    isfloat = true;
    break;
  default:
    cout << "only int16 and float32 images are currently supported, sorry" << endl;
    return 1;
  }

  // read the image data
  nifti_image_load(img);


  // labelfile
  ifstream labelstr(labelfile.c_str());
  if(!labelstr.is_open()) {
    cerr << "failed to open label file " << labelfile << endl;
    return 1;
  }


  vector<int> labels;
  int label;
  do {
    labelstr >> label;
    labels.push_back(label);
  } while(!labelstr.fail() && !labelstr.eof());
  labelstr.close();


  // mask
  nifti_image *mask = NULL;

  // read the mask header
  if(!maskfile.empty()) {
    mask = nifti_image_read(maskfile.c_str(), 0);
    if(mask == NULL) {
      cerr << "could not open " << maskfile << " for reading a nifti image"
	   << endl;
      return 1;
    }

    // validate type
    if(mask->datatype != DT_SIGNED_SHORT) {
      cout << "only short int masks are currently supported, sorry" << endl;
      return 1;
    }

    // read the mask data
    nifti_image_load(mask);
  }
  short *maskdat = (short*) mask->data;


  // write the example file
  ofstream examplestr;
  if(!splitfiles) {
    examplestr.open(outfile.c_str());
  }

  for(int t = 0; t < nt; t++) {
    // look for labels to skip
    if(labels[t] > 100) {
      continue;
    }

    if(splitfiles) {
      stringstream thisfile;
      thisfile << t << '.' << outfile;

      examplestr.open(thisfile.str().c_str());
    }

    // write the label
    examplestr << labels[t] << " ";

    // loop over voxels, writing each
    for(int v = 0; v < nvox; v++) {
      if(mask && !maskdat[v]) { // skip if masked out
	continue;
      }

      if(isshort) {
	examplestr << v << ":" << ((short*) img->data)[t*nvox+v] << " ";
      }
      else {
	examplestr << v << ":" << ((float*) img->data)[t*nvox+v] << " ";
      }
    }
    examplestr << endl;

    if(splitfiles) {
      examplestr.close();
    }
  }
  if(!splitfiles) {
    examplestr.close();
  }

}

// main
int main(int argc, char **argv) {
  
  Parms parms = parseArgs(argc,argv)) {
  params.executeName = argv[0];
  if(!validateParams(params) || params.error) {
    cerr << "exit" << endl;
    printUsage();
    return 0;
  }
  
  // data loading
  read_nifti_docs(parms);


  return 0;
}

