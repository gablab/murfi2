/******************************************************************************
 * RtInputScannerImages.cpp defines a class that implements scanner
 * image communication operations
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-14
 *
 *****************************************************************************/
static char *VERSION = "$Id$";

#include"RtInputScannerImages.h"

static const int             DEFAULT_PORT = 15000;
static const char*           DEFAULT_HOST = "localhost";

#define MAXBUFSIZ 256*256*256*2
static char buffer[MAXBUFSIZ];

// default constructor
RtInputScannerImages::RtInputScannerImages()
                 : host(DEFAULT_HOST), port(DEFAULT_PORT) {

}

// destructor
RtInputScannerImages::~RtInputScannerImages() {
}

// configure and open
bool RtInputScannerImages::open(RtConfig &config) {
  RtInput::open(config);

  // validate
  if(!validateHostAndPort(config)) {
    cerr << "ERROR: host " << config.get("scannerHost")
	 << ":" << config.get("scannerPort") << " is invalid." << endl;
    isOpen = false;
    return false;
  }

  // get the host and port from the config
  // try to open the socket
  port = config.get("scannerPort");
  host = (char*) config.get("scannerHost");

  // build the address
  address.set(port,host.c_str());

  if(connector.connect(stream,address) == -1) {
    cerr << "ERROR: could not open connection to receive "
	 << "images from the scanner." << endl
	 << "using host " << host << " and port " << port << endl;
    isOpen = false;
    return false;
  }

  return true;
}

// close and clean up
bool RtInputScannerImages::close() {

  if(isOpen) {
    //    socketService->stopServer();
  }

  RtInput::close();

  return true;
}

// run the scanner input
bool RtInputScannerImages::run() {
  RtExternalImageInfo *ei;
  unsigned short *img;

  for (int i=0;; i++) {

    ei = receiveImageInfo(stream);
    displayImageInfo(*ei);

    img = receiveImage(stream, *ei);
    cout << "read an image" << endl;

    cout << "a few values: " <<  img[1]  << " " << img[2] << " " << img[3]
	 << " " << img[4] << endl;

    delete ei;
    delete [] img;
  }

  return true;
}


// read the scanner image info from a socket stream
// NOTE: performes blocking read
//  in
//   stream: a socket stream to receive on
//  out
//   image info struct on successful read (NULL otherwise)
RtExternalImageInfo *RtInputScannerImages::receiveImageInfo(ACE_SOCK_Stream &stream) {

  // read until we have all the bytes we need
  // ADD ERROR HANDLING HERE!!!
  for(unsigned int rec = 0; rec < sizeof(RtExternalImageInfo);
    rec += stream.recv_n (buffer+rec, sizeof(RtExternalImageInfo)-rec));

  struct RtExternalImageInfo* info = new RtExternalImageInfo(buffer);

  return info;
}

// read an image info from a socket stream
// NOTE: performes blocking read
//  in
//   stream: a socket stream to receive on
//   info:   the last read image info struct
//  out
//   image data on successful read (NULL otherwise)
unsigned short *RtInputScannerImages::receiveImage(ACE_SOCK_Stream &stream,
						   const RtExternalImageInfo &info) {

  int numPix = info.nLin * info.nCol;
  for(unsigned int rec = 0; rec < numPix*sizeof(unsigned short);
    rec += stream.recv_n (buffer+rec, numPix*sizeof(unsigned short)-rec));

  unsigned short *img = new unsigned short[numPix];
  memcpy(img,buffer,numPix*sizeof(unsigned short));

  return img;
};

// print info about a received image
//  in
//   info:   the last read image info struct
void RtInputScannerImages::displayImageInfo(const RtExternalImageInfo &info) {
    char myID[5];
    memcpy (myID, info.chID, 4);
    myID[4] = '\0';
    cout << "-----------------------------" << endl;
    cout << "Dumping RtExternalImageInfo         " << endl;
    cout << "Data:"  << sizeof (RtExternalImageInfo) << " Bytes used" << endl;
    //cout << "Patient name    :"  << info.chPatientName << endl;
    //cout << "Patient position:"  << info.chPatientPosition << endl;
    cout << "-----------------------------" << endl;
	//cout << "Sequence name : " << info.cSequenceName << endl;
    cout << "ID -->" << myID << "<-- Version" << info.lVersion << endl;
    cout << "nlin / ncol / FOVread / FOVphase  = " << info.nLin << " / " << info.nCol << " / " << info.dFOVread << " / " << info.dFOVphase << endl;
    cout << "Slice Thickness                   = " << info.dThick << endl;
    cout << "Slice Position Sag / Cor / Tra    = " << info.dPosSag << " / " << info.dPosCor << " / " << info.dPosTra << endl;
    cout << "Slice Normal   Sag / Cor / Tra    = " << info.dNorSag << " / " << info.dNorCor << " / " << info.dNorTra << endl;
    cout << "Slice Phase En Sag / Cor / Tra    = " << info.dPhaSag << " / " << info.dPhaCor << " / " << info.dPhaTra << endl;
    cout << "Slice Row Vect Sag / Cor / Tra    = " << info.dRowSag << " / " << info.dRowCor << " / " << info.dRowTra << endl;
    cout << "Slice Col Vect Sag / Cor / Tra    = " << info.dColSag << " / " << info.dColCor << " / " << info.dColTra << endl;
    cout << "iAcqNo / TimeAfterStart / TrigTi  = " << info.iAcquisitionNumber << " / " << info.dTimeAfterStart << " / " << info.dTriggerTime << endl;
    cout << "TE / TR / TI                      = " << info.dTE << " / " << info.dTR << " / " << info.dTI << endl;
    cout << "AcquisitionDate                   = " << info.chAcquisitionDate << endl;
    cout << "AcquisitionTime                   = " << info.chAcquisitionTime << endl;
    cout << "Number of pixels / dataLength     = " << info.lNumberOfPixels   << " / " << info.lImageDataLength << endl;
    cout << "Bytes per pixel                   = " << info.lBytesPerPixel << endl;
    cout << "Current Slice Index               = " << info.lSliceIndex <<  endl;
    cout << "Window Width                      = " << info.iWindowWidth << endl;
    cout << "Window Center                     = " << info.iWindowCenter << endl;
    cout << "Trigger Time                      = " << info.dTriggerTime << endl;
    cout << "ImageTypeValue4[0]                = " << info.chImageTypeValue4[0] << endl;
    cout << "AbsTablePosition                  = " << info.lAbsTablePosition << endl;
    cout << "Data Source                       = " << info.iDataSource << endl;
    cout << "Frame Of Reference                = " << info.chframeOfReference << endl;
    cout << "Time Delay                        = " << info.dTimeDelay << endl;
/*
    for  (int i = 0; i < (sizeof(m_myInfo.lReserved)/sizeof(long)); i++)
    {
        cout << "lReserved				= " << info.lReserved[i] << endl;
    }

    for  (i = 0; i < (sizeof(m_myInfo.dReserved)/sizeof(double)); i++)
    {
        cout << "dReserved				= " << info.dReserved[i] << endl;
    }

    for  (i = 0; i < (sizeof(m_myInfo.chReserved)/sizeof(char)); i++)
    {
        cout << "chReserved				= " << info.chReserved[i] << endl;
    }
*/
	// cout << "Number of Tracking Channels/current= " << info.lNumTrackCha << " / " << info.lCurrentTrackCha << endl;
    //cout << "Current Slice Index / # of slices = " << info.lSliceIndex << " / " << info.lNumberOfSlices << endl;
   // cout << "Current Repet Index / # of reps   = " << info.lMeasurementIndex << " / " << info.lNumberOfMeasurements << endl;
    //cout << "lReserved                         = " << info.lReserved << endl;
/*#ifdef TMAP
	cout << "-------------- TMapping features -----------------------\n";
	cout << "Image type:                         "<< info.ThisKindOfImage.eImageType
		 << "Image context: "<< info.ThisKindOfImage.eImageContext
		 << "Mosaic image: " << info.ThisKindOfImage.eIsThisAMosaicImage << endl;
	cout << "Image temperatures:               = "<< info.ThisTempRange.dT_Min << "   to   "<< info.ThisTempRange.dT_Max<< endl;

    cout << " Windowing: width/center/level/slope/intercept: "<<  info.WindowingOfThisImage.iWindowWidth << " / "
		 <<  info.WindowingOfThisImage.iWindowCenter << " / " <<  info.WindowingOfThisImage.dWindowLevel << " / "
		 <<  info.WindowingOfThisImage.dWindowSlope  << " / " <<  info.WindowingOfThisImage.dWindowIntercept << endl;
#endif
		 */
	/*cout << "Device Coordinate System:"<< endl;
	cout << "Slice Position X / Y / Z    ;		   " << info.dPosX << " / " << info.dPosY << " / " << info.dPosZ << endl;
	cout << "Slice Normal   X / Y / Z    ;		   " << info.dNorX << " / " << info.dNorY << " / " << info.dNorZ << endl;
	cout << "Slice Phase En X / Y / Z    ;		   " << info.dPhaX << " / " << info.dPhaY << " / " << info.dPhaZ << endl;
	cout << "Slice Row Vect X / Y / Z    ;		   " << info.dRowX << " / " << info.dRowY << " / " << info.dRowZ << endl;
	cout << "Slice Col Vect X / Y / Z    ;		   " << info.dColX << " / " << info.dColY << " / " << info.dColZ << endl;
	*/
};

// write a received image to a file
//  in
//   img:  pixel data
//   info: info struct for the image
//  out
//   success or failure
bool RtInputScannerImages::writeImage(unsigned short *img,
				      const RtExternalImageInfo &info) {

  char imgNum[6] = "";
  sprintf(imgNum, "%06d", info.iAcquisitionNumber);
  char *suffix = ".dat";

  const size_t pix = 2*info.nLin*info.nCol;

  string fname = saveFilestem + imgNum + suffix;

  FILE *imgFile = fopen(fname.c_str(),"wb");
  fwrite(img, pix, 1, imgFile);
  fclose(imgFile);

  return fclose(imgFile);
}


// validate the passed host and port
bool RtInputScannerImages::validateHostAndPort(RtConfig &config) {

  // host
  if(inet_addr((char*) config.get("scannerHost")) < 1) {
    return false;
  }

  // port
//  if(port < 0 || port > 65535) {
//    return false;
//  }

  return true;
}


// gets the version
//  out:
//   cvs version string for this class
char *RtInputScannerImages::getVersionString() {
  return VERSION;
}


//  //*** private data members  ***//
//
//  string tcpPort;
//  bool   saveImagesToFile;
//  string saveDirectory;
//  string saveFilestem;

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


