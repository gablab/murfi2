// run svm on fmri data
//
// Oliver Hinds <ohinds@mit.edu> 2009-07-15

#include"fmri_svm-light_common.h"

#include<fstream>
#include<iostream>
#include<vector>
#include<limits>
#include<cstring>

#include<nifti1_io.h>

#include<svm_common.h>

using namespace std;


// read a single document
bool readDoc(
       string niiFile, string labFile, short *maskdat, long numOnVox,// in
       DOC ** &docs, double * &labels, long &numExamples)  {        // out

  // read the label file
  bool readLabels = !labFile.empty();
  vector<double> labRead;
  long numLabeledTR = 0;
  if(readLabels) {
    ifstream labelstr(labFile.c_str());
    if(!labelstr.is_open()) {
      cerr << "failed to open label file " << labFile << endl;
      return false;
    }

    double label;
    do {
      labelstr >> label;
      labRead.push_back(label);
      if(label != 0) numLabeledTR++;
    } while(!labelstr.fail() && !labelstr.eof());
    labelstr.close();

    if(numLabeledTR < 1) {
      cerr << "no input trs (all labels are 0)" << endl;
      return false;
    }
  }

  // read the nifti header
  nifti_image *img = nifti_image_read(niiFile.c_str(), 0);
  if(img == NULL) {
    cerr << "could not open " << niiFile
	 << " for reading a nifti image" << endl;
    return false;
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
    return false;
  }

  // read the image data
  nifti_image_load(img);

  if(!maskdat) {
    numOnVox = img->nvox;
  }

  // mask the labels array
  numExamples = readLabels ? numLabeledTR-1 : img->nt;;
  labels = new double[numExamples];

  // make the documents
  docs = new DOC*[numExamples];
  long curDoc = 0, curLab = 0;
  for(long t = 0; t < img->nt; t++) {
    // look for labels to skip
    if(readLabels && abs(labRead[t]) < numeric_limits<double>::epsilon()) {
      continue;
    }

    labels[curLab++] = readLabels ? labRead[t] : 0;

    // create the feature vector
    WORD *words = new WORD[numOnVox+1];
    long curWord = 0;

    // loop over voxels, writing each
    long nvox = img->nx * img->ny *img->nz;
    for(long v = 0; v < nvox; v++) {
      if(maskdat && !maskdat[v]) { // skip if masked out
	continue;
      }

      words[curWord].wnum = v+1; // words are one-based
      if(isshort) {
	words[curWord].weight = (FVAL) ((short*) img->data)[t*nvox+v];
      }
      else {
	words[curWord].weight = (FVAL) ((float*) img->data)[t*nvox+v];
      }

      curWord++;
    }
    words[curWord].wnum = 0;
    words[curWord].weight = 0.;

    // allocate a DOC
    docs[curDoc++] = create_example(curDoc, 0., 0., 1.,
				    create_svector(words,"",1.0));
    delete [] words;
  }
  nifti_image_free(img);


  // check that everything was read
  if(curDoc != numExamples) {
    cerr << "expected " << numExamples << " examples, but only read " << curDoc
	 << " from " << niiFile << endl;
    return false;
  }

  return true;
}

// save an svm model for fmri
// modified from write_model in svm_common.c
//
// the file has an ascii header containing preprocessing details for
// the fmri data and svm model parms followed by a binary section
// containing the support vectors
bool writeFmriModel(string filename, const Parms &p) {
  ofstream ms;
  ms.open(filename.c_str());

  ms << "fmri_preprocessing_parms" << endl
     << "mask_file:   " << p.maskFile << endl
     << "svm_parms "    << endl
     << "kernel_type: " << p.model.kernel_parm.kernel_type << endl
     << "poly_degree: " << p.model.kernel_parm.poly_degree << endl
     << "rbf_gamma:   " << p.model.kernel_parm.rbf_gamma << endl
     << "coef_lin:    " << p.model.kernel_parm.coef_lin << endl
     << "coef_const:  " << p.model.kernel_parm.coef_const << endl
     << "custom:      " << p.model.kernel_parm.custom << endl
     << "totwords:    " << p.model.totwords << endl
     << "totdoc:      " << p.model.totdoc << endl;

  long sv_num = 1;
  SVECTOR *v;
  for(long i = 1; i < p.model.sv_num; i++) {
    for(v = p.model.supvec[i]->fvec; v; v = v->next) {
      sv_num++;
    }
  }
  ms << "sv_num:      " << sv_num << endl;
  ms << "b:           " << p.model.b << endl;
  ms << "support_vectors_(binary)" << endl;

  ms.close();

  // reopen in binary mode
  ms.open(filename.c_str(), ios::out | ios::binary | ios::app);

  // support vecs
  double write_double;
  long   write_long;
  for(long i = 1; i < p.model.sv_num; i++) {
    for(v = p.model.supvec[i]->fvec; v; v = v->next) {
      write_double = p.model.alpha[i]*v->factor;
      ms.write((char*) &(write_double), sizeof(double));
      for(long j = 0; (v->words[j]).wnum; j++) {
	ms.write((char*) &((v->words[j]).wnum), sizeof(long));
	ms.write((char*) &((v->words[j]).weight), sizeof(double));
      }
      write_long = 0;
      ms.write((char*) &(write_long), sizeof(long));

    }
  }

  ms.close();
}


// read a model from a binary model file
bool readFmriModel(string filename, Parms &p) {
  ifstream ms;
  string trash;
  ms.open(filename.c_str());

  if(!ms.good()) return false;

  // read fmri parm header
  ms >> trash;

  ms >> trash;
  ms >> p.maskFile;

  // read svm parm header
  ms >> trash;

  ms >> trash;
  ms >> p.model.kernel_parm.kernel_type;

  ms >> trash;
  ms >> p.model.kernel_parm.poly_degree;

  ms >> trash;
  ms >> p.model.kernel_parm.rbf_gamma;

  ms >> trash;
  ms >> p.model.kernel_parm.coef_lin;

  ms >> trash;
  ms >> p.model.kernel_parm.coef_const;

  ms >> trash;
  ms >> p.model.kernel_parm.custom;

  ms >> trash;
  ms >> p.model.totwords;

  ms >> trash;
  ms >> p.model.totdoc;

  ms >> trash;
  ms >> p.model.sv_num;

  ms >> trash;
  ms >> p.model.b;
  ms >> trash;

  p.model.supvec = new DOC*[p.model.sv_num]();
  p.model.alpha = new double[p.model.sv_num]();
  p.model.index = NULL;
  p.model.lin_weights = NULL;

  WORD *words = new WORD[p.model.totwords+1]();
  char buf[max(sizeof(double), sizeof(long))];
  ms.read(buf,1); // newline

  // reopen in binary mode
  streampos binBegin = ms.tellg();
  ms.close();
  ms.open(filename.c_str(), ios::in | ios::binary);
  ms.seekg(binBegin);

  for(long i = 1; i < p.model.sv_num; i++) {

    ms.read(buf, sizeof(double));
    memcpy(&(p.model.alpha[i]), buf, sizeof(double));

    long curWord = 0;
    ms.read(buf, sizeof(long));
    memcpy(&(words[curWord].wnum), buf, sizeof(long));

    while(words[curWord].wnum) {
      ms.read(buf, sizeof(double));
      memcpy(&(words[curWord].weight), buf, sizeof(double));
      curWord++;
      ms.read(buf, sizeof(long));
      memcpy(&(words[curWord].wnum), buf, sizeof(long));
    }

    p.model.supvec[i] = create_example(-1,
				       0,0,
				       0.0,
				       create_svector(words,"",1.0));
  }
  delete [] words;


  ms.close();
  return true;
}

// utility functions

bool representsTrue(const string &s) {
  return
    s == "1" ||
    s == "true" ||
    s == "TRUE" ||
    s == "True";
}

float *convertDoubleArrayToFloatArray(double *darr, long len) {
  float *farr = new float[len];
  for(int i = 0; i < len; i++) farr[i] = static_cast<float>(darr[i]);
  return farr;
}

// save a nifti file from float data using an existing nifti file as a
// reference for voxel geometry, world coordinates
bool saveNifti(string refNii, string outNii, float *data) {

  // load the ref
  nifti_image *ref = nifti_image_read(refNii.c_str(), 0);
  if(ref == NULL) {
    cerr << "could not open " << refNii
	 << " for reading the reference nifti file" << endl;
    return false;
  }

  // make the ref header into our image header
  nifti_image *img = ref;
  img->datatype = DT_FLOAT32;
  img->nbyper = sizeof(float);
  free(img->fname);
  img->fname = (char*) malloc(outNii.length() * sizeof(char));
  strcpy(img->fname, outNii.c_str());

  // allocate and copy data

  img->data = (void*) malloc(img->nbyper*img->nvox);
  memcpy(img->data, data, img->nbyper*img->nvox);

  // write the file
  nifti_image_write(img);

  // clean up
  nifti_image_free(img);

  return true;
}
