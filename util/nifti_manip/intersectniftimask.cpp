// intersect nifti masks

#include<nifti1_io.h>
#include<iostream>

using namespace std;

int main(int argc, char **argv) {
  if(argc < 3) {
    cout << "usage: " << argv[0] << " outfile infile1 [infile2 ...]" << endl;
    return 0;
  }

  // read the first mask
  nifti_image *mask = nifti_image_read(argv[2], 0);
  if(mask == NULL) {
    cerr << "could not open " << argv[2] << " for reading a nifti mask"
	 << endl;
    return 1;
  }

  // threshold based on type
  if(mask->datatype != DT_SIGNED_SHORT) {
    cout << "cant intersect datatype " << mask->datatype
	 << ", only signed short (4) accepted."
	 << endl;
    return 1;
  }

  // read the image data
  nifti_image_load(mask);

  short *data = (short*) mask->data;

  // load each of the other masks
  for(int i = 3; i < argc; i++) {
    nifti_image *nextmask = nifti_image_read(argv[i],0);
    if(nextmask == NULL) {
      cout << "couldn't find mask " << argv[i] << ", skipping." << endl;
      continue;
    }

    if(nextmask->datatype != mask->datatype) {
      cout << "error: trying to compare masks of two different datatypes"
	   << endl;
      continue;
    }

    if(nextmask->nvox != mask->nvox) {
      cout << "error: masks of two different sizes: "
	   << nextmask->nvox << " != " << mask->nvox << endl
	   << "trying to continue" << endl;
      continue;
    }

    nifti_image_load(nextmask);
    short *nextdata = (short*) nextmask->data;

    for(int j = 0; j < nextmask->nvox; j++) {
      if(!nextdata[j]) {
	data[j] = 0;
      }
    }

    nifti_image_free(nextmask);
  }

  // debugging
  //nifti_image_infodump(mask);

  // write the file
  strcpy(mask->fname,argv[1]);
  nifti_image_write(mask);
  //nifti_image_free(mask);

  return 0;
}
