// unmosaic a nifti file

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
double sliceVoxDim = 3.85;

// convert from a mosaic representation
bool unmosaic(nifti_image *img, int matrixSize, int numSlices, double sliceVoxDim) {
  // validate
  int lastNonZeroDim = 0;
  while(img->dim[lastNonZeroDim++] > 1);
  if(lastNonZeroDim != 4) { // dims
    cerr << "can't unmosaic an image with " << lastNonZeroDim-2 << " dimensions"
	 << endl;
    return false;
  }

  if(matrixSize < 2) {
    cerr << "can't unmosaic an image if I don't know the matrix size"
	 << endl;
    return false;
  }

  // build a new dim array
  int width = img->dim[1];
  int height = img->dim[2];

  img->dim[0] = 3;
  img->ndim = 3;
  img->dim[1] = matrixSize;
  img->nx = matrixSize;
  img->dim[2] = matrixSize;
  img->ny = matrixSize;
  img->dim[3] = numSlices;
  img->nz = numSlices;
  img->nt = img->nu = img->nv = img->nw = 1;

  for(int i = 4; i < 8; i++) {
    img->dim[i] = 1;
  }

  // build a new pixdim
  img->pixdim[2] = sliceVoxDim;
  img->dz = sliceVoxDim;

  // reshape the data
  int numPix = matrixSize*matrixSize*numSlices;
  short *newdata = new short[numPix];
  short *data = (short*) img->data;

  unsigned int slc, row, col, newind;
  unsigned int sqMatrixSize = matrixSize*matrixSize;
  for(unsigned int i = 0; i < width*height; i++) {
    slc = (i%width)/matrixSize +
      (i/(matrixSize*width))*(width/matrixSize);
    row = (i/width)%matrixSize;
    col = i%matrixSize;

    if(slc > numSlices) {
      continue;
    }

    newind = slc*sqMatrixSize+row*matrixSize+col;

    //fprintf(stdout,"%d %d %d %d %d %d\n", data[i], newind, i, slc, row, col);
    newdata[newind] = data[i];
  }

//  ofstream of;
//  of.open("/tmp/imgarr.txt");
//  for(unsigned int i=0; i < numPix; i++) {
//    of << newdata[i] << endl;
//    //cout << newdata[i] << endl;
//  }
//  of.close();

  img->data = (void*) newdata;

  //nifti_image_infodump(img);


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
       << " -s: slice direction voxel size (mm, default 3.85)" << endl
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
    opt = getopt_long (argc, argv, "i:o:m:n:s:v?",
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
    case 's':
      sliceVoxDim = atof(optarg);
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
  parseArgs(argc,argv);

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
    unmosaic(img, matrixSize, numSlices, sliceVoxDim);
    break;
  default:
    cout << "only short int images are currently supported, sorry" << endl;
    return 0;
  }

  // modify the header to change the filename
  strcpy(img->fname, outfile.c_str());

  // debugging
  //nifti_image_infodump(img);

  // write the file
  nifti_image_write(img);

  //nifti_image_free(img);

  return 0;
}
