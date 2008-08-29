#ifndef FR_NOTIFY
#define FR_NOTIFY

// Undefined for any reason
#define FRN_UNDEFINED 0

// Any subcontroller can send a notification message
//  to main controller. Main controller may process it
// or may just ignore. 
// By using notification tool can force update
// of pipline or even reinstall it.

// Pipline update notification
#define FRN_PIPLINE_UPDATE 1    // Force reinstall render pipline
#define FRN_TCB_UPDATE 2        // Update threshold/brightnesss/contrast

// Some other notification codes

#endif
