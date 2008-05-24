// Oliver Hinds <ohinds@mit.edu> 2008-03-21
// threshold a nifti file

#include<nifti1_io.h>
#include<iostream>

using namespace std;

int thresholdDouble(nifti_image *img, double thresh) {
  double *data = (double*) img->data;

  // realloc
  img->nbyper = sizeof(short);
  short *mask = (short*) malloc(img->nvox*img->nbyper);

  int sum = 0;
  for(int i = 0; i < img->nvox; i++) {
    if((thresh > 0 && data[i] < thresh)
      ||
      (thresh < 0 && data[i] > thresh)) {
      mask[i] = 0;
    }
    else {
      mask[i] = 1;
      sum++;
    }
  }

  img->data = (void*) mask;
  free(data);
  
  return sum;
}

int thresholdFloat(nifti_image *img, double thresh) {
  float *data = (float*) img->data;

  // realloc
  img->nbyper = sizeof(short);
  short *mask = (short*) malloc(img->nvox*img->nbyper);

  int sum = 0;
  for(int i = 0; i < img->nvox; i++) {
    if(data[i] < thresh) {
      mask[i] = 0;
    }
    else {
      mask[i] = 1;
      sum++;
    }
  }

  img->data = (void*) mask;
  free(data);
  
  return sum;
}

int thresholdShort(nifti_image *img, double thresh) {
  short *data = (short*) img->data;

  // realloc
  img->nbyper = sizeof(short);
  short *mask = (short*) malloc(img->nvox*img->nbyper);

  int sum = 0;
  for(int i = 0; i < img->nvox; i++) {
    if(data[i] < thresh) {
      mask[i] = 0;
    }
    else {
      mask[i] = 1;
      sum++;
    }
  }

  img->data = (void*) mask;
  free(data);
  
  return sum;
}

int thresholdUnsignedShort(nifti_image *img, double thresh) {
  unsigned short *data = (unsigned short*) img->data;

  // realloc
  img->nbyper = sizeof(short);
  short *mask = (short*) malloc(img->nvox*img->nbyper);

  int sum = 0;
  for(int i = 0; i < img->nvox; i++) {
    if(data[i] < thresh) {
      mask[i] = 0;
    }
    else {
      mask[i] = 1;
      sum++;
    }
  }

  img->data = (void*) mask;
  free(data);
  
  return sum;
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

  int sum;
  // threshold based on type
  switch(img->datatype) {
  case DT_DOUBLE:
    sum = thresholdDouble(img,thresh);
    break;
  case DT_FLOAT:
    sum = thresholdFloat(img,thresh);
    break;
  case DT_SIGNED_SHORT:
    sum = thresholdShort(img,thresh);
    break;
  case DT_UINT16:
    sum = thresholdUnsignedShort(img,thresh);
    break;
  default:
    break;
  }

  cout << "threshold got " << sum << " voxels" << endl;

  // modify the header to change the filename
  strcpy(img->fname, outfile.c_str());  

  // change the datatype
  img->datatype = DT_SIGNED_SHORT;

  // debugging
  //nifti_image_infodump(img);

  // write the file
  nifti_image_write(img);

  nifti_image_free(img);

  return 0;
}
