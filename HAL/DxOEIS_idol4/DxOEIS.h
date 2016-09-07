//==========================================================
// DxO Labs proprietary and confidential information
// Copyright (C) DxO Labs 1999-2012 - (All rights reserved)
//==========================================================

#ifndef __DxOEIS_h__
#define __DxOEIS_h__

#include <stdint.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>

#define  INC(f) <f>

#ifdef __cplusplus
extern "C" {
#endif

// ====================
// Device Configuration
// ====================

/** Buffer size that must be allocated by customer when calling DxOEIS_init(). */
#define DxOEIS_STATE_SIZE            295688

/** Buffer size that must be allocated by customer when calling DxOEIS_process(). */
#define DxOEIS_TMP_SIZE              4

/** Maximum accepted input image width in pixels. */
#define DxOEIS_MAXWIDTH             1920
/** Maximum accepted input image height in pixels. */
#define DxOEIS_MAXHEIGHT            1080

/** Minimum accepted input image width in pixels. */
#define DxOEIS_MINWIDTH             640
/** Minimum accepted input image height in pixels. */
#define DxOEIS_MINHEIGHT            480

/** Maximum supported number of parallel threads */
#define DxOEIS_MAXTHREAD            8

/** indicates that gyroscope informations can be exploited */
#define DxOEIS_GYRO                 1

// List of format supported
#define DxOEIS_INPUT_NV12           1
#define DxOEIS_INPUT_YUV422         0

// =====================
// Functional interface
// =====================

/** Status returned by all DxOEIS main functions */
typedef enum {
	DxOEIS_ok                            =  0
,	DxOEIS_widthTooLarge                 =  1 // assert( width  <= DxOEIS_MAXWIDTH  )
,	DxOEIS_heightTooLarge                =  2 // assert( height <= DxOEIS_MAXHEIGHT )
,	DxOEIS_widthTooSmall                 =  3 // assert( width > DxOEIS_MINWIDTH )
,	DxOEIS_heightTooSmall                =  4 // assert( height > DxOEIS_MINHEIGHT )
,	DxOEIS_borderTooSmall                =  5 // assert( border >= 0.0 )
,	DxOEIS_borderTooLarge                =  6 // assert( border <= DxOEIS_MAX_MVT_PERCENT )
,	DxOEIS_transSmoothTooLarge           =  7 // assert( transSmooth <= 1.0 )
,	DxOEIS_transSmoothTooSmall           =  8 // assert( transSmooth >= 0.0 )
,	DxOEIS_rotSmoothTooLarge             =  9 // assert( rotSmooth <= 1.0 )
,	DxOEIS_rotSmoothTooSmall             = 10 // assert( rotSmooth >= 0.0 )
,	DxOEIS_digitalScalingProductTooSmall = 11 // assert( digitalScalingProduct >= 0.0 )
,	DxOEIS_vbRatioTooLarge               = 12 // assert( (input height  * digitalScalingProduct )/sensorFrameLength <= 1.0 )
,	DxOEIS_invalidInputFormat            = 13 // assert( format < DxOEIS_nbImgFormat )
,	DxOEIS_invalidOutputFormat           = 14 // assert( format < DxOEIS_nbImgFormat )
,	DxOEIS_invalidRowStride              = 15 // assert( rowStride >= width )
,	DxOEIS_invalidMode                   = 16 // assert( mode < DxOEIS_nbMode )
,	DxOEIS_invalidImgDefinitionChange    = 17 // image size/format has changed since init
,	DxOEIS_invalidMatrix                 = 18 // input coordinates do not fit within input image when computing output image
,	DxOEIS_invalidConfiguration          = 19 // configuration not handle in allowed configurations
,	DxOEIS_imageDefinitionMismatch       = 20 // mismatch between in and out image definitions : not supported in this release
,	DxOEIS_numberOfThreadTooLarge        = 21 // assert( nThread <= DxOEIS_MAX_THREAD )
,	DxOEIS_numberOfThreadTooSmall        = 22 // assert( nThread >= 1                 )
,	DxOEIS_widthOutImageTooLarge         = 23 // output image width cannot be greater than input image width
,	DxOEIS_heightOutImageTooLarge        = 24 // output image height cannot be greater than input image height
,	DxOEIS_threadInitFail                = 25 // one of the init thread function fail
,	DxOEIS_threadCancelFail              = 26 // one of cancellation fail
,	DxOEIS_gyroEnableFail                = 27 // enable gyro as required fail
,	DxOEIS_gyroGetMinDelayFail           = 28 // get min delay for gyro
,	DxOEIS_gyroSetEventRateFail          = 29 // fix gyro event rate as required fail
,	DxOEIS_gyroDisableFail               = 30 // disable gyro as required fail
,	DxOEIS_nbErrcode                     = 31
} DxOEIS_errCode ;

/** Versioning */
typedef struct {
	uint16_t majorId ;
	uint16_t minorId ;
} DxOEIS_version ;

/** Frame format */
typedef enum {
	DxOEIS_NV12
,	DxOEIS_YUV422_coplanar
,	DxOEIS_nbImgFormat
} DxOEIS_imgFormat ;

/** Image definition */
typedef struct {
	DxOEIS_imgFormat format ; /**< image format */
	size_t           width  ; /**< image width (number of Y pixels horizontally) */
	size_t           height ; /**< image height (number of Y pixels vertically) */
} DxOEIS_imgDef ;

typedef struct {
	uint8_t u;
	uint8_t v;
} DxOEIS_UV ;

typedef struct {
	uint8_t u ;
	uint8_t y0;
	uint8_t v ;
	uint8_t y1;
} DxOEIS_YUV422 ;

typedef union {
	struct {
		DxOEIS_YUV422* pix       ;
		size_t         rowStride ; // distance from the first pixel of a line to the first pixel of the following line
	} YUV422 ;
	struct {
		uint8_t*   pixY        ;
		size_t     rowStrideY  ; // distance from the first pixel of a line to the first pixel of the following line
		DxOEIS_UV* pixUV       ;
		size_t     rowStrideUV ; // distance from the first pixel of a line to the first pixel of the following line
	} NV12 ;
} DxOEIS_ptrDesc ;

typedef struct {
	DxOEIS_imgDef  def ;
	DxOEIS_ptrDesc ptr ;
} DxOEIS_imgDesc ;

/** EIS mode */
typedef enum {
	DxOEIS_off         = 0x0 /**< output matrix is identity - no processing is done - perform a centered crop */
,	DxOEIS_passThrough = 0x1 /**< output matrix only compensates for borders crop - all processing is done - perform a centered crop */
,	DxOEIS_on          = 0x4 /**< output matrix is computed to stabilize rigid motion and compensate ERS */
,	DxOEIS_nbMode
} DxOEIS_mode ;

/** EIS configuration */
typedef struct {
	float                 border           ; /**< stabilization margin on each side, expressed in percentage of input image size */
	uint32_t              nThread          ; /**< number of used threads, min 1, max MAX_THREAD */
	uint8_t               gyroDisabled     ; /**< disable gyroscope for test purpose */
	float                 lensFocalInPixel ; /**< lens focal in pixels. */
} DxOEIS_config ;

/** EIS settings */
typedef struct {
	float         transSmooth           ; /**< Translation smoothing factor:
	                                       0.0 gives the least stabilized translation motion, but with very good tracking of the scene.
	                                       1.0 gives the most stabilized translation motion, at the expense of a rigidity in the tracking of the scene. */
	float         rotSmooth             ; /**< Rotation smoothing factor: same behavior than translation smoothing, applied on rotation motion. */
	float         digitalScalingProduct ; /**< product of all digital scaling factors. */
	size_t        sensorFrameLength     ; /**< sensor Frame Length ( total vertical lines of the sensor including vertical blanking lines ) */
	uint16_t      droppedFrames         ; /**< Number of dropped frames between two calls to DxOEIS_process() main function. */
	DxOEIS_mode   mode                  ; /**< EIS mode */
} DxOEIS_settings ;

/** Buffer to store algorithm state.
It must be allocated by customer when calling DxOEIS_init(). */
typedef uint64_t DxOEIS_state  [(DxOEIS_STATE_SIZE +sizeof(uint64_t)-1)/sizeof(uint64_t)] ;
/** Buffer to store temp data.
It must be allocated by customer when calling DxOEIS_process(). */
typedef uint32_t DxOEIS_tmp    [(DxOEIS_TMP_SIZE   +sizeof(uint32_t)-1)/sizeof(uint32_t)] ;


/** Function to return DxO EIS version number */
DxOEIS_version DxOEIS_getVersion();


/** EIS initialization function.
Must be called before any other call ! */
DxOEIS_errCode DxOEIS_init (
	DxOEIS_config const config    /**< [input] EIS configuration */
,	DxOEIS_imgDef const imgInDef  /**< [input] EIS input frame definition */
,	DxOEIS_imgDef const imgOutDef /**< [input] EIS output frame definition */
,	DxOEIS_state        state     /**< [input/output] EIS state */
) ;


/** EIS processing function.
To be called for each new input frame. */
DxOEIS_errCode DxOEIS_process (
	DxOEIS_imgDesc *      imgIn    /**< [input] descriptor of input image */
,	DxOEIS_imgDesc *      imgOut   /**< [input/output] descriptor of output image */
,	DxOEIS_settings const settings /**< [input] EIS settings */
,	DxOEIS_state          state    /**< [input/output] EIS state */
,	DxOEIS_tmp            tmp      /**< [input/output] EIS working temp buffer : can be remapped at each call of DxOEIS_process */
) ;


/** EIS finalization function.
Must be called at the end of all processings. */
DxOEIS_errCode DxOEIS_finalize (
	DxOEIS_state state /**< [input/output] EIS state */
) ;

/** EIS thread info function.
Must be called after DxOEIS_init to retreive pthread_t of threaded created.
This allows the user to change externally of DxoEIS Library scheduling policy or other thread behaviour
Note that the thread index 0 give the main thread. */

DxOEIS_errCode DxOEIS_getThreadList(
	DxOEIS_state state
,	pthread_t**  pThreadList
) ;

typedef DxOEIS_version (*pDxOEIS_getVersion)() ;

typedef DxOEIS_errCode (*pDxOEIS_init)(
	DxOEIS_config const config    // input
,	DxOEIS_imgDef const imgInDef  // input
,	DxOEIS_imgDef const imgOutDef // input
,	DxOEIS_state        state     // input/output
) ;

typedef DxOEIS_errCode (*pDxOEIS_process)(
	DxOEIS_imgDesc *      imgIn    // input
,	DxOEIS_imgDesc *      imgOut   // input/output
,	DxOEIS_settings const settings // input
,	DxOEIS_state          state    // input/output
,	DxOEIS_tmp            tmp      // input/output : can be remapped at each call of DxOEIS_process
) ;

typedef DxOEIS_errCode (*pDxOEIS_finalize)(
	DxOEIS_state state // input/output
) ;

typedef DxOEIS_errCode (*pDxOEIS_getThreadList)(
	DxOEIS_state state
,	pthread_t**  pThreadList
) ;

#ifdef __cplusplus
} // extern "C"
#endif


#endif //__DxOEIS_h__
