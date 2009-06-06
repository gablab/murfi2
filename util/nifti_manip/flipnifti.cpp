// Oliver Hinds <ohinds@mit.edu> 2008-03-21
// threshold a nifti file

#include<nifti1_io.h>
#include<iostream>

using namespace std;

void flipDouble(nifti_image *img) {
  double *data = (double*) img->data;
  double *tmp = (double*) malloc(img->nbyper*img->dim[2]);
  for(int i = 0; i < img->dim[1]; i++) {
    double *beg = data+(i*img->dim[1]);
    memcpy(tmp, beg, img->nbyper*img->dim[2]);
    for(int j = 0; j < img->nbyper*img->dim[2]; j++) {
      beg[img->dim[2]-j-1] = tmp[j];
    }
  }

  //free(tmp);
}

void flipFloat(nifti_image *img) {
  float *data = (float*) img->data;
  float *tmp = (float*) malloc(img->nbyper*img->dim[2]);
  for(int i = 0; i < img->dim[1]; i++) {
    float *beg = data+(i*img->dim[1]);
    memcpy(tmp, beg, img->nbyper*img->dim[2]);
    for(int j = 0; j < img->nbyper*img->dim[2]; j++) {
      beg[img->dim[2]-j-1] = tmp[j];
    }
  }

  //free(tmp);
}

void flipShort(nifti_image *img) {
  short *data = (short*) img->data;
  for(int i = 0; i < img->dim[1]; i++) {
    for(int j = 0; j < img->dim[2]/2; j++) {
      int rowind = i*img->dim[2];
      short tmp = data[rowind+j];
      //short tmp = 2;
      data[rowind+j] = data[rowind+img->dim[2]-j-1];
      data[rowind+img->dim[2]-j-1] = tmp;
    }
  }
}

void flipUnsignedShort(nifti_image *img) {
  unsigned short *data = (unsigned short*) img->data;
  unsigned short *tmp = (unsigned short*) malloc(img->nbyper*img->dim[2]);
  for(int i = 0; i < img->dim[1]; i++) {
    unsigned short *beg = data+(i*img->dim[1]);
    memcpy(tmp, beg, img->nbyper*img->dim[2]);
    for(int j = 0; j < img->nbyper*img->dim[2]; j++) {
      beg[img->dim[2]-j-1] = tmp[j];
    }
  }

  //free(tmp);
}

int main(int argc, char **argv) {
  if(argc != 3) {
    cout << "usage: " << argv[0] << " infile outfile" << endl;
    return 0;
  }

  // get the filenames and threshold
  string infile = argv[1];
  string outfile = argv[2];

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
  case DT_DOUBLE:
    flipDouble(img);
    break;
  case DT_FLOAT:
    flipFloat(img);
    break;
  case DT_SIGNED_SHORT:
    flipShort(img);
    break;
  case DT_UINT16:
    flipUnsignedShort(img);
    break;
  default:
    break;
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
