#ifndef FR_NOTIFY
#define FR_NOTIFY

// Undefined for any reason
#define FRN_UNDEFINED 0

//--------------------------------------------------------------------
// Any subcontroller can send a notification message
//  to main controller. Main controller may process it
// or may just ignore. 
// By using notification tool can force update
// of pipline or even reinstall it.

// Pipline update notification
#define FRN_PIPLINE_UPDATE  1    // Force reinstall render pipline
#define FRN_TBC_UPDATE      2    // Update threshold/brightness/contrast
#define FRN_SETNEXTSLICE    3	 // set next slice in multi-slice image

// Some other notification codes

//-------------------------------------------------------------------
// MainController can manage pipeline of view using specified
// points in that pipeline. So define that points.
#define FRP_FULL        0
#define FRP_READIMAGE   1
#define FRP_SLICE       2
#define FRP_TBC         3
#define FRP_SETCAM      4

#endif
