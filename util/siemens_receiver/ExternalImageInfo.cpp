/******************************************************************************
 * implementation of member functions to receive and send siemens
 * external sender data
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14
 *****************************************************************************/

#include"ExternalImageInfo.h"

#include<fstream>
using namespace std;


// default constructor
ExternalImageInfo::ExternalImageInfo()
  :
  iVersion(0),
  iSizeOfExternalImageInfo(EXTERNALSENDERSIZEOF),
  dFOVread(200),
  dFOVphase(200),
  dThick(5),
  nCol(256),
  nLin(256),
  iImageDataLength(294912),
  iNumberOfPixels(147456),
  iBytesPerPixel(2),
  dPosSag(0),
  dPosCor(0),
  dPosTra(0),
  dNorSag(0),
  dNorCor(0),
  dNorTra(0),
  dPhaSag(0),
  dPhaCor(0),
  dPhaTra(0),
  dRowSag(0),
  dRowCor(0),
  dRowTra(0),
  dColSag(0),
  dColCor(0),
  dColTra(0),
  dMoCoTransX(0.0),
  dMoCoTransY(0.0),
  dMoCoTransZ(0.0),
  dMoCoRotX(0.0),
  dMoCoRotY(0.0),
  dMoCoRotZ(0.0),
  bIsMoCo(false),
  iNoOfImagesInMosaic(1),
  iMosaicGridSize(1),
  bSwapReadPhase(0),
  iAcquisitionNumber(1),
  dTimeAfterStart(0),
  dTE(30),
  dTR(3000),
  dTI(0),
  dTriggerTime(0),
  iSliceIndex(0),
  iNumTrackCha(0),
  iCurrentTrackCha(0),
  iWindowCenter(737),
  iWindowWidth(1694),
  iAbsTablePosition(0),
  iDataSource(0),
  dTimeDelay(0)
{
  strcpy((char*) chID, "IFEI");

  chAcquisitionDate[0] = '\0';
  strcpy(chAcquisitionTime,"123456.789101");

  chImageTypeValue4[0][0] = '\0';

  strcpy(chframeOfReference,"123456.789101");

  iReserved[0] = 0;
  dReserved[0] = 0;
  chReserved[0] = '\0';
  cSeriesInstanceUID[0] = '\0';
}

// constructor for data from physical scanner
// this function does selective memcpy through the passed data because the 
// scanner sends packed data with a few trash spots, which were identified 
// manually with plenty of frustration. 
ExternalImageInfo::ExternalImageInfo(char *data, unsigned int len) {
  char trash[TRASHSIZE];
  char *readptr = data;

  // check that we have enough data to read iSizeOfExternalImageInfo
  if(len < 4*CHARSIZE + 2*INTSIZE) {
    cerr << "not enough data to read size of external image info" << endl;
    return;
  }

  memcpy(chID, readptr, 4*CHARSIZE);
  readptr += 4*CHARSIZE;
  
  memcpy(&iVersion, readptr, INTSIZE);
  readptr += INTSIZE;

  memcpy(&iSizeOfExternalImageInfo, readptr, INTSIZE);
  readptr += INTSIZE;

  // test that we have enough data
  if(len < (unsigned int) iSizeOfExternalImageInfo) {
    cerr << "not enough data to fill image info: " << len << " < " << iSizeOfExternalImageInfo << endl;
    return;
  }

  memcpy(trash, readptr, INTSIZE);
  readptr += INTSIZE;

  memcpy(&dFOVread, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(&dFOVphase, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(&dThick, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(&nCol, readptr, INTSIZE);
  readptr += INTSIZE;


  memcpy(&nLin, readptr, INTSIZE);
  readptr += INTSIZE;

  memcpy(&iImageDataLength, readptr, INTSIZE);
  readptr += INTSIZE;

  memcpy(&iNumberOfPixels, readptr, INTSIZE);
  readptr += INTSIZE;

  memcpy(&iBytesPerPixel, readptr, INTSIZE);
  readptr += INTSIZE;

  memcpy(trash, readptr, INTSIZE);
  readptr += INTSIZE;

  memcpy(&dPosSag, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;


  memcpy(&dPosCor, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;


  memcpy(&dPosTra, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;


  memcpy(&dNorSag, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(&dNorCor, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(&dNorTra, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;


  memcpy(&dPhaSag, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(&dPhaCor, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(&dPhaTra, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;


  memcpy(&dRowSag, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(&dRowCor, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(&dRowTra, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;


  memcpy(&dColSag, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(&dColCor, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(&dColTra, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;



  memcpy(&dMoCoTransX, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(&dMoCoTransY, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(&dMoCoTransZ, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(&dMoCoRotX, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(&dMoCoRotY, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(&dMoCoRotZ, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;



  memcpy(&bIsMoCo, readptr, sizeof(bool));
  readptr += sizeof(bool);

  memcpy(trash, readptr, 3);
  readptr += 3;

  memcpy(&iNoOfImagesInMosaic, readptr, INTSIZE);
  readptr += INTSIZE;

  memcpy(&iMosaicGridSize, readptr, INTSIZE);
  readptr += INTSIZE;

  memcpy(cSeriesInstanceUID, readptr, 65*CHARSIZE);
  readptr += 65*CHARSIZE;
    

  memcpy(&bSwapReadPhase, readptr, sizeof(bool));
  readptr += sizeof(bool);

  memcpy(trash, readptr, 2);
  readptr += 2;

 
  memcpy(&iAcquisitionNumber, readptr, INTSIZE);
  readptr += INTSIZE;

  memcpy(trash, readptr, 4);
  readptr += 4;


  memcpy(&dTimeAfterStart, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;


  memcpy(&dTE, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;


  memcpy(&dTR, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;


  memcpy(&dTI, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;


  memcpy(&dTriggerTime, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(chAcquisitionDate, readptr, ACQUISITION_DATE_LEN*CHARSIZE);
  readptr += ACQUISITION_DATE_LEN*CHARSIZE;
  
  memcpy(chAcquisitionTime, readptr, ACQUISITION_TIME_LEN*CHARSIZE);
  readptr += ACQUISITION_TIME_LEN*CHARSIZE;
  
  memcpy(&iSliceIndex, readptr, INTSIZE);
  readptr += INTSIZE;
  
  memcpy(&iNumTrackCha, readptr, INTSIZE);
  readptr += INTSIZE;
  
  memcpy(&iCurrentTrackCha, readptr, INTSIZE);
  readptr += INTSIZE;

  memcpy(trash, readptr, 3);
  readptr += 3;


  memcpy(&iWindowCenter, readptr, INTSIZE);
  readptr += INTSIZE;

  memcpy(&iWindowWidth, readptr, INTSIZE);
  readptr += INTSIZE;

  memcpy(chImageTypeValue4, readptr, _NO_OF_V4_ENTRIES*IMAGETYPE_V4_LEN*CHARSIZE);
  readptr += _NO_OF_V4_ENTRIES*IMAGETYPE_V4_LEN*CHARSIZE;


  memcpy(&iAbsTablePosition, readptr, INTSIZE);
  readptr += INTSIZE;

  memcpy(chframeOfReference, readptr, FOR_ARRAY_LEN*CHARSIZE);
  readptr += FOR_ARRAY_LEN*CHARSIZE;

  memcpy(&dTimeDelay, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(trash, readptr, 16*INTSIZE);
  readptr += 16*INTSIZE;
  
  memcpy(trash, readptr, 16*DOUBLESIZE);
  readptr += 16*DOUBLESIZE;
  
  memcpy(trash, readptr, 32*CHARSIZE);
  readptr += 32*CHARSIZE;
  
  memcpy(trash, readptr, 8*CHARSIZE);
  readptr += 8*CHARSIZE;  

  //displayImageInfo();
  //
  //cout << "read image header with " << readptr - data << " bytes" << endl;
}


// print info about a received image
//  in
//   info:   the last read image info struct
void ExternalImageInfo::displayImageInfo() {
  char myID[5];
  memcpy (myID, chID, 4);
  myID[4] = '\0';
  cout << "-----------------------------" << endl;
  cout << "Dumping ExternalImageInfo         " << endl;
  cout << "Data:"  << sizeof (ExternalImageInfo) << " Bytes used" << endl;
  cout << "-----------------------------" << endl;
  cout << "ID -->" << myID << "<-- Version" << iVersion << endl;
  cout << "nlin / ncol / FOVread / FOVphase  = " << nLin 
       << " / " << nCol << " / " << dFOVread << " / " << dFOVphase << endl;
  cout << "Slice Thickness                   = " << dThick << endl;
  cout << "Slice Position Sag / Cor / Tra    = " << dPosSag
       << " / " << dPosCor << " / " << dPosTra << endl;
  cout << "Slice Normal   Sag / Cor / Tra    = " << dNorSag
       << " / " << dNorCor << " / " << dNorTra << endl;
  cout << "Slice Phase En Sag / Cor / Tra    = " << dPhaSag
       << " / " << dPhaCor << " / " << dPhaTra << endl;
  cout << "Slice Row Vect Sag / Cor / Tra    = " << dRowSag
       << " / " << dRowCor << " / " << dRowTra << endl;
  cout << "Slice Col Vect Sag / Cor / Tra    = " << dColSag
       << " / " << dColCor << " / " << dColTra << endl;
  cout << "isMoCo                            = " << bIsMoCo << endl;
  cout << "numMosaicImages / mosaicGridSize  = " << iNoOfImagesInMosaic
       << " / " << iMosaicGridSize << endl;
  cout << "seriesInstanceUID                 = " << cSeriesInstanceUID << endl;
  cout << "iAcqNo / TimeAfterStart / TrigTi  = " << iAcquisitionNumber
       << " / " << dTimeAfterStart << " / " << dTriggerTime << endl;
  cout << "TE / TR / TI                      = " << dTE << " / " << dTR
       << " / " << dTI << endl;
  cout << "AcquisitionDate                   = " << chAcquisitionDate << endl;
  cout << "AcquisitionTime                   = " << chAcquisitionTime << endl;
  cout << "Number of pixels / dataLength     = " << iNumberOfPixels
       << " / " << iImageDataLength << endl;
  cout << "Bytes per pixel                   = " << iBytesPerPixel << endl;
  cout << "Current Slice Index               = " << iSliceIndex <<  endl;
  cout << "Window Width                      = " << iWindowWidth << endl;
  cout << "Window Center                     = " << iWindowCenter << endl;
  cout << "Trigger Time                      = " << dTriggerTime << endl;
  cout << "ImageTypeValue4[0]                = " << chImageTypeValue4[0] << endl;
  cout << "AbsTablePosition                  = " << iAbsTablePosition << endl;
  cout << "Frame Of Reference                = " << chframeOfReference << endl;
  cout << "Time Delay                        = " << dTimeDelay << endl;
  cout << endl;
}



