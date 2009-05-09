/******************************************************************************
 * template class definition for a data image
 * 
 * Oliver Hinds <ohinds@mit.edu> 2008-09-03 
 * 
 *****************************************************************************/

#include"ace/Log_Msg.h"
#include<fstream>
#include"RtFSLInterface.h"

using namespace std;

// default constructor
template<class T>
RtDataImage<T>::RtDataImage() : RtData(),
				magicNumber(MAGIC_NUMBER),
				filename(""),
				alignOnRead(false),
				mosaicOnRead(false),
				unMosaicOnRead(false),
				flipLROnRead(false),
				imgDataLen(0),
				numPix(0) {
  ACE_TRACE(("RtDataImage<T>::RtDataImage()"));

  //addToID("image");
  data = NULL;
  bytesPerPix = sizeof(T);
  matrixSize = 64;
  numSlices = 1;
  sliceThick = 1;
  sliceGap = 0.1;
  dims.reserve(4);
  pixdims.reserve(4);


  isMosaiced = false;
}

// constructor that accepts a filename to read an image from
template<class T>
RtDataImage<T>::RtDataImage(const string &filename) : RtData(), data(NULL),
						      magicNumber(MAGIC_NUMBER),
						      filename(""),
						      alignOnRead(false),
						      mosaicOnRead(false),
						      unMosaicOnRead(false),
						      flipLROnRead(false),
						      imgDataLen(0),
						      numPix(0),
						      bytesPerPix(sizeof(unsigned short)) {
  ACE_TRACE(("RtDataImage<T>::RtDataImage(string)"));

  isMosaiced = false;
  matrixSize = 64;
  numSlices = 1;
  sliceThick = 1;
  sliceGap = 0.1;
  dims.reserve(4);
  pixdims.reserve(4);
  //addToID("image");
  bytesPerPix = sizeof(T);
  data = NULL;

  read(filename);

  vxl2ras.set_identity();
  ras2ref.set_identity();

  isMosaiced = false;
}

// construct from raw bytes sent by RtInputScannerImages
// BE CAREFUL WITH THIS
//template<class T>
//RtDataImage<T>::RtDataImage(char *bytes, unsigned int len) : RtData(),
//       magicNumber(MAGIC_NUMBER),
//       filename(""),
//       imgDataLen(0),
//       numPix(0),
//       bytesPerPix(sizeof(unsigned short)),
//       vxl2ras(gsl_matrix_calloc(4,4)),
//       ras2ref(gsl_matrix_calloc(4,4)) {
//  ACE_TRACE(("RtDataImage<T>::RtDataImage(char*,unsigned int)"));
//
//  addToID("image");
//
//  // try to do some checks
//  if(len < sizeof(RtDataImageInfo)) {
//    return;
//  }
//
//  // extract the info
//  memcpy(&info,bytes,sizeof(RtDataImageInfo));
//
//  // check if we have enough for the image data
//  if(len-sizeof(RtDataImageInfo) != info.imgDataLen) {
//    return;
//  }
//
//  data = new T[info.numPix];
//  memcpy(data,bytes+sizeof(RtDataImageInfo),info.imgDataLen);
//}


// construct from an image info struct and (possibly blank) data
//template<class T>
//RtDataImage<T>::RtDataImage(RtDataImageInfo &_info, T *_data)
//    : RtData(),
//       magicNumber(MAGIC_NUMBER),
//       filename(""),
//       imgDataLen(0),
//       numPix(0),
//       bytesPerPix(sizeof(unsigned short)),
//       vxl2ras(gsl_matrix_calloc(4,4)),
//       ras2ref(gsl_matrix_calloc(4,4))  {
//  ACE_TRACE(("RtDataImage<T>::RtDataImage(RtDataImageInfo,T*)"));
//
//  addToID("image");
//
//  setImage(_info, _data);
//}

// construct from another image (deep copy)
// only use this with like datatypes
template<class T>
RtDataImage<T>::RtDataImage(RtDataImage &img) : RtData() {
  ACE_TRACE(("RtDataImage<T>::RtDataImage(RtDataImage)"));

  // copy all fields
  (*this) = *img;

  // allocate and copy the matrices
  vxl2ras = img.vxl2ras;

  ras2ref = img.ras2ref;

  // allocate and copy the data
  if(DEBUG_LEVEL & ALLOC) {
    cerr << "1 allocating data for " << this << endl; cerr.flush();
  }

  data = new T[numPix];
  if(img.bytesPerPix == bytesPerPix) {
    memcpy(data, img.data, imgDataLen);
  }
}

// destructor
template<class T>
RtDataImage<T>::~RtDataImage() {
  ACE_TRACE(("RtDataImage<T>::~RtDataImage"));

  // free data
  if(data != NULL) {
    if(DEBUG_LEVEL & ALLOC) {
      cerr << "1 deleting data for " << this << endl; cerr.flush();
    }
    delete [] data;
  }
}

// lock the data (read/write)
template<class T>
void RtDataImage<T>::lock() {
  ACE_TRACE(("RtDataImage::lock"));
  mutex.acquire();
}

// unlock the data
template<class T>
void RtDataImage<T>::unlock() {
  ACE_TRACE(("RtDataImage::unlock"));
  mutex.release();
}

// print info about this image
template<class T>
void RtDataImage<T>::printInfo(ostream &os) {
  ACE_TRACE(("RtDataImageInfo::print"));

  int wid = 30;

  os << setiosflags(ios::left);

  os << "---------------------------" << endl
     << dataID                            << endl
     << "---------------------------" << endl
     << "magic number: " << magicNumber << endl
     << setw(wid) << "ndims: / dim1 / dim2... /" << dims.size() << ": / ";

  for(vector<int>::iterator i = dims.begin(); i != dims.end(); i++) {
    os << *i << " / ";
  }

  os << setw(wid) << "pixdim1 / pixdim2... /: ";

  for(vector<double>::iterator i = pixdims.begin(); i != pixdims.end(); i++) {
    os << *i << " / ";
  }

  os << endl
     << setw(wid) << "imgDataLen in bytes " << imgDataLen << endl
     << setw(wid) << "numPix" << numPix << endl
     << setw(wid) << "bytesPerPix" << bytesPerPix << endl;

  os << setw(wid) << "vxl2ras transform";
  for(int i = 0; i < 4; i++) {
    if(i > 0) {
      os << setw(wid) << "";
    }
    for(int j = 0; j < 4; j++) {
      os << vxl2ras[i][j] << " ";
    }
    os << endl;
  }

  os << setw(wid) << "ras2ref transform";
  for(int i = 0; i < 4; i++) {
    if(i > 0) {
      os << setw(wid) << "";
    }
    for(int j = 0; j < 4; j++) {
      os << ras2ref[i][j] << " ";
    }
    os << endl;
  }

  os << "---------------------------" << endl;
}

// serialize the data as xml for transmission or saving to a file
template<class T>
TiXmlElement *RtDataImage<T>::serializeAsXML() {
  TiXmlElement *element = new TiXmlElement( "error" );

  TiXmlText *text = new TiXmlText("this is a stub, no data serialization is supported for this type");
  element->LinkEndChild(text);

  return element;
}

// save the image to a file (already set filename)
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::save() {
  ACE_TRACE(("RtDataImage<T>::save"));

  if(filename == "") {
    return false;
  }

  return write(filename);
}

// write the image to a file
//  in
//   filename: string filename
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::write(const string &_filename) {
  ACE_TRACE(("RtDataImage<T>::write"));
  return writeNifti(_filename);
}

// write the image to a file
//  in
//   filename: string filename
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::writeRaw(const string &_filename) {
  ACE_TRACE(("RtDataImage<T>::write"));

  ofstream imgFile(_filename.c_str(), ios::out | ios::binary);

  if(imgFile.fail()) {
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("could not open %s for writing an image\n"),
				  _filename));
    imgFile.close();
    return false;
  }

  lock(); {

    // write info
    if(!writeInfo(imgFile)) {
      ACE_DEBUG((LM_DEBUG, ACE_TEXT("couldnt write info to %s\n"),_filename));
      imgFile.close();
      return false;
    }

    // write data
    if(!writeData(imgFile)) {
      ACE_DEBUG((LM_DEBUG,ACE_TEXT("couldnt write img data to %s\n"),_filename));
      imgFile.close();
      return false;
    }

  } unlock();

  imgFile.close();


  // store filename if its not set
  if(filename.empty()) {
    filename = _filename;
  }

  return true;
}

// write the image to a nifti file
//  in
//   filename: string filename
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::writeNifti(const string &_filename) {
  ACE_TRACE(("RtDataImage<T>::writeNifti"));

  // store filename if its not set
  filename = _filename;

  // fill the nifti struct with our data
  nifti_image *img = (nifti_image*) malloc(sizeof(nifti_image));

  lock(); {

    copyInfo(img);

    // allocate and copy data
    img->data = (void*) malloc(img->nbyper*img->nvox);
    memcpy(img->data, data, img->nbyper*img->nvox);

    // debugging
    //nifti_image_infodump(img);

    // write the file
    nifti_image_write(img);
  } unlock();

  nifti_image_free(img);

  return true;
}

// constants for field lengths
#define FILESTR_LEN 256
#define MAX_NDIMS   10

// write the info (all but data) to a stream
//  in
//   stream to write to
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::writeInfo(ostream &os) {
  if(os.fail()) return false;

  // write all the fields
  os.write((char*) &magicNumber, sizeof(unsigned int));

  // filename
  char filecstr[FILESTR_LEN];
  strcpy(filecstr,filename.c_str());
  os.write(filecstr, FILESTR_LEN*sizeof(char));

  // dims
  unsigned int ndims = dims.size();
  os.write((char*) &ndims, sizeof(unsigned int));

  unsigned int dimarr[MAX_NDIMS];
  int ind = 0;
  for(vector<int>::iterator i = dims.begin(); i != dims.end(); i++, ind++) {
    dimarr[ind] = *i;
  }
  os.write((char*) dimarr, MAX_NDIMS*sizeof(unsigned int));

  // pixdims
  double pixdimarr[MAX_NDIMS];
  ind = 0;
  for(vector<double>::iterator i = pixdims.begin(); i != pixdims.end();
      i++, ind++) {
    pixdimarr[ind] = *i;
  }
  os.write((char*) pixdimarr, MAX_NDIMS*sizeof(double));

  // img data info
  os.write((char*) &imgDataLen, sizeof(unsigned long));
  os.write((char*) &numPix, sizeof(unsigned long));
  os.write((char*) &bytesPerPix, sizeof(unsigned short));

  // matrices
  //os.write((char*) gsl_matrix_ptr(vxl2ras,0,0), 4*4*sizeof(double));
  //os.write((char*) gsl_matrix_ptr(ras2ref,0,0), 4*4*sizeof(double));

  // min/max info
  char mmset = (char) minMaxSet;
  os.write((char*) &mmset, sizeof(char));
  os.write((char*) &minVal, sizeof(T));
  os.write((char*) &maxVal, sizeof(T));

  return os.good();
}

// write the data to a stream
//  in
//   stream to write to
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::writeData(ostream &os) {
  if(os.fail()) return false;

  os.write((char*) data, imgDataLen);

  return os.good();
}

// load the image to a file (already set filename)
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::load() {
  ACE_TRACE(("RtDataImage<T>::load"));

  if(filename == "") {
    return false;
  }

  bool ret = read(filename);

  return ret;
}


// read the image from a file
//  in
//   filename: string filename
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::read(const string &_filename) {
  ACE_TRACE(("RtDataImage<T>::read"));
  bool suc = readNifti(_filename);

  //  unmosaic (guessed by default, or read the parm)
  if(suc && (seemsMosaic() || unMosaicOnRead)) {
    unmosaic();
  }

  // mosaic if we need to
  if(suc && mosaicOnRead) {
    mosaic();
  }

  // flip if we need to
  if(suc && flipLROnRead) {
    flipLR();
  }
  
  return suc;
}

// read the image header from a file
//  in
//   filename: string filename
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::readHeader(const string &_filename) {
  ACE_TRACE(("RtDataImage<T>::read"));
  bool suc = readNiftiHeader(_filename);

  return suc;
}

// read the image from a raw file
//  in
//   filename: string filename
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::readRaw(const string &_filename) {
  ACE_TRACE(("RtDataImage<T>::read"));

  // delete our current image data, if we have it
  if(data != NULL) {
    if(DEBUG_LEVEL & ALLOC) {
      cerr << "2 deleting data for " << this << endl; cerr.flush();
    }
    delete [] data;
  }

  ifstream imgFile(_filename.c_str(), ios::in | ios::binary);

  if(imgFile.fail()) {
    ACE_DEBUG((LM_DEBUG, ACE_TEXT("could not open %s for reading an image\n"),
				  _filename));
    imgFile.close();
    return false;
  }

  lock(); {

    // read the image info
    if(!readInfo(imgFile)) {
      ACE_DEBUG((LM_DEBUG, ACE_TEXT("error reading image info from %s\n"),
		 _filename));
      imgFile.close();
      return false;
    }

    // allocate new data
    if(DEBUG_LEVEL & ALLOC) {
      cerr << "2 allocating data for " << this << endl; cerr.flush();
    }
    data = new T[numPix];

    // read the image data
    if(!readData(imgFile)) {
      ACE_DEBUG((LM_DEBUG, ACE_TEXT("error reading image data from %s\n"),
		 _filename));
      imgFile.close();
      return false;
    }
  } unlock();

  imgFile.close();

  // store filename if its not set
  if(filename.empty()) {
    filename = _filename;
  }

  return true;
}

// read the header from a nifti1 image file
// WARNING! sets header info but does not read the pixels, can create
// a mismatch between data and info
//  in
//   filename: string filename
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::readNiftiHeader(const string &_filename) {
  ACE_TRACE(("RtDataImage<T>::readNiftiHeader"));

  nifti_image *img = nifti_image_read(_filename.c_str(), 0);
  if(img == NULL) {
    cerr << "could not open " << _filename << " for reading a nifti image"
	 << endl; 
    return false;
  }

  // validate the datatype
  if(img->nbyper != sizeof(T)) {
    cerr << _filename << " has " << img->nbyper
	 << " bytes/voxel but i was expecting " << sizeof(T)
	 << endl;
    return false;
  }

  // store filename if its not set
  if(filename.empty()) {
    filename = _filename;
  }

  lock(); {
    // transform nifti header into our image info struct
    setInfo(img);
  } unlock(); 

  nifti_image_free(img);

  return true;
}


// read the image from a nifti1 file
//  in
//   filename: string filename
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::readNifti(const string &_filename) {
  ACE_TRACE(("RtDataImage<T>::read"));

  nifti_image *img = nifti_image_read(_filename.c_str(), 0);
  if(img == NULL) {
    cerr << "could not open " << _filename << " for reading a nifti image"
	 << endl; 
    return false;
  }

  // validfate the datatype
  if(img->nbyper != sizeof(T)) {
    cerr << _filename << " has " << img->nbyper
	 << " bytes/voxel but i was expecting " << sizeof(T)
	 << endl;
    return false;
  }

  lock(); {

    // transform nifti header into our image info struct
    setInfo(img);

    // delete our current image data, if we have it
    if(data != NULL) {
      if(DEBUG_LEVEL & ALLOC) {
        cerr << "3 deleting data for " << this << endl; cerr.flush();
      }
      delete [] data;
    }

    // read the image data
    nifti_image_load(img);

    // allocate new data
    if(DEBUG_LEVEL & ALLOC) {
      cerr << "3 allocating data for " << this << endl; cerr.flush();
    }
    data = new T[img->nvox];
    memcpy(data, img->data, img->nbyper * img->nvox);

  }  unlock();

  // SCOPIC added to fix memory leakage  
  nifti_image_free(img);
    
  // store filename if its not set
  if(filename.empty()) {
    filename = _filename;
  }

  return true;
}

// read info from a stream
//  in
//   input stream to read from
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::readInfo(istream &is) {
  if(!is.good()) return false;

  // read all the fields
  is.read((char*) &magicNumber, sizeof(unsigned int));

  // filename
  char filecstr[FILESTR_LEN];
  is.read(filecstr, FILESTR_LEN*sizeof(char));
  filename = filecstr;

  // dims
  unsigned int ndims;
  is.read((char*) &ndims, sizeof(unsigned int));

  unsigned int dimarr[MAX_NDIMS];
  is.read((char*) dimarr, MAX_NDIMS*sizeof(unsigned int));

  dims.clear();
  for(unsigned int ind = 0; ind < ndims && ind < MAX_NDIMS; ind++) {
    dims.push_back(dimarr[ind]);
  }

  // pixdims
  double pixdimarr[MAX_NDIMS];
  is.read((char*) pixdimarr, MAX_NDIMS*sizeof(double));

  pixdims.clear();
  for(unsigned int ind = 0; ind < ndims && ind < MAX_NDIMS; ind++) {
    pixdims.push_back(pixdimarr[ind]);
  }

  // img data info
  is.read((char*) &imgDataLen, sizeof(unsigned long));
  is.read((char*) &numPix, sizeof(unsigned long));
  is.read((char*) &bytesPerPix, sizeof(unsigned short));

  // matrices
  //is.read((char*) gsl_matrix_ptr(vxl2ras,0,0), 4*4*sizeof(double));
  //is.read((char*) gsl_matrix_ptr(ras2ref,0,0), 4*4*sizeof(double));

  // min/max info
  char mmset;
  is.read((char*) &mmset, sizeof(char));
  minMaxSet = (bool) mmset;

  is.read((char*) &minVal, sizeof(T));
  is.read((char*) &maxVal, sizeof(T));

  return is.good();
}

// read data from a stream
//  in
//   input stream to read from
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::readData(istream &is) {
  if(is.fail()) return false;

  is.read((char*) data, imgDataLen);

  return is.good();
}

// set info from a nifti image header
//  in
//   nifti image struct
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::setInfo(nifti_image *hdr) {
  magicNumber = MAGIC_NUMBER;

  // filename
  filename = hdr->fname;

  // dims
  dims.clear();
  for(int ind = 0; ind < hdr->ndim && ind < MAX_NDIMS; ind++) {
    dims.push_back(hdr->dim[ind+1]);
  }

  // pixdims
  pixdims.clear();
  for(int ind = 0; ind < hdr->ndim && ind < MAX_NDIMS; ind++) {
    pixdims.push_back(hdr->pixdim[ind+1]);
  }

  // img data info
  numPix = hdr->nvox;
  bytesPerPix = hdr->nbyper;
  imgDataLen = numPix*hdr->nbyper;

  // vxl2ras
  vxl2ras.put(0,0,hdr->sto_xyz.m[0][0]);
  vxl2ras.put(0,1,hdr->sto_xyz.m[0][1]);
  vxl2ras.put(0,2,hdr->sto_xyz.m[0][2]);
  vxl2ras.put(0,3,hdr->sto_xyz.m[0][3]);
  vxl2ras.put(1,0,hdr->sto_xyz.m[1][0]);
  vxl2ras.put(1,1,hdr->sto_xyz.m[1][1]);
  vxl2ras.put(1,2,hdr->sto_xyz.m[1][2]);
  vxl2ras.put(1,3,hdr->sto_xyz.m[1][3]);
  vxl2ras.put(2,0,hdr->sto_xyz.m[2][0]);
  vxl2ras.put(2,1,hdr->sto_xyz.m[2][1]);
  vxl2ras.put(2,2,hdr->sto_xyz.m[2][2]);
  vxl2ras.put(2,3,hdr->sto_xyz.m[2][3]);
  vxl2ras.put(3,0,hdr->sto_xyz.m[3][0]);
  vxl2ras.put(3,1,hdr->sto_xyz.m[3][1]);
  vxl2ras.put(3,2,hdr->sto_xyz.m[3][2]);
  vxl2ras.put(3,3,hdr->sto_xyz.m[3][3]);

  minMaxSet = false;
  minVal = 0;
  maxVal = 0;

  return true;
}

// copy info from our image into a nifti image header
//  in
//   nifti image struct
//  out
//   success or failure
template<class T>
bool RtDataImage<T>::copyInfo(nifti_image *hdr) {
  // info
  hdr->iname_offset = 348;
  hdr->swapsize = 0;
  //hdr->analyze75_orient = a75_transverse_unflipped;

  // filename
  hdr->fname = (char*) malloc((filename.length()+1)*sizeof(char));
  strcpy(hdr->fname,filename.c_str());
  hdr->iname = (char*) malloc((filename.length()+1)*sizeof(char));
  strcpy(hdr->iname,filename.c_str());

  // dims
  hdr->ndim = (dims.size() >= 4) ? dims.size() : 4;
  hdr->dim[0] = hdr->ndim;
  for(int ind = 1; ind < 8; ind++) {
    hdr->dim[ind] = 1;
    hdr->pixdim[ind] = 1;
  }

  for(unsigned int ind = 1; ind <= dims.size(); ind++) {
    hdr->dim[ind] = dims[ind-1];
    hdr->pixdim[ind] = pixdims[ind-1];
  }

  // crappy
  switch(hdr->ndim) {
  case 7:
    hdr->nw = hdr->dim[7];
    hdr->dw = hdr->pixdim[7];
  case 6:
    hdr->nv = hdr->dim[6];
    hdr->dv = hdr->pixdim[6];
  case 5:
    hdr->nu = hdr->dim[5];
    hdr->du = hdr->pixdim[5];
  case 4:
    hdr->nt = hdr->dim[4];
    hdr->dt = hdr->pixdim[4];
  case 3:
    hdr->nz = hdr->dim[3];
    hdr->dz = hdr->pixdim[3];
  case 2:
    hdr->ny = hdr->dim[2];
    hdr->dy = hdr->pixdim[2];
  case 1:
    hdr->nx = hdr->dim[1];
    hdr->dx = hdr->pixdim[1];
    break;
  default:
    break;
  }

  // img data info
  hdr->nvox = numPix;
  hdr->nbyper = bytesPerPix;

  // data type
  switch(bytesPerPix) {
//  case sizeof(bool):
//    hdr->datatype = DT_BINARY;
//    break;
  case sizeof(short):
    hdr->datatype = DT_SIGNED_SHORT;
    break;
  case sizeof(double):
    hdr->datatype = DT_DOUBLE;
    break;
  default:
    hdr->datatype = DT_DOUBLE;
    break;
  }

  // nifti type (NIFTI-1 (1 file)
  hdr->nifti_type = 1;

  hdr->xyz_units = NIFTI_UNITS_MM;
  hdr->time_units = NIFTI_UNITS_MSEC;

  // change this to be read from dicom header!!
  for(int i = 0; i < 4; i++) {
    for(int j = 0; j < 4; j++) {
      hdr->qto_xyz.m[i][j] = vxl2ras[i][j];
      hdr->sto_xyz.m[i][j] = vxl2ras[i][j];
    }
  }

  hdr->sform_code = 1;
  hdr->qform_code = 0;
  hdr->quatern_b = 0;
  hdr->quatern_c = 0;
  hdr->quatern_d = 0;
  hdr->qoffset_x = 0;
  hdr->qoffset_y = 0;
  hdr->qoffset_z = 0;

  hdr->freq_dim = 1;
  hdr->phase_dim = 2;
  hdr->slice_dim = 3;

  hdr->cal_min = 0;
  hdr->cal_max = 0;

  hdr->toffset = 0;

  hdr->slice_code = 0;
  hdr->scl_slope = 0;
  hdr->scl_inter = 0;
  hdr->slice_start = 0;
  hdr->slice_end = 0;
  hdr->slice_duration = 0;

  hdr->intent_code = NIFTI_INTENT_NONE;
  hdr->intent_p1 = 0;
  hdr->intent_p2 = 0;
  hdr->intent_p3 = 0;
  strcpy(hdr->intent_name,"");

  strcpy(hdr->descrip,"");
  strcpy(hdr->aux_file,"");

  hdr->swapsize = 0;

  hdr->num_ext = 0;
  hdr->ext_list = NULL;

  return true;
}

// flip each slice LR
template<class T>
bool RtDataImage<T>::flipLR() {
  lock(); {

    for(unsigned int row = 0; row < getNumPix()/matrixSize; row++) {
      for(unsigned int j = 0, begInd = row*matrixSize; j < matrixSize/2; j++) {
	T tmp = data[begInd+j];
	data[begInd+j] = data[begInd+matrixSize-1-j];
	data[begInd+matrixSize-1-j] = tmp;
      }
    }

  }  unlock();


  return true;
}

// convert from a mosaic representation
template<class T>
bool RtDataImage<T>::unmosaic() {
  // validate
  if(!seemsMosaic()) { // dims
    cerr << "can't unmosaic an image that seemsMosaic()"
	 << endl;
    return false;
  }

  if(matrixSize == 0) { // matrix size set
    cerr << "can't unmosaic an image if i don't know the matrix size"
	 << endl;
    return false;
  }

  bool ret;
  lock(); {
    ret = unmosaicUnlocked();
  } unlock();

  return ret;
}

// convert from a mosaic representation
template<class T>
bool RtDataImage<T>::unmosaicUnlocked() {
  // validate
  if(!seemsMosaic()) { // dims
    cerr << "can't unmosaic an image that seemsMosaic()"
	 << endl;
    return false;
  }

  if(matrixSize == 0) { // matrix size set
    cerr << "can't unmosaic an image if i don't know the matrix size"
	 << endl;
    return false;
  }


  // build a new dim array
  int width = dims[0];
  int height = dims[1];

  dims.clear();
  dims.reserve(4);

  dims.push_back(matrixSize);
  dims.push_back(matrixSize);

  if(numSlices > 0) {
    dims.push_back(numSlices);
  }
  else {
    dims.push_back(width/matrixSize * height/matrixSize);
  }

  // build a new pixdim
  // scopic: mosaic image have might not to have 3rd dim
  if(pixdims.size() == 2) pixdims.push_back(0);
  pixdims[2] = sliceThick*(1+sliceGap);

  // reshape the data
  numPix = matrixSize*matrixSize*numSlices;
  T *newdata = new T[numPix];
  imgDataLen = numPix*sizeof(T);

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
    //    fprintf(stdout,"%d %d %d %d %d %d\n", data[i], newind, i, slc, row, col);
    if(slc >= numSlices) {
      continue;
    }

    newdata[newind] = data[i];
  }

  //  ofstream of;
  //  of.open("/tmp/imgarr.txt");
  //  for(unsigned int i=0; i < numPix; i++) {
  //    of << newdata[i] << endl;
  //    //cout << newdata[i] << endl;
  //  }
  //  of.close();

  if(DEBUG_LEVEL & ALLOC) {
    cerr << "4 deleting data for " << this << endl; cerr.flush();
  }
  delete [] data;


  if(DEBUG_LEVEL & ALLOC) {
    cerr << "4 allocating data for " << this << endl; cerr.flush();
  }
  data = newdata;
  isMosaiced = false;  

  return true;
}

// convert to a mosaic representation
template<class T>
bool RtDataImage<T>::mosaic() {
  // validate
  if(seemsMosaic()) {
    cerr << "can't mosaic an image that already seems to be"
	 << endl;
    return false;
  }

  if(dims[0] != dims[1]) { // must be same size
    cerr << "can't mosaic an image with different row and column sizes"
	 << endl;
    return false;
  }

  bool ret;
  lock(); {
    ret = mosaicUnlocked();
  } unlock();

  return ret;
}

// convert to a mosaic representation (private unlocked version)
template<class T>
bool RtDataImage<T>::mosaicUnlocked() {
  // validate
  if(seemsMosaic()) {
    cerr << "can't mosaic an image that already seems to be"
	 << endl;
    return false;
  }

  if(dims[0] != dims[1]) { // must be same size
    cerr << "can't mosaic an image with different row and column sizes"
	 << endl;
    return false;
  }

  if(dims.size() < 3) { // must be more than one slice
    return false;
  }

  // set the matrix size and number of slices
  matrixSize = dims[0];
  numSlices = dims[2];

  dims.clear();
  dims.reserve(2);

  //sqrt(matrixSize^2*ceil(sqrt(numSlices))^2);
  dims.push_back(getMosaicedWidth());
  dims.push_back(getMosaicedHeight());
  numPix = dims[0]*dims[1];

  // reshape the data
  T *newdata = new T[numPix];
  imgDataLen = numPix*sizeof(T);

  unsigned int newrow, newcol, oldslc, newind;
  unsigned int sqMatrixSize = matrixSize*matrixSize;
  for(unsigned int i = 0; i < numPix; i++) {
    oldslc = i/sqMatrixSize;

    // these expressions are bracketed carefully to truncate
    // DO NOT SIMPLIFY THESE EXPRESSIONS
    newrow = (i/matrixSize)%matrixSize + matrixSize*(i/(dims[0]*matrixSize));
    newcol = (matrixSize*(i/sqMatrixSize) + (i%matrixSize))%dims[1];
    newind = newrow*dims[1]+newcol;

    // copy if within slices
    if(oldslc < numSlices) {
      newdata[newind] = data[i];
    }
    else { // fill the blank panels with zeros
      newdata[newind] = 0;
    }
  }

  if(DEBUG_LEVEL & ALLOC) {
    cerr << "5 deleting data for " << this << endl; cerr.flush();
  }
  delete [] data;


  if(DEBUG_LEVEL & ALLOC) {
    cerr << "5 allocating data for " << this << endl; cerr.flush();
  }
  data = newdata;

  isMosaiced = true;

  return true;
}


// get a copy of the data in mosaiced format 
// (use getMosaicedWidth/Height to get size)
//  out
//   copy: pointer to the data that will be created
template<class T>
T* RtDataImage<T>::getMosaicedCopy() {
  T* copy;
  lock(); {

    bool mos = mosaicUnlocked();
    copy = new T[numPix];
    memcpy(copy, data, numPix*sizeof(T));
    if(mos) {
      unmosaicUnlocked();
    }

  } unlock();

  return copy;
}

// get the width of a potential mosaiced version of this data
template<class T>
unsigned int RtDataImage<T>::getMosaicedWidth() {
  if(matrixSize < 1 || numSlices < 1) {
    return 0;
  }

  return (int) sqrt( matrixSize*matrixSize 
		     * pow(ceil(sqrt((double)numSlices)), 2) );
}

// get the height of a potential mosaiced version of this data
template<class T>
unsigned int RtDataImage<T>::getMosaicedHeight() {
  if(matrixSize < 1 || numSlices < 1) {
    return 0;
  }

  return (int) matrixSize * ceil((double)numSlices 
				 / (getMosaicedWidth() / matrixSize));
}

// test for a mosaic representation
template<class T>
bool RtDataImage<T>::isMosaic() {
  return isMosaiced;
}

// test whether an images dimensions seem consistent with mosaic representation
template<class T>
bool RtDataImage<T>::seemsMosaic() {
  // see whether there are only two dimensions
  bool onlyTwo = dims.size() >= 2;
  for(int i = 2; i < dims.size(); i++) {
    if(dims[i] > 1) {
      onlyTwo = false;
      break;
    }
  }
  if(!onlyTwo) {
    return false;
  }

  // see if the two dimensions look like mosaiced dimensions
  // assumptions: if the matrix size is 32, 64, or 128 assume its a single slice
  // scopic: dims from 0 to 2
  return (dims[1] != dims[0])
    || !(dims[1] == 32 || dims[1] == 64 || dims[1] == 128);
}

// get the unique data id
template<class T>
RtDataID &RtDataImage<T>::getDataID() { 
  return dataID;
}

// initialize to all zeros
template<class T>
void RtDataImage<T>::initToZeros() {
  if(data == NULL) {
    return;
  }

  lock(); {
    for(unsigned int i = 0; i < numPix; i++) {
      data[i] = 0;
    }
  }  unlock();
}

// initialize to all nans
template<class T>
void RtDataImage<T>::initToNans() {
  if(data == NULL) {
    return;
  }

  lock(); {
    for(unsigned int i = 0; i < numPix; i++) {
      if(std::numeric_limits<T>::has_quiet_NaN) {
	data[i] = std::numeric_limits<T>::quiet_NaN();
      }
      else {
	data[i] = 0;
      }
    }
  }  unlock();
}

//************ sets *****************//

// get filename
template<class T>
string RtDataImage<T>::getFilename() const {
  return filename;
}

// get dimensions
template<class T>
vector<int> &RtDataImage<T>::getDims() {
  return dims;
}

// get vxl2ras
template<class T>
vnl_matrix_fixed<double,4,4> &RtDataImage<T>::getVxl2Ras() {
  return vxl2ras;
}

// get ras2ref
template<class T>
vnl_matrix_fixed<double,4,4> &RtDataImage<T>::getRas2Ref() {
  return ras2ref;
}

// get dimensions
template<class T>
int RtDataImage<T>::getDim(int i) {
  int ret;
  lock(); {
    ret = (int)dims.size() > i && i >= 0 ? dims[i] : -1;
  }  unlock();
  return ret;
}

// get voxel dimensions
template<class T>
vector<double> &RtDataImage<T>::getPixDims() {
  return pixdims;
}

// get vox dimensions
template<class T>
double RtDataImage<T>::getPixDim(int i) {
  double ret;
  lock(); {
    ret = pixdims.size() > i && i >= 0 ? pixdims[i] : -1; 
  }  unlock();
  return ret;
}

// get number of pix
template<class T>
unsigned int RtDataImage<T>::getNumPix() {
  unsigned int ret;
  lock(); {
    ret = numPix;
  }  unlock();
  return ret;
}

// get number of elements (same as num pix)
template<class T>
unsigned int RtDataImage<T>::getNumEl() {
  return getNumPix();
}

// get number of bytes per pix
template<class T>
unsigned short RtDataImage<T>::getBytesPerPix() {
  unsigned short ret;
  lock(); {
    ret = bytesPerPix;
  }  unlock();
  return ret;
}

// get pixel value
template<class T>
T RtDataImage<T>::getPixel(unsigned int i) {
  T ret;
  lock(); {
    ret = i < numPix ? data[i] : 0; 
  }  unlock();
  return ret;
}

// get element value (same as pixel)
template<class T>
T RtDataImage<T>::getElement(unsigned int i) {
  return getPixel(i);
}

// get data size
template<class T>
unsigned int RtDataImage<T>::getImgDataLen() {
  unsigned int ret;
  lock(); {
    ret = imgDataLen; 
  }  unlock();
  return ret;
}

// get a pointer to the image data
template<class T>
T *RtDataImage<T>::getDataCopy() {
  T* copy = new T[numPix];
  memcpy(copy, data, numPix*sizeof(T));
  return copy;
}

//************ sets *****************//

// set filename
template<class T>
void RtDataImage<T>::setFilename(string _filename) {
  filename = _filename;
}

// set pixel dim
template<class T>
void RtDataImage<T>::setPixDim(unsigned int i, double dim) {
  if(i < 0 || i >= pixdims.size()) {
    return;
  }
  pixdims[i] = dim;
}

// set pixel value
template<class T>
void RtDataImage<T>::setPixel(unsigned int i, T v) {
  lock(); {
    data[i] = v;
  }  unlock();
}

// set element value
template<class T>
void RtDataImage<T>::setElement(unsigned int i, T v) {
  return setPixel(i, v);
}

// sets the min and max pixel value for this data image
template<class T>
void RtDataImage<T>::setMinMax() {
  ACE_TRACE(("RtDataImage::setMinMax"));

  int mini = -1, maxi = -1;

  if(numPix < 1) return;

  maxVal = data[0];
  minVal = data[0];
  for(unsigned int i = 0; i < numPix; i++) {
    if(data[i] > maxVal) {
      maxVal = data[i];
      maxi = i;
    }
    if(data[i] < minVal) {
      minVal = data[i];
      mini = i;
    }
  }

  minMaxSet = true;
}

//************** transforms ******************//

// transform 1D index into RAS world coords
template<class T>
void RtDataImage<T>::getRASfrom1D(unsigned int i, double &r, double &a, double &s) {

}

// transform 2D index into RAS world coords
template<class T>
void RtDataImage<T>::getRASfrom2D(unsigned int i, unsigned int j,
		  double &r, double &a, double &s) {

}

// transform 3D index into RAS world coords
template<class T>
void RtDataImage<T>::getRASfrom3D(unsigned int i, unsigned int j, unsigned int k,
		  double &r, double &a, double &s) {

}

// transform 2D index into 1D index
template<class T>
unsigned int RtDataImage<T>::get1Dfrom2D(unsigned int i, unsigned int j) {
  // different answers based on how many dimensions the image is
  switch(dims.size()) {
  case 0:
    return 0;
  case 1:
    return i;
  case 2:
    return i*dims[1]+j;
  case 3:
    return i*dims[1]*ceil(sqrt(dims[2]))+j;
  default:
    return i;
  }
}

// transform 3D index into 1D index
template<class T>
unsigned int RtDataImage<T>::get1Dfrom3D(unsigned int i, unsigned int j, unsigned int k) {
  // different answers based on how many dimensions the image is
  switch(dims.size()) {
  case 0:
    return 0;
  case 1:
    return i;
  case 2:
    return i*dims[1]+j;
  case 3:
    return i*dims[1]*dims[2]+j*dims[2];
  default:
    return i;
  }
}

// transform RAS world coords into 1D index
template<class T>
unsigned int RtDataImage<T>::get1DfromRAS(double r, double a, double s) {
  // make vectors for transform
  gsl_vector *ras = gsl_vector_alloc(4);
  gsl_vector_set(ras,0,r);
  gsl_vector_set(ras,1,a);
  gsl_vector_set(ras,2,s);
  gsl_vector_set(ras,3,1.0);

  gsl_vector *vox = gsl_vector_alloc(4);
  gsl_vector_set(vox,0,r);
  gsl_vector_set(vox,1,a);
  gsl_vector_set(vox,2,s);
  gsl_vector_set(vox,3,1.0);

  return 0;
}

// transform 1D index into 2D index
template<class T>
void RtDataImage<T>::get2Dfrom1D(unsigned int i1D, unsigned int &i2D, unsigned int &j2D) {
  // different answers based on how many dimensions the image is
  switch(dims.size()) {
  case 0:
    i2D = 0;
    j2D = 0;
    break;
  case 1:
    i2D = i1D;
    j2D = 0;
    break;
  case 2:
    i2D = i1D/dims[1];
    j2D = i1D%dims[1];
    break;
  case 3:
    i2D = i1D/(dims[1]*ceil(sqrt(dims[2])));
    j2D = floor(fmod(i1D,(dims[1]*ceil(sqrt(dims[2])))));
  default:
    i2D = 0;
    j2D = 0;
    break;
  }
}

// transform 3D index into 2D index
template<class T>
void RtDataImage<T>::get2Dfrom3D(unsigned int i1D, unsigned int j1D, unsigned int k1D,
		 unsigned int &i2D, unsigned int &j2D) {

}

// transform 1D index into 3D index
template<class T>
void RtDataImage<T>::get3Dfrom1D(unsigned int i1D,
		 unsigned int &i3D, unsigned int &j3D, unsigned int &k3D) {

}

// transform 2D index into 3D index
template<class T>
void RtDataImage<T>::get2Dfrom2D(unsigned int i1D, unsigned int j1D,
		 unsigned int &i3D, unsigned int &j3D, unsigned int &k3D) {

}

// convert a siemens hhmmss.xxxxxx time string to an ACE_Date_Time type
template<class T>
ACE_Date_Time RtDataImage<T>::siemensTime2ACE_Date_Time(const char *tc) {
  string timeStr = tc;
  ACE_Date_Time t;
  t.hour(atol(timeStr.substr(0,2).c_str()));
  t.minute(atol(timeStr.substr(2,2).c_str()));
  t.second(atol(timeStr.substr(4,2).c_str()));
  t.microsec(atol(timeStr.substr(7,6).c_str()));
  return t;
}

#define SIEMENS_TIMESTR_LEN 13
// convert an ACE_Date_Time type to  siemens hhmmss.xxxxxx time string
template<class T>
string RtDataImage<T>::ACE_Date_Time2SiemensTime(const ACE_Date_Time &t) {
  char str[] = "hhmmss.xxxxxx";
  sprintf(str,"%02ld%02ld%02ld.%06ld",
	  t.hour(), t.minute(), t.second(), t.microsec());

  string s(str);
  return s;
}


