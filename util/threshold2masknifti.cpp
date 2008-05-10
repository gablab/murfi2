// Oliver Hinds <ohinds@mit.edu> 2008-03-21
// threshold a nifti file

#include<nifti1_io.h>
#include<iostream>

using namespace std;

void thresholdDouble(nifti_image *img, double thresh) {
  double *data = (double*) img->data;

  // realloc
  img->nbyper = sizeof(short);
  short *mask = (short*) malloc(img->nvox*img->nbyper);

  for(int i = 0; i < img->nvox; i++) {
    if(data[i] < thresh) {
      mask[i] = 0;
    }
    else {
      mask[i] = 1;
    }
  }

  img->data = (void*) mask;
  free(data);
}

void thresholdFloat(nifti_image *img, double thresh) {
  float *data = (float*) img->data;

  // realloc
  img->nbyper = sizeof(short);
  short *mask = (short*) malloc(img->nvox*img->nbyper);

  for(int i = 0; i < img->nvox; i++) {
    if(data[i] < thresh) {
      mask[i] = 0;
    }
    else {
      mask[i] = 1;
    }
  }

  img->data = (void*) mask;
  free(data);
}

void thresholdShort(nifti_image *img, double thresh) {
  short *data = (short*) img->data;

  // realloc
  img->nbyper = sizeof(short);
  short *mask = (short*) malloc(img->nvox*img->nbyper);

  for(int i = 0; i < img->nvox; i++) {
    if(data[i] < thresh) {
      mask[i] = 0;
    }
    else {
      mask[i] = 1;
    }
  }

  img->data = (void*) mask;
  free(data);
}

void thresholdUnsignedShort(nifti_image *img, double thresh) {
  unsigned short *data = (unsigned short*) img->data;

  // realloc
  img->nbyper = sizeof(short);
  short *mask = (short*) malloc(img->nvox*img->nbyper);

  for(int i = 0; i < img->nvox; i++) {
    if(data[i] < thresh) {
      mask[i] = 0;
    }
    else {
      mask[i] = 1;
    }
  }

  img->data = (void*) mask;
  free(data);
}

int main(int argc, char **argv) {
  if(argc != 4) {
    cout << "usage: " << argv[0] << " infile thresh outfile" << endl;
  }

  // get the filenames and threshold
  string infile = argv[1];
  double thresh = atof(argv[2]);
  string outfile = argv[3];

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
    thresholdDouble(img,thresh);
    break;
  case DT_FLOAT:
    thresholdFloat(img,thresh);
    break;
  case DT_SIGNED_SHORT:
    thresholdShort(img,thresh);
    break;
  case DT_UINT16:
    thresholdUnsignedShort(img,thresh);
    break;
  default:
    break;
  }

  // modify the header to change the filename
  strcpy(img->fname, outfile.c_str());  

  // change the datatype
  img->datatype = DT_SIGNED_SHORT;

  // debugging
  nifti_image_infodump(img);

  // write the file
  nifti_image_write(img);

  nifti_image_free(img);

  return 0;
}
