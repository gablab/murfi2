/******************************************************************************
 * RtDataIDs.h contains defines for data id strings
 *
 * Oliver Hinds <ohinds@mit.edu> 2007-09-12
 *
 *****************************************************************************/

#ifndef RTDATAIDS_H
#define RTDATAIDS_H

// remember to modify RtStream.cpp to add the new module type

// setup is that processing module ids have the format:
//  #define ID_<MODULENAME>
// and below that define names of all types of data that the module produces:
//  #define NAME_<MODULENAME>_<DATANAME>


#define ID_STREAM                 		"generic-stream"

#define ID_SCANNERIMG                 		"mri"
  #define NAME_SCANNERIMG_EPI         		"epi"
  #define NAME_SCANNERIMG_T1          		"t1"
  #define NAME_SCANNERIMG_T2          		"t2"
  #define NAME_SCANNERIMG_T1EPI       		"t1wepi"

#define ID_MOSAIC                     		"mosaic"
// same names as scanner image

#define ID_UNMOSAIC                   		"unmosaic"
// same names as scanner image

#define ID_TEMPDIFF                   		"temporal-difference"
  #define NAME_TEMPDIFF_IMG           		"diff-img"

#define ID_TEMPMEAN                   		"temporal-mean"
#define NAME_TEMPMEAN_IMG             		"mean-img"

#define ID_TEMPVAR                    		"temporal-variance"
  #define NAME_TEMPVAR_IMG            		"var-img"

#define ID_SPATIAL_INTENSITYNORM      		"spatial-intensity-norm"
  #define NAME_SPATIAL_INTENSITYNORM_IMG        "inorm-img"

#define ID_TEMPORALZSCORE            		"zscore"
  #define NAME_TEMPORALZSCORE_IMG     		"zscore-img"

#define ID_SLIDEWIN                		"slidewin"
  #define NAME_SLIDEWIN_COR                	"cor-img"
  #define NAME_SLIDEWIN_TSTAT                	"tstat-img"
  #define NAME_SLIDEWIN_BETA                	"beta-img"

#define ID_ACCUMCOR                   		"accumcor"
  #define NAME_ACCUMCOR_COR                   	"cor-img"
  #define NAME_ACCUMCOR_TSTAT                  	"tstat-img"
  #define NAME_ACCUMCOR_BETA              	"beta-img"

#define ID_INCREMENTALGLM             		"incrglm"
  #define NAME_INCREMENTALGLM_COR              	"cor-img"
  #define NAME_INCREMENTALGLM_TSTAT            	"tstat-img"
  #define NAME_INCREMENTALGLM_BETA            	"beta-img"

// note: the names here will be appended with <_conditionName>
#define ID_SINGLEIMCOR                		"singleimcor"
  #define NAME_SINGLEIMCOR_ACTIVATION          	"activation-img"
  #define NAME_SINGLEIMCOR_BETA            	"beta-img"

#define ID_FLUCTUATIONMONITOR         		"fluctuationmonitor"
  #define NAME_FLUCTUATIONMONITOR_ACTIVATION  	"activation-img"
  #define NAME_FLUCTUATIONMONITOR_BETA        	"beta-img"

#define ID_ACTIVATIONSUM              		"activation-sum"
  #define NAME_ACTIVATIONSUM           		"sum"

#define ID_ACTIVATIONSUMDIFFERENCE    		"activation-sum-difference"
  #define NAME_ACTIVATIONSUMDIFFERENCE 		"diff"

#define ID_EVENTTRIGGER               		"event-trigger"
#define ID_EVENTTRIGGER_ACTIVATIONSUMS 		"event-trigger-activation-sums"
#define ID_EVENTTRIGGER_ACTIVATIONDIFF 		"event-trigger-activation-diff"
  #define NAME_EVENTTRIGGER_SIGNAL             	"signal"
  #define NAME_EVENTTRIGGER_BAD               	"bad"
  #define NAME_EVENTTRIGGER_GOOD               	"good"

#endif

/*****************************************************************************
 * $Source$
 * Local Variables:
 * mode: c++
 * fill-column: 76
 * comment-column: 0
 * End:
 *****************************************************************************/


