
#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Connector.h"
#include"ace/SOCK_Stream.h"
#include<strstream>
#include<iostream>

#include"nifti1_io.h"

#include"../../src/io/RtExternalSenderImageInfo.h"

using namespace std;

int firstImg = 0;

void usage(char *execname) {
  cerr << "usage: " << execname
       << " niftifile "
       << " ["
       << "host "
       << "port "
       << "tr "
       << "]"
       << endl;
}

// \// convert from a mosaic representation
// \bool mosaic(nifti_image *img, int matrixSize, int numSlices) {
// \
// \  //sqrt(matrixSize^2*ceil(sqrt(numSlices))^2);
// \  img->ndim = 2;
// \  img->dim[0] = 2;
// \  img->dim[1] = (int) sqrt(matrixSize*matrixSize*pow(ceil(sqrt((double)numSlices)),2));
// \  img->nx = img->dim[1];
// \  img->dim[2] = (int)sqrt(matrixSize*matrixSize*pow(ceil(sqrt((double)numSlices)),2));
// \  img->ny = img->dim[2];
// \  int numPix = img->dim[1]*img->dim[2];
// \  img->nvox = numPix;
// \
// \  // reshape the data
// \  short *newdata = new short[numPix];
// \  short *data = (short*) img->data;
// \
// \  unsigned int newrow, newcol, oldslc, newind;
// \  unsigned int sqMatrixSize = matrixSize*matrixSize;
// \  for(unsigned int i = 0; i < numPix; i++) {
// \    oldslc = i/sqMatrixSize;
// \
// \    // these expressions are bracketed carefully to truncate
// \    // DO NOT SIMPLIFY THESE EXPRESSIONS
// \    newrow = (i/matrixSize)%matrixSize + matrixSize*(i/(img->dim[1]*matrixSize));
// \    newcol = (matrixSize*(i/sqMatrixSize) + (i%matrixSize))%img->dim[2];
// \    newind = newrow*img->dim[2]+newcol;
// \
// \    // copy if within slices
// \    if(oldslc < numSlices) {
// \      newdata[newind] = data[i];
// \    }
// \    else { // fill the blank panels with zeros
// \      newdata[newind] = 0;
// \    }
// \  }
// \
// \  img->data = (void*) newdata;
// \
// \
// \  return true;
// \}
// \
// \void copyTrToVol(nifti_image *vols, unsigned int tr, nifti_image *vol) {
// \  *vol = *vols;
// \
// \  // hdr
// \  strcpy(vol->fname,""); //filename.c_str());
// \  strcpy(vol->iname,""); //,filename.c_str());
// \
// \  // dims
// \  vol->dim[0]--;
// \  vol->dim[4] = 1;
// \  vol->nt = 1;
// \  vol->nvox = vol->dim[1]*vol->dim[2]*vol->dim[3]; 
// \  
// \  // copy data
// \  vol->data = (void*) malloc(vol->nbyper*vol->nvox);
// \  for(unsigned int i = 0; i < vol->nvox; i++) {
// \    ((short *)vol->data)[i] = ((short*) vols->data)[tr*vol->nvox+i];
// \  }
// \}


int ACE_TMAIN (int argc, ACE_TCHAR *argv[]) {
  if(argc < 2) {
    cerr << "filename is required" << endl;
    usage(argv[0]);
    return 1;
  }

  // args
  int arg = 1;
  string niifile(argv[arg++]);
  int port = argc > arg ? atoi(argv[arg++]) : 15000;
  string host(argc > arg ? argv[arg++] : "localhost");
  float inputTr = (argc > arg ? atof(argv[arg++]) : -1);

  // Local server address.
  ACE_INET_Addr my_addr (port, host.c_str());
  // Data transfer object.
  ACE_SOCK_Stream stream;
  // Initialize the connector.
  ACE_SOCK_Connector connector;

  // load the image
  nifti_image *vols = nifti_image_read(niifile.c_str(), 0);
  if(vols == NULL) {
    cerr << "could not open " << niifile << " for reading a nifti image"
	 << endl;
    usage(argv[0]);
    return 1;
  }

  // get info from the header
  unsigned int ndim = vols->dim[0];
  unsigned int matrixSize = vols->dim[1];
  unsigned int numSlices = vols->dim[3];
  unsigned int numImgs = vols->dim[4];

  float voxDim[3];
  voxDim[0] = vols->pixdim[1];
  voxDim[1] = vols->pixdim[2];
  voxDim[2] = vols->pixdim[3];

  float tr = (inputTr < 0) ? vols->pixdim[4] : inputTr;

  // validate
  if(ndim != 4) {
    cerr << "only 4D nifti files are supported" << endl;
    usage(argv[0]);
    return 1;
  }

  if(vols->datatype != DT_SIGNED_SHORT) {
    cerr << "only signed short nifti files are supported" << endl;
    usage(argv[0]);
    return 1;
  }

  // read the volumes
  nifti_image_load(vols);

  // keep making new connections while we havent sent the whole series
  unsigned int i = 0;
  for(; i < numImgs && !connector.connect (stream, my_addr); i++) {
    cout << "made connection, sending image " << i << endl;

//    nifti_image *vol;
//    copyTrToVol(vols,i,vol);

    // mosaic and send

    RtExternalImageInfo *ei = new RtExternalImageInfo();

    unsigned int mosaicSide = (int) sqrt(matrixSize*matrixSize
					 *pow(ceil(sqrt((double)numSlices)),2));
    ei->nCol = matrixSize;
    ei->nLin = matrixSize;

    unsigned int numPix = mosaicSide*mosaicSide;

    ei->bIsMoCo = true;

    ei->iNoOfImagesInMosaic = numSlices;
    ei->iMosaicGridSize = ceil(sqrt(numSlices));

    ei->dThick = vols->pixdim[3];

    cout 
      << "nCol " <<  ei->nCol << " "
      << "nLin " <<  ei->nLin << " "
      << "iNoOfImagesInMosaic " <<  ei->iNoOfImagesInMosaic << " "
      << "iMosaicGridSize " <<  ei->iMosaicGridSize << endl;

    ei->dRowSag = vols->sto_xyz.m[0][0];
    ei->dRowCor = vols->sto_xyz.m[0][1];
    ei->dRowTra = vols->sto_xyz.m[0][2];

    ei->dColSag = vols->sto_xyz.m[1][0];
    ei->dColCor = vols->sto_xyz.m[1][1];
    ei->dColTra = vols->sto_xyz.m[1][2];

    ei->dNorSag = vols->sto_xyz.m[2][0];
    ei->dNorCor = vols->sto_xyz.m[2][1];
    ei->dNorTra = vols->sto_xyz.m[2][2];

    ei->dPosSag = vols->sto_xyz.m[3][0];
    ei->dPosCor = vols->sto_xyz.m[3][1];
    ei->dPosTra = vols->sto_xyz.m[3][2];

    ei->lImageDataLength = vols->nbyper*numPix;
    ei->lNumberOfPixels = numPix;

    // mosaic
    short *newdata = new short[numPix];
    short *imgdata 
      = (short*) vols->data + i*vols->dim[1]*vols->dim[2]*vols->dim[3];

    unsigned int newrow, newcol, oldslc, newind;
    unsigned int sqMatrixSize = matrixSize*matrixSize;
    for(unsigned int p = 0; p < numPix; p++) {
      oldslc = p/sqMatrixSize;

      // these expressions are bracketed carefully to truncate
      // DO NOT SIMPLIFY THESE EXPRESSIONS
      newrow = (p/matrixSize)%matrixSize + matrixSize*(p/(mosaicSide*matrixSize));
      newcol = (matrixSize*(p/sqMatrixSize) + (p%matrixSize))%mosaicSide;
      newind = newrow*mosaicSide+newcol;

      // copy if within slices
      if(oldslc < numSlices) {
	newdata[newind] = imgdata[p];
      }
      else { // fill the blank panels with zeros
	newdata[newind] = 0;
      }
    }
    

    ei->iAcquisitionNumber = i+1;
    cout << "sending img  " << ei->iAcquisitionNumber << endl;

    char *data = new char[ei->iSizeOfRtExternalImageInfo];
    data = ei->convertToScannerDataArray();
    cout << "sending info of size " << ei->iSizeOfRtExternalImageInfo << endl;
    stream.send_n (data, ei->iSizeOfRtExternalImageInfo);
    delete data;

    cout << "sending img of size " << ei->lImageDataLength << endl;

    int sent = stream.send_n(newdata, ei->lImageDataLength);

    cout << "sent " << sent << endl;

    usleep(1000000*tr);

    stream.close();

    delete ei;
    delete [] newdata;
  }

  if(i == 0) {
    cout << "failed to connect" << endl;
  }

  return 0;
}

////    
////    void copyTrToVol(nifti_image *vols, unsigned int i, nifti_image &vol) {
////      vol = *vols;
////    
////    //  // info
////    //  vol.iname_offset = 348;
////    //  vol.swapsize = 0;
////    //  //vol.analyze75_orient = a75_transverse_unflipped;
////    
////      // filename
////    //  vol.fname = (char*) malloc((filename.length()+1)*sizeof(char));
////      strcpy(vol.fname,""); //filename.c_str());
////    //  vol.iname = (char*) malloc((filename.length()+1)*sizeof(char));
////      strcpy(vol.iname,""); //,filename.c_str());
////    
////      // dims
////    //  vol.ndim = (dims.size() >= 4) ? dims.size() : 4;
////      vol.dim[0]--;// = vol.ndim;
////      vol.dim[4] = 1;
////    //  for(int ind = 1; ind < 8; ind++) {
////    //    vol.dim[ind] = 1;
////    //    vol.pixdim[ind] = 1;
////    //  }
////    //
////    //  for(unsigned int ind = 1; ind <= dims.size(); ind++) {
////    //    vol.dim[ind] = dims[ind-1];
////    //    vol.pixdim[ind] = pixdims[ind-1];
////    //  }
////    
////    //  // crappy
////    //  switch(vol.ndim) {
////    //    vol.nw = vol.dim[7];
////    //    vol.dw = vol.pixdim[7];
////    //  case 6:
////    //    vol.nv = vol.dim[6];
////    //    vol.dv = vol.pixdim[6];
////    //  case 5:
////    //    vol.nu = vol.dim[5];
////    //    vol.du = vol.pixdim[5];
////    //  case 4:
////    //    vol.nt = vol.dim[4];
////    //    vol.dt = vol.pixdim[4];
////    //  case 3:
////      vol.nz = vol.dim[3];
////      vol.dz = vol.pixdim[3];
////    //  case 2:
////      vol.ny = vol.dim[2];
////      vol.dy = vol.pixdim[2];
////    //  case 1:
////      vol.nx = vol.dim[1];
////      vol.dx = vol.pixdim[1];
////    //    break;
////    //  default:
////    //    break;
////    //  }
////    
////      // img data info
////      vol.nvox = vol.dim[1]*vol.dim[2]*vol.dim[3]; //numPix;
////      //vol.nbyper = bytesPerPix;
////    
////    //  // data type
////    //  switch(bytesPerPix) {
////    ////  case sizeof(bool):
////    ////    vol.datatype = DT_BINARY;
////    ////    break;
////    //  case sizeof(short):
////    //    vol.datatype = DT_SIGNED_SHORT;
////    //    break;
////    //  case sizeof(double):
////    //    vol.datatype = DT_DOUBLE;
////    //    break;
////    //  default:
////    //    vol.datatype = DT_DOUBLE;
////    //    break;
////    //  }
////    //
////    //  // nifti type (NIFTI-1 (1 file)
////    //  vol.nifti_type = 1;
////    //
////    //  vol.xyz_units = NIFTI_UNITS_MM;
////    //  vol.time_units = NIFTI_UNITS_MSEC;
////    //
////    //  // change this to be read from dicom header!!
////    //  for(int i = 0; i < 4; i++) {
////    //    for(int j = 0; j < 4; j++) {
////    //      vol.qto_xyz.m[i][j] = vxl2ras[i][j];
////    //      vol.sto_xyz.m[i][j] = vxl2ras[i][j];
////    //    }
////    //  }
////    //
////    //  vol.sform_code = 1;
////    //  vol.qform_code = 0;
////    //  vol.quatern_b = 0;
////    //  vol.quatern_c = 0;
////    //  vol.quatern_d = 0;
////    //  vol.qoffset_x = 0;
////    //  vol.qoffset_y = 0;
////    //  vol.qoffset_z = 0;
////    //
////    //  vol.freq_dim = 1;
////    //  vol.phase_dim = 2;
////    //  vol.slice_dim = 3;
////    //
////    //  vol.cal_min = 0;
////    //  vol.cal_max = 0;
////    //
////    //  vol.toffset = 0;
////    //
////    //  vol.slice_code = 0;
////    //  vol.scl_slope = 0;
////    //  vol.scl_inter = 0;
////    //  vol.slice_start = 0;
////    //  vol.slice_end = 0;
////    //  vol.slice_duration = 0;
////    //
////    //  vol.intent_code = NIFTI_INTENT_NONE;
////    //  vol.intent_p1 = 0;
////    //  vol.intent_p2 = 0;
////    //  vol.intent_p3 = 0;
////    //  strcpy(vol.intent_name,"");
////    //
////    //  strcpy(vol.descrip,"");
////    //  strcpy(vol.aux_file,"");
////    //
////    //  vol.swapsize = 0;
////    //
////    //  vol.num_ext = 0;
////    //  vol.ext_list = NULL;
////      
////    }
