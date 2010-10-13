/******************************************************************************
 * RtDisplayImage.cpp defines a class for displaying single images
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-29
 *
 *****************************************************************************/

#ifdef USE_OLDFRONTEND

#include"RtDisplayImage.h"
#include"RtDataIDs.h"
#include"RtExperiment.h"

#include<vnl/vnl_vector.h>
#include "simple_ls.h"
#include"gnuplot_i.hpp"

#include"ace/Mutex.h"

// scopic alexsid: #define WIN32
#ifndef RT_DISPLAY_IMAGE_TEXTURE
  #ifdef WIN32 
    #define RT_DISPLAY_IMAGE_TEXTURE GL_TEXTURE_2D
  #else
     #ifdef GL_TEXTURE_RECTANGLE_EXT
       #ifndef GL_TEXTURE_RECTANGLE_NV
         #define GL_TEXTURE_RECTANGLE_NV GL_TEXTURE_RECTANGLE_EXT
       #endif
     #else
       #ifdef GL_TEXTURE_RECTANGLE_NV
         #ifndef GL_TEXTURE_RECTANGLE_EXT
           #define GL_TEXTURE_RECTANGLE_EXT GL_TEXTURE_RECTANGLE_NV
         #endif
     //#else
     //#define GL_TEXTURE_RECTANGLE_NV GL_TEXTURE_2D
     //#define GL_TEXTURE_2D GL_TEXTURE_2D
       #endif
     #endif
   #define RT_DISPLAY_IMAGE_TEXTURE GL_TEXTURE_RECTANGLE_EXT
  #endif
#endif

// debuging
#define DISPLAY_THREAD_HACK

//#ifdef GL_TEXTURE_2D
//#ifndef GL_TEXTURE_RECTANGLE_NV
//#define GL_TEXTURE_RECTANGLE_NV GL_TEXTURE_2D
//#endif
//#else
//#ifdef GL_TEXTURE_RECTANGLE_NV
//#ifndef GL_TEXTURE_2D
//#define GL_TEXTURE_2D GL_TEXTURE_RECTANGLE_NV
//#endif
////#else
////#define GL_TEXTURE_RECTANGLE_NV GL_TEXTURE_2D
////#define GL_TEXTURE_2D GL_TEXTURE_2D
//#endif
//#endif

// defaults
#define DEFAULT_X 400
#define DEFAULT_Y 0
#define DEFAULT_W 600
#define DEFAULT_H 600
#define DEFAULT_NUMMEAS 600
#define DEFAULT_TITLE "murfi"
#define DEFAULT_POSOVERLAYID "data.image.activation.voxel-singleimcor"
#define DEFAULT_POSOVERLAYROIID "unset"
#define DEFAULT_POSMASKID "data.image.mask"
#define DEFAULT_POSMASKROIID "unset"
#define DEFAULT_NEGOVERLAYID "data.image.activation.voxel-singleimcor"
#define DEFAULT_NEGOVERLAYROIID "unset"
#define DEFAULT_NEGMASKID "data.image.mask"
#define DEFAULT_NEGMASKROIID "unset"
#define DEFAULT_POSACTIVATIONSUMID "data.image.activation.voxel-singleimcor.activation-sum"
#define DEFAULT_POSACTIVATIONSUMROIID "unset"
#define DEFAULT_NEGACTIVATIONSUMID "data.image.activation.voxel-singleimcor.activation-sum"
#define DEFAULT_NEGACTIVATIONSUMROIID "unset"


#define max(a,b) ((a >= b) ? a : b)
#define min(a,b) ((a <= b) ? a : b)

//*** constructors/destructors  ***//

// default constructor
RtDisplayImage::RtDisplayImage() {
  initialized = false;

  x = DEFAULT_X; 
  y = DEFAULT_Y; 
  width = DEFAULT_W; 
  height = DEFAULT_H;

  img = NULL; 
  imageTex = 0; 
  imageDisplayType = ID_MOSAIC;

  loadInitialImage = false;
  initialImageFilename = "";

  posOverlayTex = 0; 
  negOverlayTex = 0; 
  threshold = 4.0;
  thresholdModAmount = 1.2;

  posMaskTex = 0; 
  negMaskTex = 0; 
  needsRepaint = true; 

  newTex = false;
  newPosOverlay = false; 
  newNegOverlay = false; 
  newPosMask = false; 
  newNegMask = false; 
  newImageType = true; 

  posOverlayOn = false;
  negOverlayOn = false;


  posMaskOn = false;
  loadInitialPosMask = false;
  initialPosMaskFilename = "";
  flipInitialPosMask = false;

  negMaskOn = false;
  loadInitialNegMask = false;
  initialNegMaskFilename = "";
  flipInitialNegMask = false;

  gnuPlot = new Gnuplot("lines");

  posOverlayID = DEFAULT_POSOVERLAYID;
  posOverlayRoiID = DEFAULT_POSOVERLAYROIID;
  negOverlayID = DEFAULT_NEGOVERLAYID;
  negOverlayRoiID = DEFAULT_NEGOVERLAYROIID;
  posMaskID = DEFAULT_POSMASKID;
  posMaskRoiID = DEFAULT_POSMASKROIID;
  negMaskID = DEFAULT_NEGMASKID;
  negMaskRoiID = DEFAULT_NEGMASKROIID;
  posActivationSumID = DEFAULT_POSACTIVATIONSUMID;
  posActivationSumID = DEFAULT_POSACTIVATIONSUMROIID;
  negActivationSumID = DEFAULT_NEGACTIVATIONSUMID;
  negActivationSumID = DEFAULT_NEGACTIVATIONSUMROIID;

  // movie stuff
  makeMovie = false;
  framePrefix = "/tmp/real_frame_";
  curFrame = 0;
  screenShotCommand = "xwd -screen -root ";

}

// constructor with stuff
//  in
//   _x: initial x position of the window
//   _y: initial y position of the window
//   _w: initial width of the window
//   _h: initial height of the window
//   _title: title string of the window
RtDisplayImage::RtDisplayImage(int _x, int _y,
			       int _w, int _h,
			       char *_title) {
  initialized = false;

  x = _x;
  y = _y;
  width = _w;
  height = _h;

  img = NULL;
  imageTex = 0;
  posOverlayTex = 0;
  negOverlayTex = 0;
  posMaskTex = 0;
  negMaskTex = 0;
  needsRepaint = true;

  newTex = false;
  newPosOverlay = false;
  newNegOverlay = false;
  newPosMask = false; 
  newNegMask = false; 
  newImageType = true;
  posOverlayOn = false;
  negOverlayOn = false;
  posMaskOn = false;
  negMaskOn = false;

  imageDisplayType = ID_MOSAIC;

  posOverlayID = DEFAULT_POSOVERLAYID;
  posOverlayRoiID = DEFAULT_POSOVERLAYROIID;
  negOverlayID = DEFAULT_NEGOVERLAYID;
  negOverlayRoiID = DEFAULT_NEGOVERLAYROIID;
  posMaskID = DEFAULT_POSMASKID;
  posMaskRoiID = DEFAULT_POSMASKROIID;
  negMaskID = DEFAULT_NEGMASKID;
  negMaskRoiID = DEFAULT_NEGMASKROIID;
  posActivationSumID = DEFAULT_POSACTIVATIONSUMID;
  posActivationSumID = DEFAULT_POSACTIVATIONSUMROIID;
  negActivationSumID = DEFAULT_NEGACTIVATIONSUMID;
  negActivationSumID = DEFAULT_NEGACTIVATIONSUMROIID;

  // movie stuff
  makeMovie = false;
  framePrefix = "/tmp/real_frame_";
  curFrame = 0;
  screenShotCommand = "xwd -screen -root ";

  strcpy(title,_title);

  delete gnuPlot;
}

// destructor
RtDisplayImage::~RtDisplayImage() {
  glutDestroyWindow(windowID);
}

// 

// initialize the display
//  in
//   config: config class to get configuration variables from
bool RtDisplayImage::open(RtConfig &config) {
  ACE_TRACE(("RtDisplayImage::open"));

  x = config.isSet("oldgui:winX")
    ? config.get("oldgui:winX") : DEFAULT_X;

  y = config.isSet("oldgui:winY")
    ? config.get("oldgui:winY") : DEFAULT_Y;

  width = config.isSet("oldgui:winW")
    ? config.get("oldgui:winW") : DEFAULT_W;

  height = config.isSet("oldgui:winH")
    ? config.get("oldgui:winH") : DEFAULT_H;

  strcpy(title, config.isSet("oldgui:winTitle")
	 ? config.get("oldgui:winTitle").str().c_str() : DEFAULT_TITLE);


  return init();
}

// initialize the display
//  in
//   _x: initial x position of the window
//   _y: initial y position of the window
//   _w: initial width of the window
//   _h: initial height of the window
//   _title: title string of the window
bool RtDisplayImage::init(int _x, int _y, int _w, int _h, char *_title) {
  ACE_TRACE(("RtDisplayImage::init(..)"));

  x = _x;
  y = _y;
  width = _w;
  height = _h;
  strcpy(title,_title);

  return init();
}

// initialize the display
bool RtDisplayImage::init() {
  ACE_TRACE(("RtDisplayImage::init()"));

  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE);
  glutInitWindowSize(width, height);
  glutInitWindowPosition(x, y);

  glutMaster.CallGlutCreateWindow(title, this);
  glViewport(0, 0, width, height);
  glutMaster.EnableTimerFunction();
  glutMaster.SetTimerToCurrentWindow();
  //glutMaster.SetTimerPeriod(1000);

  // make a menu
  glutAttachMenu(GLUT_RIGHT_BUTTON);

  glutAddMenuEntry("'q' quit",'q');
  glutAddMenuEntry("'r' fmri run", 'r');
  glutAddMenuEntry("'s' show live scanner images", 's');
  glutAddMenuEntry("'d' show difference images", 'd');
  glutAddMenuEntry("'m' show mean image", 'm');
  glutAddMenuEntry("'v' show variance image", 'n');
  glutAddMenuEntry("'n' show intensity norm image", 'n');
  glutAddMenuEntry("'=/+' toggle positive mask visibility", '=');
  glutAddMenuEntry("'-/_' toggle negative mask visibility", '-');
  glutAddMenuEntry("'z' toggle positive overlay visibility", 'z');
  glutAddMenuEntry("'Z' toggle negative overlay visibility", 'Z');

  /* erase color */
  glClearColor(0.0f, 0.0f, 0.0f, 1);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, (double) width, 0.0, (double) height, 1.0, -1.0);

  initialized = true;

  return true;
}

// perpare for a single run
bool RtDisplayImage::prepareRun(RtConfig &config) {

  // configuration

  numMeas = config.isSet("scanner:measurements")
    ? config.get("scanner:measurements") : DEFAULT_NUMMEAS;

  // resize the vectors to take the new number of measurements
  postc.set_size(numMeas);
  postc.fill(0.0);

  negtc.set_size(numMeas);
  negtc.fill(0.0);

  if(config.isSet("oldgui:initialImage")) {
    initialImageFilename = config.get("oldgui:initialImage").str();
    loadInitialImage = true;
  }

  if(config.isSet("oldgui:displayType")) {
    imageDisplayType = config.get("oldgui:displayType").str();
    cout << "set image display type to " << imageDisplayType << endl;
  }

  if(config.isSet("oldgui:initialPosMask")) {
    initialPosMaskFilename = config.get("oldgui:initialPosMask").str();
    loadInitialPosMask = true;
  }
  
  if(config.isSet("oldgui:flipInitialPosMask") 
     && config.get("oldgui:flipInitialPosMask") == true) {  
    flipInitialPosMask = true;
  }

  if(config.isSet("oldgui:initialNegMask")) {
    initialNegMaskFilename = config.get("oldgui:initialNegMask").str();
    loadInitialNegMask = true;
  }

  if(config.isSet("oldgui:flipInitialNegMask") 
     && config.get("oldgui:flipInitialNegMask") == true) {  
    flipInitialNegMask = true;
  }

  if(config.isSet("oldgui:overlayID")) {
    posOverlayID = config.get("oldgui:overlayID").str();
    posOverlayOn = true;
  }

  if(config.isSet("oldgui:posOverlayID")) {
    posOverlayID = config.get("oldgui:posOverlayID").str();
    posOverlayOn = true;
  }

  posOverlayRoiID = config.isSet("oldgui:posOverlayRoiID")
	 ? config.get("oldgui:posOverlayRoiID").str() :DEFAULT_POSOVERLAYROIID;

  if(config.isSet("oldgui:negOverlayID")) {
    negOverlayID = config.get("oldgui:negOverlayID").str();
  }

  negOverlayRoiID = config.isSet("oldgui:negOverlayRoiID")
	 ? config.get("oldgui:negOverlayRoiID").str() :DEFAULT_NEGOVERLAYROIID;

  if(config.isSet("oldgui:posMaskID")) {
    posMaskID = config.get("oldgui:posMaskID").str();
    posMaskOn = true;
  }

  posMaskRoiID = config.isSet("oldgui:posMaskRoiID")
	 ? config.get("oldgui:posMaskRoiID").str() : DEFAULT_POSMASKROIID;

  if(config.isSet("oldgui:negMaskID")) {
    negMaskID = config.get("oldgui:negMaskID").str();
    negMaskOn = true;
  }

  negMaskRoiID = config.isSet("oldgui:negMaskRoiID")
	 ? config.get("oldgui:negMaskRoiID").str() : DEFAULT_NEGMASKROIID;

  posActivationSumID = config.isSet("oldgui:posActivationSumID")
	 ? config.get("oldgui:posActivationSumID").str() 
    : DEFAULT_POSACTIVATIONSUMID;
  posActivationSumRoiID = config.isSet("oldgui:posActivationSumRoiID")
	 ? config.get("oldgui:posActivationSumRoiID").str() 
    : DEFAULT_POSACTIVATIONSUMROIID;

  negActivationSumID = config.isSet("oldgui:negActivationSumID")
	 ? config.get("oldgui:negActivationSumID").str() 
    : DEFAULT_NEGACTIVATIONSUMID;
  negActivationSumRoiID = config.isSet("oldgui:negActivationSumRoiID")
	 ? config.get("oldgui:negActivationSumRoiID").str() 
    : DEFAULT_NEGACTIVATIONSUMROIID;

  // movie stuff
  if(config.isSet("oldgui:makeMovie") 
     && config.get("oldgui:makeMovie") == true) {
    makeMovie = true;
  }
  if(config.isSet("oldgui:framePrefix")) {
    framePrefix = config.get("oldgui:framePrefix").str();
  }
  if(config.isSet("oldgui:screenShotCommand")) {
    screenShotCommand = config.get("oldgui:screenshotCommand").str();
  }

  // display prep

  // delete the old textures
  if(glIsTexture(imageTex)) {
    glDeleteTextures(1, &imageTex);
  }
  if(glIsTexture(posOverlayTex)) {
    glDeleteTextures(1, &posOverlayTex);
  }
  if(glIsTexture(negOverlayTex)) {
    glDeleteTextures(1, &negOverlayTex);
  }
  if(glIsTexture(posMaskTex)) {
    glDeleteTextures(1, &posMaskTex);
  }
  if(glIsTexture(negMaskTex)) {
    glDeleteTextures(1, &negMaskTex);
  }

  // load the initial images
  if(loadInitialImage) {
    img = new RtMRIImage();
    if(!img->read(initialImageFilename)) {
      cout << "couldn't read initial image to display" << endl;
    }
    else { // success
      // unmosaic?
      if(img->isMosaic()) {
	img->mosaic();
      }
      newTex = true;
    }
  }

  if(loadInitialPosMask) {
    posMask = new RtMaskImage();
    if(!posMask->read(initialPosMaskFilename)) {
      cout << "couldn't read initial pos mask to display" << endl;
    }
    else { // success
      // unmosaic?
      if(posMask->isMosaic()) {
	posMask->mosaic();
      }
      if(flipInitialPosMask) {
	posMask->flipLR();
      }
      newPosMask = true;
    }
  }

  if(loadInitialNegMask) {
    negMask = new RtMaskImage();
    if(!negMask->read(initialNegMaskFilename)) {
      cout << "couldn't read initial neg mask to display" << endl;
    }
    else { // success
      // unmosaic?
      if(negMask->isMosaic()) {
	negMask->mosaic();
      }
      if(flipInitialNegMask) {
	negMask->flipLR();
      }
      newNegMask = true;
    }
  }

  needsRepaint = true;

  return true;
}

// thread entry
int RtDisplayImage::svc() {

  ACE_TRACE(("RtDisplayImage::svc"));

  if(!initialized) {
    cerr << "ERROR: display has not been initialized" << endl;
    return 1;
  }

  // register me to get the data
  getDataStore().addListener(this);

  // make the display
  glutMaster.CallGlutMainLoop();

  return 0;
}

// sets the image to be displayed
void RtDisplayImage::notify(const RtDataID &id) {
  ACE_TRACE(("RtDisplayImage::notify"));


  if(DEBUG_LEVEL & BASIC) {
    cout << "got data: " << id << endl;
  }

  // handle activation sum
  if(id.getDataName() == posActivationSumID 
     && id.getRoiID() == posActivationSumRoiID) {

    if(DEBUG_LEVEL & BASIC) {
      cout << "display got a pos activation sum: " 
	   << ((RtActivation*)getDataStore().getData(id))->getPixel(0)
	   << " " << id.getTimePoint() << endl;
    }

    // plot the sum
    if(id.getTimePoint() < postc.size()) {
      postc.put(id.getTimePoint(),
		((RtActivation*)getDataStore().getData(id))->getPixel(0));
    }

    gnuPlot->reset_plot();
    gnuPlot->plot_x(postc.extract(id.getTimePoint(),1),posActivationSumRoiID.c_str());

    return;
  }

  if(id.getDataName() == negActivationSumID 
     && id.getRoiID() == negActivationSumRoiID) {

    if(DEBUG_LEVEL & BASIC) {
      cout << "display got a neg activation sum: " 
	   << ((RtActivation*)getDataStore().getData(id))->getPixel(0) 
	   << endl;
    }

    // plot the sum
    if(id.getTimePoint() < negtc.size()) {
	negtc.put(id.getTimePoint(),
		  ((RtActivation*)getDataStore().getData(id))->getPixel(0));
    }

    //gp.reset_plot();
    gnuPlot->plot_x(negtc.extract(id.getTimePoint(),1),negActivationSumRoiID.c_str());
    //numTimepoints++;
    return;
  }

  // handle pos overlay
  if(id.getDataName() == posOverlayID && id.getRoiID() == posOverlayRoiID) {
    posOverlay = (RtActivation*) getDataStore().getData(id);
    newPosOverlay = true;

    if(DEBUG_LEVEL & BASIC) {
      cout << "display got a pos overlay " << id.getDataName() << endl;
    }

    return;
  }

  // handle neg overlay
  if(id.getDataName() == negOverlayID && id.getRoiID() == negOverlayRoiID) {
    negOverlay = (RtActivation*) getDataStore().getData(id);
    newNegOverlay = true;

    if(DEBUG_LEVEL & BASIC) {
      cout << "display got a neg overlay " << id.getDataName() << endl;
    }

    return;
  }

  // handle pos mask
  if(id.getDataName() == posMaskID && id.getRoiID() == posMaskRoiID) {
    // delete existing if we loaded it from a file
    if(loadInitialPosMask && posMask) {
      // memory leak??
      //delete posMask;
    }

    posMask = (RtMaskImage*) getDataStore().getData(id);
    newPosMask = true;

    if(DEBUG_LEVEL & BASIC) {
      cout << "display got a positive mask " << id.getDataName() << endl;
    }

    return;
  }

  // handle neg mask
  if(id.getDataName() == negMaskID && id.getRoiID() == negMaskRoiID) {
    // delete existing if we loaded it from a file
    if(loadInitialNegMask && negMask) {
      // memory leak??
      //delete negMask;
    }

    negMask = (RtMaskImage*) getDataStore().getData(id);
    newNegMask = true;

    if(DEBUG_LEVEL & BASIC) {
      cout << "display got a negative mask " << id.getDataName() << endl;
    }

    return;
  }

  // handle background image
  if(id.getModuleID() != imageDisplayType) {
    if(DEBUG_LEVEL & BASIC) {
      cout << "ignoring image of type " 
	   << id.getModuleID() 
	   << " because its not " 
	   << imageDisplayType
	   << endl;
    }
    ACE_DEBUG((LM_DEBUG, "ignoring image of type %s\n", id.getModuleID()));
    return;
  }
  
  // MUST BE AN IMAGE?

  // delete existing if we loaded it from a file
  if(loadInitialImage && img) {
    // memory leak??
    //delete img;
  }

  img = (RtMRIImage*) getDataStore().getData(id);

  ACE_DEBUG((LM_DEBUG, "display got an image %d\n", img->getDataID().getTimePoint()));
  //cout << "display got an image " << img->getID() << endl;

  // set the info strings
  bottomStr = img->getDataID().getModuleID() + " " + img->getDataID().getDataName();

  stringstream s;
  s << img->getDataID().getTimePoint();
  topStr = s.str();

  newTex = true;
}

// makes a texture from the image data and prepares it for display
void RtDisplayImage::makeTexture() {
  ACE_TRACE(("RtDisplayImage::makeTexture"));

  /* delete the old texture if there is one */
  if(glIsTexture(imageTex)) {
    glDeleteTextures(1, &imageTex);
  }

  /* get the id for the texture */
  glGenTextures(1, &imageTex);

  if(newImageType) {
    /* contrast */
    float contrast = img->getAutoContrast();
    glPixelTransferf(GL_RED_SCALE,   contrast);
    glPixelTransferf(GL_GREEN_SCALE, contrast);
    glPixelTransferf(GL_BLUE_SCALE,  contrast);

    /* brightness */
    float brightness = img->getAutoBrightness();
    glPixelTransferf(GL_RED_BIAS,   brightness);
    glPixelTransferf(GL_GREEN_BIAS, brightness);
    glPixelTransferf(GL_BLUE_BIAS,  brightness);

    //cout << "image " << img->getID() << " bright: " << brightness << " contrast: " << contrast << endl;

    newImageType = false;
  }

  /* create the image texture */
  glBindTexture(RT_DISPLAY_IMAGE_TEXTURE, imageTex);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  //  cout << "making image texture" << endl;

  // unmosaic if needed
  short *imageData;
//  if(!img->isMosaic()) {
//    imageData = img->getMosaicedCopy();
//    imageW = img->getMosaicedWidth();
//    imageH = img->getMosaicedHeight();
//  }
//  else {
    imageData = new short[img->getNumEl()];
    imageW = img->getDim(0);
    imageH = img->getDim(1);
    imageData = img->getDataCopy();
//  }

  glTexImage2D(RT_DISPLAY_IMAGE_TEXTURE, 0, 1, imageW, imageH,
	       0, GL_LUMINANCE, GL_SHORT, imageData);

  if(!glIsTexture(imageTex)) {
    cerr << "ERROR: could not generate a new texture" << endl;
  }

  delete [] imageData;
  needsRepaint = true;
}

// makes a texture from the overlay data and prepares it for display
void RtDisplayImage::makeOverlayTexture(bool pos) {
  ACE_TRACE(("RtDisplayImage::makeOverlayTexture"));

  GLuint *overlayTex = pos ? &posOverlayTex : &negOverlayTex;
  RtActivation *overlay = pos ? posOverlay : negOverlay;
  
  /* delete the old texture if there is one */
  if(glIsTexture(*overlayTex)) {
    glDeleteTextures(1, overlayTex);
  }

  /* get the id for the texture */
  glGenTextures(1, overlayTex);

  // mosaic if needed
  double *imageData;
  int numImageData;
  if(!overlay->isMosaic()) {
    imageData = overlay->getMosaicedCopy();
    imageW = overlay->getMosaicedWidth();
    imageH = overlay->getMosaicedHeight();
    numImageData = imageW*imageH;
  }
  else {
    imageData = new double[overlay->getNumEl()];
    numImageData = overlay->getNumEl();
    imageData = overlay->getDataCopy();
  }

  // convert overlay data into a displayable image
  short *overlayImg = new short[4*numImageData];

  // debugging
  double min = 1000000, max = -1000000;

  for(int i = 0; i < numImageData; i++) {
    // debugging
      if(min > imageData[i]) {
	min = imageData[i];
      }
      if(max < imageData[i]) {
	max = imageData[i];
      }

    // use a cheap heat colormap
    if(!overlay->getScaleIsInverted() 
       && imageData[i] > threshold) {
      overlayImg[4*i+0] = SHRT_MAX; // r
      overlayImg[4*i+1] = (short) rint(min(1,((imageData[i]
			     -threshold)/overlay->getCeiling()))
				       *SHRT_MAX); // g
      overlayImg[4*i+2] = 0; // b
      overlayImg[4*i+3] = SHRT_MAX; // a
    }
    else if(!overlay->getScaleIsInverted() 
	    && imageData[i] < -threshold) {
      overlayImg[4*i+0] = 0; // r
      overlayImg[4*i+1] = (short) rint(min(1,-1*((imageData[i]
			     +threshold)/overlay->getCeiling()))
				       *SHRT_MAX); // g
      overlayImg[4*i+2] = SHRT_MAX; // b
      overlayImg[4*i+3] = SHRT_MAX; // a
    }
    else if(overlay->getScaleIsInverted() 
	    && fabs(imageData[i]) < threshold) {
      overlayImg[4*i+0] = SHRT_MAX; // r
      overlayImg[4*i+1] = (short) 
	(1 - (fabs(imageData[i])/threshold))
	*SHRT_MAX; // g
      overlayImg[4*i+2] = 0; // b
      overlayImg[4*i+3] = SHRT_MAX; // a
    }
    else {
      overlayImg[4*i+0] = 0; // r
      overlayImg[4*i+1] = 0; // g
      overlayImg[4*i+2] = 0; // b
      overlayImg[4*i+3] = 0; // a
    }
  }
  delete [] imageData;

  /* create the image texture */
  glBindTexture(RT_DISPLAY_IMAGE_TEXTURE, *overlayTex);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage2D(RT_DISPLAY_IMAGE_TEXTURE, 0, 4, imageW, imageH, 0, GL_RGBA,
	       GL_SHORT, overlayImg);

  delete overlayImg;

  if(!glIsTexture(*overlayTex)) {
    cerr << "ERROR: could not generate a new overlay texture" << endl;
  }

  needsRepaint = true;
}

// makes a texture from a pos mask and prepares it for display
void RtDisplayImage::makePosMaskTexture() {
  ACE_TRACE(("RtDisplayImage::makePosMaskTexture"));

  /* delete the old texture if there is one */
  if(glIsTexture(posMaskTex)) {
    glDeleteTextures(1, &posMaskTex);
  }

  // mosaic if needed
  short *imageData;
  int numImageData;
  if(!posMask->isMosaic()) {    
    imageData = posMask->getMosaicedCopy();
    imageW = posMask->getMosaicedWidth();
    imageH = posMask->getMosaicedHeight();
    numImageData = imageW*imageH;
  }
  else {
    imageData = new short[posMask->getNumEl()];
    numImageData = posMask->getNumEl();
    imageData = posMask->getDataCopy();
  }


  /* get the id for the texture */
  glGenTextures(1, &posMaskTex);

  // convert posMask into a displayable image
  short *posMaskImg = new short[4*numImageData];
  for(int i = 0; i < numImageData; i++) {
    posMaskImg[4*i+0] = 0; // r
    posMaskImg[4*i+1] = imageData[i] ? SHRT_MAX : 0; // g
    posMaskImg[4*i+2] = 0; // b
    posMaskImg[4*i+3] = imageData[i] ? SHRT_MAX/3 : 0; // a
  }

  delete [] imageData;


  /* create the image texture */
  glBindTexture(RT_DISPLAY_IMAGE_TEXTURE, posMaskTex);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage2D(RT_DISPLAY_IMAGE_TEXTURE, 0, 4, imageW, imageH, 0, GL_RGBA,
	       GL_SHORT, posMaskImg);

  delete posMaskImg;

  if(!glIsTexture(posMaskTex)) {
    cerr << "ERROR: could not generate a new positive mask texture" << endl;
  }

  needsRepaint = true;
}

// makes a texture from a neg mask and prepares it for display
void RtDisplayImage::makeNegMaskTexture() {
  ACE_TRACE(("RtDisplayImage::makeNegMaskTexture"));

  /* delete the old texture if there is one */
  if(glIsTexture(negMaskTex)) {
    glDeleteTextures(1, &negMaskTex);
  }

  // mosaic if needed
  short *imageData;
  int numImageData;
  if(!negMask->isMosaic()) {    
    imageData = negMask->getMosaicedCopy();
    imageW = negMask->getMosaicedWidth();
    imageH = negMask->getMosaicedHeight();
    numImageData = imageW*imageH;
  }
  else {
    imageData = new short[negMask->getNumEl()];
    numImageData = negMask->getNumEl();
    imageData = negMask->getDataCopy();
  }


  /* get the id for the texture */
  glGenTextures(1, &negMaskTex);

  // convert negMask into a displayable image
  short *negMaskImg = new short[4*numImageData];
  for(int i = 0; i < numImageData; i++) {
    negMaskImg[4*i+0] = 0; // r
    negMaskImg[4*i+1] = 0; // g
    negMaskImg[4*i+2] = imageData[i] ? SHRT_MAX : 0; // b
    negMaskImg[4*i+3] = imageData[i] ? SHRT_MAX/3 : 0; // a
  }

  delete [] imageData;


  /* create the image texture */
  glBindTexture(RT_DISPLAY_IMAGE_TEXTURE, negMaskTex);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage2D(RT_DISPLAY_IMAGE_TEXTURE, 0, 4, imageW, imageH, 0, GL_RGBA,
	       GL_SHORT, negMaskImg);

  delete negMaskImg;

  if(!glIsTexture(negMaskTex)) {
    cerr << "ERROR: could not generate a new negitive mask texture" << endl;
  }

  needsRepaint = true;
}

// debug
// GLuint tex;
// 
// /**
//  * makes the textures for display from the current volume 
//  */
// void makeTextures(int width, int height) {
//   GLuint format = GL_LUMINANCE;
//   GLenum type = GL_SHORT;
//   int bpp = 8*sizeof(short);
// 
//   /* delete the old texture if there is one */
//   if(tex != 0) {
//     return;
//     //glDeleteTextures(1, &tex);
//   }
// 
//   short *img = (short*) malloc(width*height*sizeof(short));
//   for(int i = 0; i < width*height; i++) {
//     img[i] = i;
//   }
// 
//   /* perform texture init */
//   glPixelStorei(GL_UNPACK_ALIGNMENT,1);
//     
// 
//   /* get the id for the textures */
//   glGenTextures(1, &tex);
// 
//   if(!glIsTexture(tex)) {
//     printf("texture id %d\n",tex);
//   }
//   
//   /* create the image texture */
//   glBindTexture(RT_DISPLAY_IMAGE_TEXTURE, tex);
//   glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_WRAP_S, GL_REPEAT);
//   glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_WRAP_T, GL_REPEAT);
//   glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//   glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//   glTexImage2D(RT_DISPLAY_IMAGE_TEXTURE, 0, 1, width, height, 0, format, type, img); 
//   free(img);
// }

void RtDisplayImage::CallBackDisplayFunc(void) {
  ACE_TRACE(("RtDisplayImage::CallBackDisplayFunc"));

  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glEnable(GL_DEPTH_TEST);
  glClearDepth(1.0);
  glDepthFunc(GL_LEQUAL);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable(GL_BLEND);

  //glAlphaFunc(GL_GREATER,0.1);
  //glEnable(GL_ALPHA_TEST);
  //glEnable(GL_CULL_FACE);

  //glClear(GL_COLOR_BUFFER_BIT);

  /* gl init stuff */
  glPushMatrix();
  glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

  // debug
  glColor3d(0.0,1.0,0);
  glRectd(0.0, 0.0, 10, 10);
  glRectd(0.0, height, 10, height-10);
  glRectd(width, height, width-10, height-10);
  glRectd(width, 0.0, width-10, 10);



  // if there is no image yet, draw a message and return
  if(!glIsTexture(imageTex) || img == NULL) {
    drawString(10,10,"no image loaded",1,0,0);

    // debug
//    makeTextures(width,height);
//    
//  /* turn on texture mapping */
//  glEnable(RT_DISPLAY_IMAGE_TEXTURE);
//
//  /* draw the main texture */
//  glBindTexture(RT_DISPLAY_IMAGE_TEXTURE, tex);
//
//  int w = width;
//  int h = height;
//  double tw = width;
//  double th = height;
//
//  /* make a quadrilateral and provide texture coords */
//  glBegin(GL_QUADS); {
//    glTexCoord2d(0.0,0.0);
//    glVertex3f(0, h, 0.0);
//    glTexCoord2d(tw,0.0);
//    glVertex3f(w, h, 0.0);
//    glTexCoord2d(tw,th);
//    glVertex3f(w, 0, 0.0);
//    glTexCoord2d(0,th);
//    glVertex3f(0, 0, 0.0);
//  } glEnd();
//
//  glDisable(RT_DISPLAY_IMAGE_TEXTURE);

    glutSwapBuffers();
    return;
  }

  //  ACE_DEBUG((LM_DEBUG, "showing image %d\n", img->getAcquisitionNum()));

//  int imgw = img->getDim(1);
//  int imgh = img->getDim(0);

  /* turn on texture mapping */
  glEnable(RT_DISPLAY_IMAGE_TEXTURE);

  /* draw the main texture */
  glBindTexture(RT_DISPLAY_IMAGE_TEXTURE, imageTex);

  //cout << "drawing image tex: " << imageTex << endl;

  /* make a quadrilateral and provide texture coords */
  glBegin(GL_QUADS); {
    glTexCoord2d(0.0,0.0);
    glVertex3f(0.0, height, 0.0);
    glTexCoord2d(imageW,0.0);
    glVertex3f(width, height, 0.0);
    glTexCoord2d(imageW, imageH);
    glVertex3f(width, 0.0, 0.0);
    glTexCoord2d(0.0,imageH);
    glVertex3f(0.0, 0.0, 0.0);
  } glEnd();


  /* draw the negMask texture */
  if(negMaskOn && glIsTexture(negMaskTex)) {
    //cout << "drawing neg mask tex: " << negMaskTex << endl;

    glBindTexture(RT_DISPLAY_IMAGE_TEXTURE, negMaskTex);

    /* make a quadrilateral and provide texture coords */
    glBegin(GL_QUADS); {
      glTexCoord2d(0.0,0.0);
      glVertex3f(0.0, height, 0.0);
      glTexCoord2d(imageW,0.0);
      glVertex3f(width, height, 0.0);
      glTexCoord2d(imageW, imageH);
      glVertex3f(width, 0.0, 0.0);
      glTexCoord2d(0.0,imageH);
      glVertex3f(0.0, 0.0, 0.0);
    } glEnd();

  }

  /* draw the posMask texture */
  if(posMaskOn && glIsTexture(posMaskTex)) {
    //cout << "drawing pos mask tex: " << posMaskTex << endl;

    glBindTexture(RT_DISPLAY_IMAGE_TEXTURE, posMaskTex);

    /* make a quadrilateral and provide texture coords */
    glBegin(GL_QUADS); {
      glTexCoord2d(0.0,0.0);
      glVertex3f(0.0, height, 0.0);
      glTexCoord2d(imageW,0.0);
      glVertex3f(width, height, 0.0);
      glTexCoord2d(imageW, imageH);
      glVertex3f(width, 0.0, 0.0);
      glTexCoord2d(0.0,imageH);
      glVertex3f(0.0, 0.0, 0.0);
    } glEnd();

  }

  /* draw the pos overlay texture */
  if(posOverlayOn && glIsTexture(posOverlayTex)) {
    //cout << "drawing pos overlay tex: " << posOverlayTex << endl;

    glBindTexture(RT_DISPLAY_IMAGE_TEXTURE, posOverlayTex);

    /* make a quadrilateral and provide texture coords */
    glBegin(GL_QUADS); {
      glTexCoord2d(0.0,0.0);
      glVertex3f(0.0, height, 0.0);
      glTexCoord2d(imageW,0.0);
      glVertex3f(width, height, 0.0);
      glTexCoord2d(imageW, imageH);
      glVertex3f(width, 0.0, 0.0);
      glTexCoord2d(0.0,imageH);
      glVertex3f(0.0, 0.0, 0.0);
    } glEnd();

  }

  /* draw the neg overlay texture */
  if(negOverlayOn && glIsTexture(negOverlayTex)) {
    //cout << "drawing neg overlay tex: " << negOverlayTex << endl;

    glBindTexture(RT_DISPLAY_IMAGE_TEXTURE, negOverlayTex);

    /* make a quadrilateral and provide texture coords */
    glBegin(GL_QUADS); {
      glTexCoord2d(0.0,0.0);
      glVertex3f(0.0, height, 0.0);
      glTexCoord2d(imageW,0.0);
      glVertex3f(width, height, 0.0);
      glTexCoord2d(imageW, imageH);
      glVertex3f(width, 0.0, 0.0);
      glTexCoord2d(0.0,imageH);
      glVertex3f(0.0, 0.0, 0.0);
    } glEnd();

  }

  glDisable(RT_DISPLAY_IMAGE_TEXTURE);

  // draw the strings
  drawString(10,10,bottomStr.c_str(),1,0,0);
  drawString(10,height-20,topStr.c_str(),0,1,0);

  glutSwapBuffers();

  // take screenshot for movie frame
  if(makeMovie) {
    // build command
    stringstream cmd;
    cmd << screenShotCommand
	<< " -out " << framePrefix;
    cmd.fill('0');
    cmd.width(5);
    cmd << curFrame++;

    //cout << cmd.str();
    
    // PLATFORM DEPENDENT
    system(cmd.str().c_str());
  }
}

void RtDisplayImage::CallBackReshapeFunc(int w, int h){

  if(width != w || height != h) {
    glutReshapeWindow(width,height);
  }
//
//   width = w;
//   height= h;
//
//   cout << w << " " << h << endl;
//
//   glViewport(0, 0, width, height);
//
//   CallBackDisplayFunc();
}


void RtDisplayImage::CallBackTimerFunc(int time, int val) {

  if(newTex) {
    newTex = false;
    makeTexture();
  }

  if(newPosOverlay) {
    newPosOverlay = false;
    makeOverlayTexture(true);
  }

  if(newNegOverlay) {
    newNegOverlay = false;
    makeOverlayTexture(false);
  }

  if(newPosMask) {
    newPosMask = false;
    makePosMaskTexture();
  }

  if(newNegMask) {
    newNegMask = false;
    makeNegMaskTexture();
  }

  if(needsRepaint) {
    needsRepaint = false;
    CallBackDisplayFunc();
  }

}

void RtDisplayImage::CallBackKeyboardFunc(unsigned char key, int x, int y) {
  action(key);
}

void RtDisplayImage::CallBackMenuFunc(int code) {
  action(code);
}

// execute an action in reponse to user input
void RtDisplayImage::action(int code) {
  string oldImageDisplayType = imageDisplayType;

  // for running
  RtConfigFmriRun runConfig;
  string response;

  switch(code) {
  case 'q': // quit
    glutMaster.CallGlutLeaveMainLoop();
    break;
  case 'r': // run
    cout << endl;
    simple_ls(getExperimentConfig().get("study:confDir").str(),".xml");
    cout << endl;

    // ask for a config file name 
    cout << "enter the config file name for the fmri run (q to quit): ";
    cin >> response;
    runConfig.parseConfigFile(getExperimentConfig().get("study:confDir").str() 
			      + response);
    prepareRun(runConfig);
    executeRun(runConfig);    
    break;
  case 's': // scanner image
    imageDisplayType = ID_MOSAIC;
    break;
  case 'd': // difference image
    imageDisplayType = ID_TEMPDIFF;
    break;
  case 'm': // mean image
    imageDisplayType = ID_TEMPMEAN;
    break;
  case '=': // pos mask
  case '+':
    posMaskOn = !posMaskOn;
    break;
  case '-': // neg mask
  case '_':
    negMaskOn = !negMaskOn;
    break;
  case 'n': // inorm
    imageDisplayType = ID_SPATIALINTENSITYNORM;
    break;
  case 'v': // variance image
    imageDisplayType = ID_TEMPVAR;
    break;
  case 't': // threshold val down
    threshold /= thresholdModAmount;
    cout << "new display stat threshold " << threshold << endl;
    break;
  case 'T': // threshold val up
    threshold *= thresholdModAmount;
    cout << "new display stat threshold " << threshold << endl;
    break;
  case 'z': // positive overlay
    posOverlayOn = !posOverlayOn;
    break;
  case 'Z': // negative overlay
    negOverlayOn = !negOverlayOn;
    break;
  }

  // signal if we are displaying a new type of image
  if(imageDisplayType != oldImageDisplayType) {
    newImageType = true;
  }
}


// draws a black box that will enclose a string of text
void RtDisplayImage::drawBlackBoxForString(const char *str, GLint x, GLint y) {
  unsigned int strWid = 0, i;

  for(i = 0; i < strlen(str); i++) {
    strWid += glutBitmapWidth(GLUT_BITMAP_HELVETICA_18, str[i]);
  }

  glColor3f(0.0f,0.0f,0.0f);
  glBegin(GL_QUADS); {
    glVertex3d(x-10,y+20,0.0);
    glVertex3d(x-10,y-10,0.0);
    glVertex3d(x+strWid+10,y-10,0.0);
    glVertex3d(x+strWid+10,y+20,0.0);
  } glEnd();
}

//draws a string at the specified coordinates
void RtDisplayImage::drawString(GLint x, GLint y, const char* s, float r, float g, float b) {
  unsigned int lines, i;

  glMatrixMode(GL_PROJECTION);
  glPushMatrix();
  glLoadIdentity();
  glOrtho(0.0, width, 0.0, height, -1.0, 1.0);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();
  glLoadIdentity();

  drawBlackBoxForString(s,x,y);

  glColor3f(r,g,b);
  glRasterPos2i(x, y);
  for(i = 0, lines = 0; i < strlen(s); i++) {
    if (s[i] == '\n') {
      lines++;
      glRasterPos2i(x, y-(lines*18));
    }
    glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, s[i]);
  }
  glPopMatrix();
  glMatrixMode(GL_PROJECTION);
  glPopMatrix();
  glMatrixMode(GL_MODELVIEW);
}


// NOTE: when we build app using frontend 
// main is defined there
//#ifndef USE_FRONTEND
//
//// main function for the realtime system
//// very simple
//int ACE_TMAIN(int argc, char **args) {
//  ACE_TRACE(("ACE_TMAIN"));
//
//  RtDisplayImage di;
//  runBackend(argc, args);
//  return
//}
//
//#endif

/*************
 * ohinds: 2009-01-18
**************/
#endif //#ifndef USE_FRONTEND

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


