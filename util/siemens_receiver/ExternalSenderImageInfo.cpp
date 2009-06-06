/******************************************************************************
 * implementation of member functions to receive and send siemens
 * external sender data
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14
 *****************************************************************************/

#include"ExternalSenderImageInfo.h"

// default constructor
ExternalImageInfo::ExternalImageInfo()
  :
  lVersion(0),
  iSizeOfExternalImageInfo(EXTERNALSENDERSIZEOF),
  dFOVread(200),
  dFOVphase(200),
  dThick(5),
  nCol(256),
  nLin(256),
  lImageDataLength(294912),
  lNumberOfPixels(147456),
  lBytesPerPixel(2),
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
  lSliceIndex(0),
  lNumTrackCha(0),
  lCurrentTrackCha(0),
  iWindowCenter(737),
  iWindowWidth(1694),
  lAbsTablePosition(0),
  iDataSource(0),
  dTimeDelay(0)
{
  strcpy((char*) chID, "IFEI");

  chAcquisitionDate[0] = '\0';
  strcpy(chAcquisitionTime,"123456.789101");

  chImageTypeValue4[0][0] = '\0';

  strcpy(chframeOfReference,"123456.789101");


  lReserved[0] = 0;
  dReserved[0] = 0;
  chReserved[0] = '\0';
  cSeriesInstanceUID[0] = '\0';
}

// constructor for data 
ExternalImageInfo::ExternalImageInfo(char *data) {
  memcpy(this,data,sizeof(ExternalImageInfo));
}

// constructor for data from physical scanner
// this function does selective memcpy through the passed data because the 
// scanner sends packed data with a few trash spots, which were identified 
// manually with plenty of frustration. 
//  in
//   data: byte data to read the structure from
//   len:  number of bytes we have
ExternalImageInfo::ExternalImageInfo(char *data, unsigned int len) {
  char trash[TRASHSIZE];
  char *readptr = data;

// DEBUGGING
//  ofstream of("/tmp/dat.hdr");
//  of.write(data,len);

  // check that we have enough data to read iSizeOfExternalImageInfo
  if(len < 4*CHARSIZE + LONGSIZE + INTSIZE) {
    return;
  }

  memcpy(chID, readptr, 4*CHARSIZE);
  readptr += 4*CHARSIZE;
  
  memcpy(&lVersion, readptr, LONGSIZE);
  readptr += LONGSIZE;

  memcpy(&iSizeOfExternalImageInfo, readptr, INTSIZE);
  readptr += INTSIZE;

  // test that we have enough data
  if(len < (unsigned int) iSizeOfExternalImageInfo) {
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

  memcpy(&lImageDataLength, readptr, LONGSIZE);
  readptr += LONGSIZE;

  memcpy(&lNumberOfPixels, readptr, LONGSIZE);
  readptr += LONGSIZE;

  memcpy(&lBytesPerPixel, readptr, LONGSIZE);
  readptr += LONGSIZE;

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
  
  memcpy(&lSliceIndex, readptr, LONGSIZE);
  readptr += LONGSIZE;
  
  memcpy(&lNumTrackCha, readptr, LONGSIZE);
  readptr += LONGSIZE;
  
  memcpy(&lCurrentTrackCha, readptr, LONGSIZE);
  readptr += LONGSIZE;

  memcpy(trash, readptr, 3);
  readptr += 3;


  memcpy(&iWindowCenter, readptr, INTSIZE);
  readptr += INTSIZE;

  memcpy(&iWindowWidth, readptr, INTSIZE);
  readptr += INTSIZE;

  memcpy(chImageTypeValue4, readptr, _NO_OF_V4_ENTRIES*IMAGETYPE_V4_LEN*CHARSIZE);
  readptr += _NO_OF_V4_ENTRIES*IMAGETYPE_V4_LEN*CHARSIZE;


  memcpy(&lAbsTablePosition, readptr, LONGSIZE);
  readptr += LONGSIZE;

  memcpy(chframeOfReference, readptr, FOR_ARRAY_LEN*CHARSIZE);
  readptr += FOR_ARRAY_LEN*CHARSIZE;

  memcpy(&dTimeDelay, readptr, DOUBLESIZE);
  readptr += DOUBLESIZE;

  memcpy(trash, readptr, 16*LONGSIZE);
  readptr += 16*LONGSIZE;
  
  memcpy(trash, readptr, 16*DOUBLESIZE);
  readptr += 16*DOUBLESIZE;
  
  memcpy(trash, readptr, 32*CHARSIZE);
  readptr += 32*CHARSIZE;
  
}

// build data to send as the physical scanner would
// this function does selective memcpy through the passed data because the 
// scanner sends packed data with a few trash spots, which were identified 
// manually with plenty of frustration. 
//  out
//   array of data that conforms to the ExternalImageSender format
char *ExternalImageInfo::convertToScannerDataArray() {
  char *data = (char*) calloc(EXTERNALSENDERSIZEOF,1);
  char *writeptr = data;
  char *trash = (char*) calloc(TRASHSIZE,1);

  memcpy(writeptr, chID, 4*CHARSIZE);
  writeptr += 4*CHARSIZE;
  
  memcpy(writeptr, &lVersion, LONGSIZE);
  writeptr += LONGSIZE;

  memcpy(writeptr, &iSizeOfExternalImageInfo, INTSIZE);
  writeptr += INTSIZE;

  memcpy(writeptr, trash, INTSIZE);
  writeptr += INTSIZE;

  memcpy(writeptr, &dFOVread, DOUBLESIZE);
  writeptr += DOUBLESIZE;

  memcpy(writeptr, &dFOVphase, DOUBLESIZE);
  writeptr += DOUBLESIZE;

  memcpy(writeptr, &dThick, DOUBLESIZE);
  writeptr += DOUBLESIZE;

  memcpy(writeptr, &nCol, INTSIZE);
  writeptr += INTSIZE;


  memcpy(writeptr, &nLin, INTSIZE);
  writeptr += INTSIZE;

  memcpy(writeptr, &lImageDataLength, LONGSIZE);
  writeptr += LONGSIZE;

  memcpy(writeptr, &lNumberOfPixels, LONGSIZE);
  writeptr += LONGSIZE;

  memcpy(writeptr, &lBytesPerPixel, LONGSIZE);
  writeptr += LONGSIZE;

  memcpy(writeptr, trash, INTSIZE);
  writeptr += INTSIZE;

  memcpy(writeptr, &dPosSag, DOUBLESIZE);
  writeptr += DOUBLESIZE;


  memcpy(writeptr, &dPosCor, DOUBLESIZE);
  writeptr += DOUBLESIZE;


  memcpy(writeptr, &dPosTra, DOUBLESIZE);
  writeptr += DOUBLESIZE;


  memcpy(writeptr, &dNorSag, DOUBLESIZE);
  writeptr += DOUBLESIZE;

  memcpy(writeptr, &dNorCor, DOUBLESIZE);
  writeptr += DOUBLESIZE;

  memcpy(writeptr, &dNorTra, DOUBLESIZE);
  writeptr += DOUBLESIZE;


  memcpy(writeptr, &dPhaSag, DOUBLESIZE);
  writeptr += DOUBLESIZE;

  memcpy(writeptr, &dPhaCor, DOUBLESIZE);
  writeptr += DOUBLESIZE;

  memcpy(writeptr, &dPhaTra, DOUBLESIZE);
  writeptr += DOUBLESIZE;


  memcpy(writeptr, &dRowSag, DOUBLESIZE);
  writeptr += DOUBLESIZE;

  memcpy(writeptr, &dRowCor, DOUBLESIZE);
  writeptr += DOUBLESIZE;

  memcpy(writeptr, &dRowTra, DOUBLESIZE);
  writeptr += DOUBLESIZE;


  memcpy(writeptr, &dColSag, DOUBLESIZE);
  writeptr += DOUBLESIZE;

  memcpy(writeptr, &dColCor, DOUBLESIZE);
  writeptr += DOUBLESIZE;

  memcpy(writeptr, &dColTra, DOUBLESIZE);
  writeptr += DOUBLESIZE;


  memcpy(writeptr, &dMoCoTransX, DOUBLESIZE);
  writeptr += DOUBLESIZE;

  memcpy(writeptr, &dMoCoTransY, DOUBLESIZE);
  writeptr += DOUBLESIZE;

  memcpy(writeptr, &dMoCoTransZ, DOUBLESIZE);
  writeptr += DOUBLESIZE;

  memcpy(writeptr, &dMoCoRotX, DOUBLESIZE);
  writeptr += DOUBLESIZE;

  memcpy(writeptr, &dMoCoRotY, DOUBLESIZE);
  writeptr += DOUBLESIZE;

  memcpy(writeptr, &dMoCoRotZ, DOUBLESIZE);
  writeptr += DOUBLESIZE;



  memcpy(writeptr, &bIsMoCo, sizeof(bool));
  writeptr += sizeof(bool);

  memcpy(writeptr, trash, 3);
  writeptr += 3;

  memcpy(writeptr, &iNoOfImagesInMosaic, INTSIZE);
  writeptr += INTSIZE;

  memcpy(writeptr, &iMosaicGridSize, INTSIZE);
  writeptr += INTSIZE;

  memcpy(writeptr, cSeriesInstanceUID, 65*CHARSIZE);
  writeptr += 65*CHARSIZE;


  memcpy(writeptr, &bSwapReadPhase, sizeof(bool));
  writeptr += sizeof(bool);

  memcpy(writeptr, trash, 2);
  writeptr += 2;

 
  memcpy(writeptr, &iAcquisitionNumber, INTSIZE);
  writeptr += INTSIZE;

  memcpy(writeptr, trash, 4);
  writeptr += 4;

  memcpy(writeptr, &dTimeAfterStart, DOUBLESIZE);
  writeptr += DOUBLESIZE;


  memcpy(writeptr, &dTE, DOUBLESIZE);
  writeptr += DOUBLESIZE;


  memcpy(writeptr, &dTR, DOUBLESIZE);
  writeptr += DOUBLESIZE;


  memcpy(writeptr, &dTI, DOUBLESIZE);
  writeptr += DOUBLESIZE;


  memcpy(writeptr, &dTriggerTime, DOUBLESIZE);
  writeptr += DOUBLESIZE;

  memcpy(writeptr, chAcquisitionDate, ACQUISITION_DATE_LEN*CHARSIZE);
  writeptr += ACQUISITION_DATE_LEN*CHARSIZE;
  
  memcpy(writeptr, chAcquisitionTime, ACQUISITION_TIME_LEN*CHARSIZE);
  writeptr += ACQUISITION_TIME_LEN*CHARSIZE;
  
  memcpy(writeptr, &lSliceIndex, LONGSIZE);
  writeptr += LONGSIZE;
  
  memcpy(writeptr, &lNumTrackCha, LONGSIZE);
  writeptr += LONGSIZE;
  
  memcpy(writeptr, &lCurrentTrackCha, LONGSIZE);
  writeptr += LONGSIZE;

  memcpy(writeptr, trash, 3);
  writeptr += 3;


  memcpy(writeptr, &iWindowCenter, INTSIZE);
  writeptr += INTSIZE;

  memcpy(writeptr, &iWindowWidth, INTSIZE);
  writeptr += INTSIZE;

  memcpy(writeptr, chImageTypeValue4, _NO_OF_V4_ENTRIES*IMAGETYPE_V4_LEN*CHARSIZE);
  writeptr += _NO_OF_V4_ENTRIES*IMAGETYPE_V4_LEN*CHARSIZE;


  memcpy(writeptr, &lAbsTablePosition, LONGSIZE);
  writeptr += LONGSIZE;

  memcpy(writeptr, chframeOfReference, FOR_ARRAY_LEN*CHARSIZE);
  writeptr += FOR_ARRAY_LEN*CHARSIZE;

  memcpy(writeptr, &dTimeDelay, DOUBLESIZE);
  writeptr += DOUBLESIZE;

  memcpy(writeptr, trash, 16*LONGSIZE);
  writeptr += 16*LONGSIZE;
  
  memcpy(writeptr, trash, 16*DOUBLESIZE);
  writeptr += 16*DOUBLESIZE;
  
  memcpy(writeptr, trash, 32*CHARSIZE);
  writeptr += 32*CHARSIZE;
  
  free(trash);
  return data;
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
  cout << "ID -->" << myID << "<-- Version" << lVersion << endl;
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
  cout << "Number of pixels / dataLength     = " << lNumberOfPixels
       << " / " << lImageDataLength << endl;
  cout << "Bytes per pixel                   = " << lBytesPerPixel << endl;
  cout << "Current Slice Index               = " << lSliceIndex <<  endl;
  cout << "Window Width                      = " << iWindowWidth << endl;
  cout << "Window Center                     = " << iWindowCenter << endl;
  cout << "Trigger Time                      = " << dTriggerTime << endl;
  cout << "ImageTypeValue4[0]                = " << chImageTypeValue4[0] << endl;
  cout << "AbsTablePosition                  = " << lAbsTablePosition << endl;
  cout << "Data Source                       = " << iDataSource << endl;
  cout << "Frame Of Reference                = " << chframeOfReference << endl;
  cout << "Time Delay                        = " << dTimeDelay << endl;
  cout << endl;
}



