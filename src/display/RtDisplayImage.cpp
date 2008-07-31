/******************************************************************************
 * RtDisplayImage.cpp defines a class for displaying single images
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-08-29
 *
 *****************************************************************************/

#include"RtDisplayImage.h"
#include"RtDataIDs.h"

#include<vnl/vnl_vector.h>
#include "gnuplot_i_vxl.h"


#include"ace/Mutex.h"

// scopic: #define WINDOWS
#ifndef RT_DISPLAY_IMAGE_TEXTURE
  #ifdef WINDOWS 
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
#define DEFAULT_TITLE "real: experimentor"
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
  x = DEFAULT_X; 
  y = DEFAULT_Y; 
  width = DEFAULT_W; 
  height = DEFAULT_H;

  img = NULL; 
  imageTex = 0; 
  imageDisplayType = ID_SCANNERIMG;

  loadInitialImage = false;
  initialImageFilename = "";

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
  loadInitialPosMask = false;
  initialPosMaskFilename = "";
  flipInitialPosMask = false;

  negMaskOn = false;
  loadInitialNegMask = false;
  initialNegMaskFilename = "";
  flipInitialNegMask = false;

  addToID(":display");

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

  imageDisplayType = ID_SCANNERIMG;

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
  addToID(":display");
}

// destructor
RtDisplayImage::~RtDisplayImage() {
  glutDestroyWindow(windowID);
}


// initialize the display
//  in
//   config: config class to get configuration variables from
bool RtDisplayImage::open(RtConfig &config) {
  ACE_TRACE(("RtDisplayImage::open"));

  x = config.isSet("display:imageWinX")
    ? config.get("display:imageWinX") : DEFAULT_X;

  y = config.isSet("display:imageWinY")
    ? config.get("display:imageWinY") : DEFAULT_Y;

  width = config.isSet("display:imageWinW")
    ? config.get("display:imageWinW") : DEFAULT_W;

  height = config.isSet("display:imageWinH")
    ? config.get("display:imageWinH") : DEFAULT_H;

  numMeas = config.isSet("scanner:measurements")
    ? config.get("scanner:measurements") : DEFAULT_NUMMEAS;

  strcpy(title, config.isSet("display:imageWinTitle")
	 ? config.get("display:imageWinTitle").str().c_str() : DEFAULT_TITLE);

  if(config.isSet("display:initialImage")) {
    initialImageFilename = config.get("display:initialImage").str();
    loadInitialImage = true;
  }

  if(config.isSet("display:initialPosMask")) {
    initialPosMaskFilename = config.get("display:initialPosMask").str();
    loadInitialPosMask = true;
  }
  
  if(config.isSet("display:flipInitialPosMask") 
     && config.get("display:flipInitialPosMask") == true) {  
    flipInitialPosMask = true;
  }

  if(config.isSet("display:initialNegMask")) {
    initialNegMaskFilename = config.get("display:initialNegMask").str();
    loadInitialNegMask = true;
  }

  if(config.isSet("display:flipInitialNegMask") 
     && config.get("display:flipInitialNegMask") == true) {  
    flipInitialNegMask = true;
  }

  if(config.isSet("display:overlayID")) {
    posOverlayID = config.get("display:overlayID").str();
    posOverlayOn = true;
  }

  if(config.isSet("display:posOverlayID")) {
    posOverlayID = config.get("display:posOverlayID").str();
    posOverlayOn = true;
  }

  posOverlayRoiID = config.isSet("display:posOverlayRoiID")
	 ? config.get("display:posOverlayRoiID").str() :DEFAULT_POSOVERLAYROIID;

  if(config.isSet("display:negOverlayID")) {
    negOverlayID = config.get("display:negOverlayID").str();
  }

  negOverlayRoiID = config.isSet("display:negOverlayRoiID")
	 ? config.get("display:negOverlayRoiID").str() :DEFAULT_NEGOVERLAYROIID;

  if(config.isSet("display:posMaskID")) {
    posMaskID = config.get("display:posMaskID").str();
    posMaskOn = true;
  }

  posMaskRoiID = config.isSet("display:posMaskRoiID")
	 ? config.get("display:posMaskRoiID").str() : DEFAULT_POSMASKROIID;

  if(config.isSet("display:negMaskID")) {
    negMaskID = config.get("display:negMaskID").str();
    negMaskOn = true;
  }

  negMaskRoiID = config.isSet("display:negMaskRoiID")
	 ? config.get("display:negMaskRoiID").str() : DEFAULT_NEGMASKROIID;

  posActivationSumID = config.isSet("display:posActivationSumID")
	 ? config.get("display:posActivationSumID").str() 
    : DEFAULT_POSACTIVATIONSUMID;
  posActivationSumRoiID = config.isSet("display:posActivationSumRoiID")
	 ? config.get("display:posActivationSumRoiID").str() 
    : DEFAULT_POSACTIVATIONSUMROIID;

  negActivationSumID = config.isSet("display:negActivationSumID")
	 ? config.get("display:negActivationSumID").str() 
    : DEFAULT_NEGACTIVATIONSUMID;
  negActivationSumRoiID = config.isSet("display:negActivationSumRoiID")
	 ? config.get("display:negActivationSumRoiID").str() 
    : DEFAULT_NEGACTIVATIONSUMROIID;

  // movie stuff
  if(config.isSet("display:makeMovie") 
     && config.get("display:makeMovie") == true) {
    makeMovie = true;
  }
  if(config.isSet("display:framePrefix")) {
    framePrefix = config.get("display:framePrefix").str();
  }
  if(config.isSet("display:screenShotCommand")) {
    screenShotCommand = config.get("display:screenshotCommand").str();
  }

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

  /* erase color */
  glClearColor(0.0f, 0.0f, 0.0f, 1);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glOrtho(0.0, (double) width, 0.0, (double) height, 1.0, -1.0);

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

  return true;
}

// thread entry
int RtDisplayImage::svc() {
  ACE_TRACE(("RtDisplayImage::svc"));

  glutMaster.CallGlutMainLoop();

  return 0;
}

// sets the image to be displayed
void RtDisplayImage::setData(RtData *data) {
  ACE_TRACE(("RtDisplayImage::setData"));

  // plot activation sum over time // DEBUGGING ONLY
  static vnl_vector<double> postc(numMeas,0.0);
  static vnl_vector<double> negtc(numMeas,0.0);
//  static Gnuplot gp = Gnuplot("lines");
  static unsigned int numTimepoints = 0;

  //cout << "got data: " << data->getID() << ":" << data->getRoiID() << endl;

  // handle activation sum
  if(data->getID() == posActivationSumID 
     && data->getRoiID() == posActivationSumRoiID) {
//  cout << "display got a pos activation sum: " 
//       << ((RtActivation*)data)->getPixel(0) 
//       << " " << numTimepoints << endl;
    // plot the sum
    postc.put(numTimepoints,((RtActivation*)data)->getPixel(0));
 //   gp.reset_plot();
 //   gp.plot_x(postc,posActivationSumRoiID.c_str());
    numTimepoints++;
    return;
  }

  if(data->getID() == negActivationSumID 
     && data->getRoiID() == negActivationSumRoiID) {
//  cout << "display got a neg activation sum: " 
//       << ((RtActivation*)data)->getPixel(0) 
//       << endl;
    // plot the sum
    negtc.put(numTimepoints,((RtActivation*)data)->getPixel(0));
    //gp.reset_plot();
//    gp.plot_x(negtc,negActivationSumRoiID.c_str());
    //numTimepoints++;
    return;
  }

  // handle pos overlay
  if(data->getID() == posOverlayID && data->getRoiID() == posOverlayRoiID) {
    posOverlay = (RtActivation*) data;
    newPosOverlay = true;
  //cout << "display got a pos overlay " << img->getID() << endl;
    return;
  }

  // handle neg overlay
  if(data->getID() == negOverlayID && data->getRoiID() == negOverlayRoiID) {
    negOverlay = (RtActivation*) data;
    newNegOverlay = true;
  //cout << "display got a neg overlay " << img->getID() << endl;
    return;
  }

  // handle pos mask
  if(data->getID() == posMaskID && data->getRoiID() == posMaskRoiID) {
    // delete existing if we loaded it from a file
    if(loadInitialPosMask && posMask) {
      // memory leak??
      //delete posMask;
    }

    posMask = (RtMaskImage*) data;
    newPosMask = true;

  //cout << "display got a positive mask " << img->getID() << endl;
    return;
  }

  // handle neg mask
  if(data->getID() == negMaskID && data->getRoiID() == negMaskRoiID) {
    // delete existing if we loaded it from a file
    if(loadInitialNegMask && negMask) {
      // memory leak??
      //delete negMask;
    }

    negMask = (RtMaskImage*) data;
    newNegMask = true;

  //cout << "display got a negative mask " << img->getID() << endl;
    return;
  }

  // handle background image
  if(data->getID() != imageDisplayType) {
    ACE_DEBUG((LM_DEBUG, "ignoring image of type %s\n", data->getID()));
    return;
  }
  
  // MUST BE AN IMAGE?

  // delete existing if we loaded it from a file
  if(loadInitialImage && img) {
    // memory leak??
    //delete img;
  }

  img = (RtMRIImage*) data;

  ACE_DEBUG((LM_DEBUG, "display got an image %d\n", img->getAcquisitionNum()));
  cout << "display got an image " << img->getID() << endl;

  // set the info strings
  bottomStr = img->getID();

  stringstream s;
  s << img->getAcquisitionNum();
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

  // unmosaic if needed
//  bool remosaic = false;
//  if(!img->isMosaic()) {
//    img->mosaic();
//    remosaic = true;
//  }

  glTexImage2D(RT_DISPLAY_IMAGE_TEXTURE, 0, 1, img->getDim(0),
	       img->getDim(1), 0, GL_LUMINANCE,
	       GL_SHORT, img->getData());

//  if(remosaic) {
//    img->unmosaic();
//  }

  //cout << "making texture " << imageTex << " from " << img << endl;

  if(!glIsTexture(imageTex)) {
    cerr << "ERROR: could not generate a new texture" << endl;
  }

  needsRepaint = true;
  //CallBackDisplayFunc();
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

  // convert overlay data into a displayable image
  short *overlayImg = new short[4*overlay->getNumPix()];

  // debugging
  double min = 1000000, max = -1000000;

  for(unsigned int i = 0; i < overlay->getNumPix(); i++) {
    // debugging
      if(min > overlay->getPixel(i)) {
	min = overlay->getPixel(i);
      }
      if(max < overlay->getPixel(i)) {
	max = overlay->getPixel(i);
      }

    // use a cheap heat colormap
    if(!overlay->getScaleIsInverted() 
       && overlay->getPixel(i) > overlay->getThreshold()) {
      overlayImg[4*i+0] = SHRT_MAX; // r
      overlayImg[4*i+1] = (short) rint(min(1,((overlay->getPixel(i)
			     -overlay->getThreshold())/overlay->getCeiling()))
				       *SHRT_MAX); // g
      overlayImg[4*i+2] = 0; // b
      overlayImg[4*i+3] = SHRT_MAX; // a
//      cout << overlay->getPixel(i) << " " << overlay->getThreshold() 
//	   << " " << overlay->getCeiling() << " " 
//	   << overlayImg[4*i+1] << " "
//	   << overlayImg[4*i+0] << "," 
//	   << overlayImg[4*i+1] << "," 
//	   << overlayImg[4*i+2] << "," 
//	   << overlayImg[4*i+3] << "," 
//	   << endl;
    }
    else if(!overlay->getScaleIsInverted() 
	    && overlay->getPixel(i) < -overlay->getThreshold()) {
      overlayImg[4*i+0] = 0; // r
      overlayImg[4*i+1] = (short) rint(min(1,-1*((overlay->getPixel(i)
			     +overlay->getThreshold())/overlay->getCeiling()))
				       *SHRT_MAX); // g
      overlayImg[4*i+2] = SHRT_MAX; // b
      overlayImg[4*i+3] = SHRT_MAX; // a
//      cout << overlay->getPixel(i) << " " << overlay->getThreshold() 
//	   << " " << overlay->getCeiling() << " " 
//	   << overlayImg[4*i+1] << endl;
    }
    else if(overlay->getScaleIsInverted() 
	    && fabs(overlay->getPixel(i)) < overlay->getThreshold()) {
      overlayImg[4*i+0] = SHRT_MAX; // r
      overlayImg[4*i+1] = (short) 
	(1 - (fabs(overlay->getPixel(i))/overlay->getThreshold()))
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

  // debugging
//  cout << "thresh=" << overlay->getThreshold() << endl 
//       << "min=" << min << endl << "max=" << max << endl;

  /* create the image texture */
  glBindTexture(RT_DISPLAY_IMAGE_TEXTURE, *overlayTex);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage2D(RT_DISPLAY_IMAGE_TEXTURE, 0, 4, overlay->getDim(0),
	       overlay->getDim(1), 0, GL_RGBA,
	       GL_SHORT, overlayImg);

  delete overlayImg;

  if(!glIsTexture(*overlayTex)) {
    cerr << "ERROR: could not generate a new overlay texture" << endl;
  }

  needsRepaint = true;
  //CallBackDisplayFunc();
}

// makes a texture from a pos mask and prepares it for display
void RtDisplayImage::makePosMaskTexture() {
  ACE_TRACE(("RtDisplayImage::makePosMaskTexture"));

  /* delete the old texture if there is one */
  if(glIsTexture(posMaskTex)) {
    glDeleteTextures(1, &posMaskTex);
  }

  /* get the id for the texture */
  glGenTextures(1, &posMaskTex);

  // convert posMask into a displayable image
  short *posMaskImg = new short[4*posMask->getNumEl()];
  for(unsigned int i = 0; i < posMask->getNumEl(); i++) {
    posMaskImg[4*i+0] = 0; // r
    posMaskImg[4*i+1] = posMask->getPixel(i) ? SHRT_MAX : 0; // g
    posMaskImg[4*i+2] = 0; // b
    posMaskImg[4*i+3] = posMask->getPixel(i) ? SHRT_MAX/3 : 0; // a
  }

  /* create the image texture */
  glBindTexture(RT_DISPLAY_IMAGE_TEXTURE, posMaskTex);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage2D(RT_DISPLAY_IMAGE_TEXTURE, 0, 4, posMask->getDim(0),
	       posMask->getDim(1), 0, GL_RGBA,
	       GL_SHORT, posMaskImg);


  delete posMaskImg;

  if(!glIsTexture(posMaskTex)) {
    cerr << "ERROR: could not generate a new positive mask texture" << endl;
  }

  needsRepaint = true;
}

// makes a texture from a negative mask and prepares it for display
void RtDisplayImage::makeNegMaskTexture() {
  ACE_TRACE(("RtDisplayImage::makeNegMaskTexture"));

  /* delete the old texture if there is one */
  if(glIsTexture(negMaskTex)) {
    glDeleteTextures(1, &negMaskTex);
  }

  /* get the id for the texture */
  glGenTextures(1, &negMaskTex);

  // convert negMask into a displayable image (cyan background?)
  short *negMaskImg = new short[4*negMask->getNumEl()];
  for(unsigned int i = 0; i < negMask->getNumEl(); i++) {
    negMaskImg[4*i+0] = negMask->getPixel(i) ? SHRT_MAX : 0; // r
    negMaskImg[4*i+1] = 0; // g
    negMaskImg[4*i+2] = negMask->getPixel(i) ? SHRT_MAX : 0; // b
    negMaskImg[4*i+3] = negMask->getPixel(i) ? SHRT_MAX/3 : 0; // a
  }

  /* create the image texture */
  glBindTexture(RT_DISPLAY_IMAGE_TEXTURE, negMaskTex);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexParameteri(RT_DISPLAY_IMAGE_TEXTURE, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

  glTexImage2D(RT_DISPLAY_IMAGE_TEXTURE, 0, 4, negMask->getDim(0),
	       negMask->getDim(1), 0, GL_RGBA,
	       GL_SHORT, negMaskImg);

  delete negMaskImg;

  if(!glIsTexture(negMaskTex)) {
    cerr << "ERROR: could not generate a new negative mask texture" << endl;
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

  int imgw = img->getDim(1);
  int imgh = img->getDim(0);

  /* turn on texture mapping */
  glEnable(RT_DISPLAY_IMAGE_TEXTURE);

  /* draw the main texture */
  glBindTexture(RT_DISPLAY_IMAGE_TEXTURE, imageTex);

  //cout << "drawing image tex: " << imageTex << endl;

  /* make a quadrilateral and provide texture coords */
  glBegin(GL_QUADS); {
    glTexCoord2d(0.0,0.0);
    glVertex3f(0.0, height, 0.0);
    glTexCoord2d(imgw,0.0);
    glVertex3f(width, height, 0.0);
    glTexCoord2d(imgw, imgh);
    glVertex3f(width, 0.0, 0.0);
    glTexCoord2d(0.0,imgh);
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
      glTexCoord2d(imgw,0.0);
      glVertex3f(width, height, 0.0);
      glTexCoord2d(imgw, imgh);
      glVertex3f(width, 0.0, 0.0);
      glTexCoord2d(0.0,imgh);
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
      glTexCoord2d(imgw,0.0);
      glVertex3f(width, height, 0.0);
      glTexCoord2d(imgw, imgh);
      glVertex3f(width, 0.0, 0.0);
      glTexCoord2d(0.0,imgh);
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
      glTexCoord2d(imgw,0.0);
      glVertex3f(width, height, 0.0);
      glTexCoord2d(imgw, imgh);
      glVertex3f(width, 0.0, 0.0);
      glTexCoord2d(0.0,imgh);
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
      glTexCoord2d(imgw,0.0);
      glVertex3f(width, height, 0.0);
      glTexCoord2d(imgw, imgh);
      glVertex3f(width, 0.0, 0.0);
      glTexCoord2d(0.0,imgh);
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

  if(width != w | height != h) {
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
  string oldImageDisplayType = imageDisplayType;

  switch(key) {
  case 'q':
    exit(0);
    break;
  case 's':
    imageDisplayType = ID_SCANNERIMG;
    break;
  case 'd':
    imageDisplayType = ID_DIFFIMG;
    break;
  case 'm':
    imageDisplayType = ID_MEANIMG;
    break;
  case '=':
  case '+':
    posMaskOn = !posMaskOn;
    break;
  case '-':
  case '_':
    negMaskOn = !negMaskOn;
    break;
  case 'n':
    imageDisplayType = ID_INTENSITYNORMIMG;
    break;
  case 'v':
    imageDisplayType = ID_VARIMG;
    break;
  case 'z':
    posOverlayOn = !posOverlayOn;
    break;
  case 'Z':
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

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


