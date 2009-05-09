// Oliver Hinds <ohinds@mit.edu> 2008-03-21
// mosaic a nifti file

#include<nifti1_io.h>
#include<iostream>

using namespace std;

// global vars
string executeName;
bool verbose = false;

string infile = "";
string outfile = "";
int matrixSize = 64;
int numSlices = 32;

// convert from a mosaic representation
bool mosaic(nifti_image *img, int matrixSize, int numSlices) {

  //sqrt(matrixSize^2*ceil(sqrt(numSlices))^2);
  img->ndim = 2;
  img->dim[0] = 2;
  img->dim[1] = (int) sqrt(matrixSize*matrixSize*pow(ceil(sqrt((double)numSlices)),2));
  img->nx = img->dim[1];
  img->dim[2] = (int)sqrt(matrixSize*matrixSize*pow(ceil(sqrt((double)numSlices)),2));
  img->ny = img->dim[2];
  int numPix = img->dim[1]*img->dim[2];
  img->nvox = numPix;

  // reshape the data
  short *newdata = new short[numPix];
  short *data = (short*) img->data;

  unsigned int newrow, newcol, oldslc, newind;
  unsigned int sqMatrixSize = matrixSize*matrixSize;
  for(unsigned int i = 0; i < numPix; i++) {
    oldslc = i/sqMatrixSize;

    // these expressions are bracketed carefully to truncate
    // DO NOT SIMPLIFY THESE EXPRESSIONS
    newrow = (i/matrixSize)%matrixSize + matrixSize*(i/(img->dim[1]*matrixSize));
    newcol = (matrixSize*(i/sqMatrixSize) + (i%matrixSize))%img->dim[2];
    newind = newrow*img->dim[2]+newcol;

    // copy if within slices
    if(oldslc < numSlices) {
      newdata[newind] = data[i];
    }
    else { // fill the blank panels with zeros
      newdata[newind] = 0;
    }
  }

  img->data = (void*) newdata;


  return true;
}

//// argument parsing stuffs
#include <unistd.h>

int getopt(int argc, char * const argv[],
		   const char *optstring);

extern char *optarg;
extern int optind, opterr, optopt;

#include"getopt.h"

int getopt_long(int argc, char * const argv[],
				const char *optstring,
				const struct option *longopts, int *longindex);

// print usage
void printUsage() {
  cout << executeName << " [options]" << endl
       << " -i: input filename (required)" << endl
       << " -o: output filename (required)" << endl
       << " -m: matrix size (default 64)" << endl
       << " -n: number of slices (default 32)" << endl
       << " -v: verbose" << endl
       << " -?: print usage" << endl
       << endl;
}


// parse the command line arguments
bool parseArgs(int argc, char **argv) {
  int opt, option_index = 0;
  char *format;

  // loop through input arguments
  while(1) {
    opt = getopt_long (argc, argv, "i:o:m:n:v?",
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
      matrixSize = atoi(optarg);
      break;
    case 'n':
      numSlices = atoi(optarg);
      break;
    case 'v':
      verbose = 1;
      break;
    case 'q':
      verbose = 0;
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
  executeName = argv[0];
  if(!parseArgs(argc,argv)) {
    return 0;
  }

  if(infile == "" || outfile == "") {
    cout << "infile and outfile and required arguments" << endl;
    printUsage();
    return 0;
  }

  // read the infile header
  nifti_image *img = nifti_image_read(infile.c_str(), 0);
  if(img == NULL) {
    cerr << "could not open " << infile << " for reading a nifti image"
	 << endl;
    return 1;
  }

  // read the image data
  nifti_image_load(img);

  // threshold based on type
  switch(img->datatype) {
  case DT_SIGNED_SHORT:
    mosaic(img, matrixSize, numSlices);
    break;
  default:
    cout << "only short int images are currently supported, sorry" << endl;
    return 0;
  }

  // modify the header to change the filename
  strcpy(img->fname, outfile.c_str());  

  // debugging
  nifti_image_infodump(img);

  // write the file
  nifti_image_write(img);

  //nifti_image_free(img);

  return 0;
}
