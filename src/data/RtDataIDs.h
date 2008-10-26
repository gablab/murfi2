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

#define ID_MASK                 		"mask"
 #define NAME_MASK       		        "mask-img"

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

#define ID_SPATIALINTENSITYNORM      		"spatial-intensity-norm"
  #define NAME_SPATIALINTENSITYNORM_IMG        "inorm-img"

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
  #define NAME_INCREMENTALGLM_RESIDUAL         	"res-img"

// note: the names here will be appended with <_conditionName>
#define ID_SINGLEIMCOR                		"singleimcor"
  #define NAME_SINGLEIMCOR_ACTIVATION          	"activation-img"
  #define NAME_SINGLEIMCOR_BETA            	"beta-img"
  #define NAME_SINGLEIMCOR_RESIDUAL          	"residual-img"

#define ID_SLIDEWINCOR                		"slidewincor"
  #define NAME_SLIDEWINCOR_COR          	"correlation-img"
  #define NAME_SLIDEWINCOR_STAT            	"stat-img"
  #define NAME_SLIDEWINCOR_ACTIVATION          	"activation-img"
  #define NAME_SLIDEWINCOR_BETA_MEAN           	"beta-mean-img"
  #define NAME_SLIDEWINCOR_BETA_ACT            	"beta-activation-img"

#define ID_FLUCTUATIONMONITOR         		"fluctuation-monitor"
  #define NAME_FLUCTUATIONMONITOR_ACTIVATION  	"activation-img"
  #define NAME_FLUCTUATIONMONITOR_BETA        	"beta-img"

#define ID_CONTRAST             		"contrast"
  #define NAME_CONTRAST_IMG            	        "con-img"
  #define NAME_CONTRAST_TSTAT            	"tstat-img"

#define ID_ROI2FEEDBACK             		"roi2feedback"

#define ID_ROISUM2FEEDBACK             		"roi-sum2feedback"
  #define NAME_ROISUM            		"roi-sum"

#define ID_ROIMEAN2FEEDBACK             	"roi-mean2feedback"
  #define NAME_ROIMEAN         	                "roi-mean"

#define ID_ROIWEIGHTEDAVE2FEEDBACK             	"roi-weightedave2feedback"
  #define NAME_ROIWEIGHTEDAVE            	"roi-weightedave"

#define ID_ROIMEDIAN2FEEDBACK             	"roi-median2feedback"
  #define NAME_ROIMEDIAN                 	"roi-median"

#define ID_ROIDIFFERENCE             	        "roi-activation-difference"
  #define NAME_ROIDIFFERENCE                 	"activation-difference"

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


