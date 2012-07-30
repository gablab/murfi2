//serve a 4d nifti image file simulating the scanner for murfi
//
// Oliver Hinds <ohinds@mit.edu> 2011-04-14

#include"ace/SOCK_Stream.h"
#include"ace/SOCK_Connector.h"
#include"ace/SOCK_Stream.h"
#include<strstream>
#include<iostream>

#include"nifti1_io.h"

#include"../../src/io/RtExternalSenderImageInfo.h"

#include<vnl/vnl_matrix.h>
#include<vnl/vnl_vector.h>
#include<vnl/vnl_matrix_fixed.h>
#include"../../src/util/printVnl44Mat.cpp"

using namespace std;

int firstImg = 0;

void usage(char *execname) {
  cerr << "usage: " << execname
       << " niftifile "
       << " ["
       << "host "
       << "port "
       << "tr (seconds)"
       << "]"
       << endl;
}

int ACE_TMAIN (int argc, ACE_TCHAR *argv[]) {
  if(argc < 2) {
    cerr << "filename is required" << endl;
    usage(argv[0]);
    return 1;
  }

  // args
  int arg = 1;
  string niifile(argv[arg++]);
  string host(argc > arg ? argv[arg++] : "localhost");
  int port = argc > arg ? atoi(argv[arg++]) : 15000;
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

  // AK: extracting rotation matrix

  // scaling matrix
  vnl_matrix_fixed<double,4,4> InvscaleMat;
  InvscaleMat.set_identity();
  InvscaleMat.put(0,0, 1/voxDim[0]);
  InvscaleMat.put(1,1, 1/voxDim[1]);
  InvscaleMat.put(2,2, 1/voxDim[2]);


  // rotation matrix
  vnl_matrix_fixed<double,4,4> vxl2rasMat;
  vxl2rasMat.set_identity();

  vxl2rasMat.put(0,0, vols->sto_xyz.m[0][0]);
  vxl2rasMat.put(0,1, vols->sto_xyz.m[0][1]);
  vxl2rasMat.put(0,2, vols->sto_xyz.m[0][2]);
  vxl2rasMat.put(0,3, vols->sto_xyz.m[0][3]);

  vxl2rasMat.put(1,0, vols->sto_xyz.m[1][0]);
  vxl2rasMat.put(1,1, vols->sto_xyz.m[1][1]);
  vxl2rasMat.put(1,2, vols->sto_xyz.m[1][2]);
  vxl2rasMat.put(1,3, vols->sto_xyz.m[1][3]);

  vxl2rasMat.put(2,0, vols->sto_xyz.m[2][0]);
  vxl2rasMat.put(2,1, vols->sto_xyz.m[2][1]);
  vxl2rasMat.put(2,2, vols->sto_xyz.m[2][2]);
  vxl2rasMat.put(2,3, vols->sto_xyz.m[2][3]);

  vnl_matrix_fixed<double,4,4> rotMat;
  rotMat = InvscaleMat*vxl2rasMat; 

  cout << "Inverse scale mat: " << endl;
  printVnl44Mat(InvscaleMat);

  cout << "vxl2ras mat: " << endl;
  printVnl44Mat(vxl2rasMat);

  cout << "rotation matrix: " << endl;
  printVnl44Mat(rotMat);

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
    cout << "made connection, sending image " << i+1 << endl;

    //    nifti_image *vol;
    //    copyTrToVol(vols,i,vol);

    // mosaic and send

    RtExternalImageInfo *ei = new RtExternalImageInfo();

    unsigned int mosaicSide = (int) sqrt(matrixSize*matrixSize
                                         *pow(ceil(sqrt((double)numSlices)),2));
    ei->nCol = matrixSize;
    ei->nLin = matrixSize;

    unsigned int numPix = mosaicSide*mosaicSide;

    ei->iNoOfImagesInMosaic = numSlices;
    ei->iMosaicGridSize = ceil(sqrt(numSlices));

    ei->dThick = vols->pixdim[3]/1.1;

    cout 
        << "nCol " <<  ei->nCol << " "
        << "nLin " <<  ei->nLin << " "
        << "iNoOfImagesInMosaic " <<  ei->iNoOfImagesInMosaic << " "
        << "iMosaicGridSize " <<  ei->iMosaicGridSize << endl;
/*
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
*/
  
    ei->dRowSag = rotMat[0][0];
    ei->dRowCor = rotMat[0][1];
    ei->dRowTra = rotMat[0][2];

    ei->dColSag = rotMat[1][0];
    ei->dColCor = rotMat[1][1];
    ei->dColTra = rotMat[1][2];

    ei->dNorSag = rotMat[2][0];
    ei->dNorCor = rotMat[2][1];
    ei->dNorTra = rotMat[2][2];

    ei->dPosSag = rotMat[0][3];
    cout << "dPosSag " << rotMat[0][3] << endl;
    ei->dPosCor = rotMat[1][3];
    ei->dPosTra = rotMat[2][3];

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

    //// send nonmoco image
    cout << "sending img  " << ei->iAcquisitionNumber << endl;

    //ei->displayImageInfo();

    char *data = new char[ei->iSizeOfRtExternalImageInfo];
    ei->bIsMoCo = false;
    data = ei->convertToScannerDataArray();
    cout << "sending info of size " << ei->iSizeOfRtExternalImageInfo << endl;
    stream.send_n (data, ei->iSizeOfRtExternalImageInfo);

    cout << "sending img of size " << ei->lImageDataLength << endl;

    int sent = stream.send_n(newdata, ei->lImageDataLength);

    cout << "sent " << sent << endl;

    stream.close();

    //    //// send moco image
    if(connector.connect (stream, my_addr)) {
      break;
    }

    cout << "sending moco img  " << ei->iAcquisitionNumber << endl;

    ei->bIsMoCo = true;
    data = ei->convertToScannerDataArray();
    cout << "sending info of size " << ei->iSizeOfRtExternalImageInfo << endl;
    stream.send_n (data, ei->iSizeOfRtExternalImageInfo);
    delete data;

    cout << "sending img of size " << ei->lImageDataLength << endl;

    sent = stream.send_n(newdata, ei->lImageDataLength);

    cout << "sent " << sent << endl;

    stream.close();

    usleep(1000000*tr);

    delete ei;
    delete [] newdata;
  }

  if(i == 0) {
    cout << "failed to connect" << endl;
  }

  return 0;
}
