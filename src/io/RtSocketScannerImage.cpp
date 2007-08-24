/******************************************************************************
 * RtSocketScannerImage.cpp defines a class that implements scanner
 * image reading from a socket
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-22
 *
 *****************************************************************************/

static char *VERSION = "$Id$";


#include"RtSocketScannerImage.h"

// construct and start listening
RtSocketScannerImage::RtSocketScannerImage(SocketService *ss, TCPSocket &ts)
  : RtSocketListener(ss, ts) {
}

// clean up
RtSocketScannerImage::~RtSocketScannerImage() {
}

// handle close
void RtSocketScannerImage::onClose() {
  cerr << "scanner image socket closed!" << endl;
}

// read some data
void RtSocketScannerImage::dataReceived(char *data, 
						unsigned int count) {
  
  cout << "read " << count << " bytes of data:" << endl;

  
  
}

// extract an image info struct from the received data
// checks for errors
//  out: pointer to the image info struct received
ExternalImageInfo *RtSocketScannerImage::extractImageInfo(char *data, 
							  unsigned int count) {
  
  // check for the correct size
  if(count != sizeof(ExternalImageInfo)) {
    cerr << "ERROR: data is the wrong size to read ExternalImageInfo." << endl;
    return NULL;
  }
  
  struct ExternalImageInfo* info = new ExternalImageInfo(data);
  
  // print out some important data
  dumpImageInfo(cout,*info);
		
  return info;
}

// extracts an image 
RtDataImage *RtSocketScannerImage::extractImage(ExternalImageInfo &info,
						char *data, 
						unsigned int count) {

  unsigned int imageDataSize = info.nCol * info.nLin * sizeof(short);

  // check for the correct size
  if(count != imageDataSize) {
    cerr << "ERROR: data size does not match image info." << endl;
    return NULL;
  }
  
  cout << "ReceiveImage():: Image Data and image received..." << endl;
  
  RtDataImage *img = new RtDataImage(info, data);
  
  return img;
}

// print out information about the image
void RtSocketScannerImage::dumpImageInfo(ostream &os,ExternalImageInfo &info) {
  char myID[5]; 
  memcpy (myID, info.chID, 4);
  myID[4] = '\0';
  os << "-----------------------------" << endl;
  os << "Dumping ExternalImageInfo         " << endl;
  os << "Data:"  << sizeof (ExternalImageInfo) << " Bytes used" << endl;
  //os << "Patient name    :"  << info.chPatientName << endl;
  //os << "Patient position:"  << info.chPatientPosition << endl;
  os << "-----------------------------" << endl;
  //os << "Sequence name : " << info.cSequenceName << endl;
  os << "ID -->" << myID << "<-- Version" << info.lVersion << endl;
  os << "nlin / ncol / FOVread / FOVphase  = " << info.nLin << " / " << info.nCol << " / " << info.dFOVread << " / " << info.dFOVphase << endl;
  os << "Slice Thickness                   = " << info.dThick << endl;
  os << "Slice Position Sag / Cor / Tra    = " << info.dPosSag << " / " << info.dPosCor << " / " << info.dPosTra << endl;
  os << "Slice Normal   Sag / Cor / Tra    = " << info.dNorSag << " / " << info.dNorCor << " / " << info.dNorTra << endl;
  os << "Slice Phase En Sag / Cor / Tra    = " << info.dPhaSag << " / " << info.dPhaCor << " / " << info.dPhaTra << endl;
  os << "Slice Row Vect Sag / Cor / Tra    = " << info.dRowSag << " / " << info.dRowCor << " / " << info.dRowTra << endl;
  os << "Slice Col Vect Sag / Cor / Tra    = " << info.dColSag << " / " << info.dColCor << " / " << info.dColTra << endl;
  os << "iAcqNo / TimeAfterStart / TrigTi  = " << info.iAcquisitionNumber << " / " << info.dTimeAfterStart << " / " << info.dTriggerTime << endl;
  os << "TE / TR / TI                      = " << info.dTE << " / " << info.dTR << " / " << info.dTI << endl;
  os << "AcquisitionDate                   = " << info.chAcquisitionDate << endl;
  os << "AcquisitionTime                   = " << info.chAcquisitionTime << endl;
  os << "Number of pixels / dataLength     = " << info.lNumberOfPixels   << " / " << info.lImageDataLength << endl;
  os << "Bytes per pixel                   = " << info.lBytesPerPixel << endl;
  os << "Current Slice Index               = " << info.lSliceIndex <<  endl;
  os << "Window Width                      = " << info.iWindowWidth << endl;   
  os << "Window Center                     = " << info.iWindowCenter << endl;  
  os << "Trigger Time                      = " << info.dTriggerTime << endl;   
  os << "ImageTypeValue4[0]                = " << info.chImageTypeValue4[0] << endl; 
  os << "AbsTablePosition                  = " << info.lAbsTablePosition << endl;
  os << "Data Source                       = " << info.iDataSource << endl;
  os << "Frame Of Reference                = " << info.chframeOfReference << endl;   
  os << "Time Delay                        = " << info.dTimeDelay << endl;
  /*    
	for  (int i = 0; i < (sizeof(m_myInfo.lReserved)/sizeof(long)); i++)
	{
	os << "lReserved				= " << info.lReserved[i] << endl;
	}

	for  (i = 0; i < (sizeof(m_myInfo.dReserved)/sizeof(double)); i++)
	{
	os << "dReserved				= " << info.dReserved[i] << endl;
	}

	for  (i = 0; i < (sizeof(m_myInfo.chReserved)/sizeof(char)); i++)
	{
	os << "chReserved				= " << info.chReserved[i] << endl;
	}
  */
  // os << "Number of Tracking Channels/current= " << info.lNumTrackCha << " / " << info.lCurrentTrackCha << endl;
  //os << "Current Slice Index / # of slices = " << info.lSliceIndex << " / " << info.lNumberOfSlices << endl;
  // os << "Current Repet Index / # of reps   = " << info.lMeasurementIndex << " / " << info.lNumberOfMeasurements << endl;
  //os << "lReserved                         = " << info.lReserved << endl;
  /*#ifdef TMAP
    os << "-------------- TMapping features -----------------------\n";
    os << "Image type:                         "<< info.ThisKindOfImage.eImageType 
    << "Image context: "<< info.ThisKindOfImage.eImageContext 
    << "Mosaic image: " << info.ThisKindOfImage.eIsThisAMosaicImage << endl;
    os << "Image temperatures:               = "<< info.ThisTempRange.dT_Min << "   to   "<< info.ThisTempRange.dT_Max<< endl;
 
    os << " Windowing: width/center/level/slope/intercept: "<<  info.WindowingOfThisImage.iWindowWidth << " / " 
    <<  info.WindowingOfThisImage.iWindowCenter << " / " <<  info.WindowingOfThisImage.dWindowLevel << " / " 
    <<  info.WindowingOfThisImage.dWindowSlope  << " / " <<  info.WindowingOfThisImage.dWindowIntercept << endl; 
    #endif
  */
  /*os << "Device Coordinate System:"<< endl;
    os << "Slice Position X / Y / Z    ;		   " << info.dPosX << " / " << info.dPosY << " / " << info.dPosZ << endl;                                 
    os << "Slice Normal   X / Y / Z    ;		   " << info.dNorX << " / " << info.dNorY << " / " << info.dNorZ << endl;                                 
    os << "Slice Phase En X / Y / Z    ;		   " << info.dPhaX << " / " << info.dPhaY << " / " << info.dPhaZ << endl;                                 
    os << "Slice Row Vect X / Y / Z    ;		   " << info.dRowX << " / " << info.dRowY << " / " << info.dRowZ << endl;                                 
    os << "Slice Col Vect X / Y / Z    ;		   " << info.dColX << " / " << info.dColY << " / " << info.dColZ << endl;                                 
  */
};


// gets the version
//  out:
//   cvs version string for this class
char *RtSocketScannerImage::getVersionString() {
  return VERSION;
}

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/

