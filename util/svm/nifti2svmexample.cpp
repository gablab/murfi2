// convert a nifti file into an svm example file
//
// Oliver Hinds <ohinds@mit.edu> 2009-07-15

#include<fstream>
#include<iostream>
#include<sstream>
#include<vector>
#include<nifti1_io.h>

using namespace std;


///// globals /////
static string executename;
static string infile;
static string outfile;
static string maskfile;
static string labelfile;
static bool splitfiles = false;


///// parm reading /////
extern char *optarg;
extern int optind, opterr, optopt;

#include"getopt.h"

int getopt_long(int argc, char * const argv[],
		const char *optstring,
		const struct option *longopts, int *longindex);

void printUsage() {
  cout << executename << " [options]" << endl
       << " -i: input filename (required)" << endl
       << " -o: output filename (required)" << endl
       << " -m: mask file" << endl
       << " -l: label file" << endl
       << " -s: spilt into single example files" << endl
       << " -?: print usage" << endl
       << endl;
}


// parse the command line arguments
bool parseArgs(int argc, char **argv) {
  int opt, option_index = 0;
  char *format;

  // loop through input arguments
  while(1) {
    opt = getopt_long (argc, argv, "i:o:m:l:sv?",
		       NULL, &option_index);
    if(opt == -1) {
      break;
    }
    if(opt == ':') {
      printUsage();
      return false;
    }

    if(opt == '?') {
      printUsage();
      return false;
    }

    switch(opt) {
    case 'i':
      infile = optarg;
      break;
    case 'o':
      outfile = optarg;
      break;
    case 'm':
      maskfile = optarg;
      break;
    case 'l':
      labelfile = optarg;
      break;
    case 's':
      splitfiles = true;
      break;
    case '?':
      printUsage();
      return false;
      break;
    }
  }

  return true;
}




// main 
int main(int argc, char **argv) {
  executename = argv[0];
  if(!parseArgs(argc,argv)) {
    return 0;
  }

  if(infile == "" || labelfile == "" || outfile == "") {
    cout << "infile, labelfile,  and outfile and required arguments" << endl;
    printUsage();
    return 0;
  }

  // infile

  // read the infile header
  nifti_image *img = nifti_image_read(infile.c_str(), 0);
  if(img == NULL) {
    cerr << "could not open " << infile << " for reading a nifti image"
	 << endl;
    return 1;
  }

  // get and validate dimensions
  int nvox = img->nx * img->ny * img->nz;
  int nt = img->nt;

  if(nvox < 1 || nt < 1) {
    cerr << "no voxels in this volume" << endl;
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

  return 0;
}

