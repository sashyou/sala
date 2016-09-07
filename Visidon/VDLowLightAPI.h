/** @file VDLowLightAPI.h 
@brief Contains API for Visidon Low-Light Algorithm


Copyright (C) 2014-2016 Visidon Oy
This file is part of Visidon Low-Light SDK.

*/ 
#ifndef _VDLOWLIGHT_H_
#define _VDLOWLIGHT_H_

#ifdef __cplusplus
extern "C" 
{
#endif

    /**
    General error codes for Visidon API
    */
    #ifndef __ErrorType__DEF__
    #define __ErrorType__DEF__
        typedef enum _VDErrorType {
            VD_OK=0, /**< Operation ok*/
            VD_NOK /**< Operation not ok*/
        } VDErrorType;
    #endif

    /**
    * General data types for Visidon API. Note that only VD_YUV_NV21, VD_YUV_NV12, YUV NV16, and YUV YUYV
    * are suppported by Low Light Shot Algorithm.
    */
    #ifndef __Datatype__DEF__
    #define __Datatype__DEF__
    typedef enum _VDDatatype {
        VD_RawGray=1, /**< 8-bit grayscale image*/
        VD_YUV422,/**< YUV422*/
        VD_RGB565, /**< RGB565 */
        VD_RGBA8888, /**< 32-bit RGB+A*/
        VD_RawBayer, /**< Specific Bayer pattern format*/
        VD_RGB888, /**< 24-bit RGB */
        VD_JPEG, /**< JPEG compression */
        VD_YUV_NV21, /**< YUV NV21 */
        VD_YUV_NV12, /**< YUV NV12 */
        VD_YUV_NV16, /**< YUV NV16 */
        VD_YUV_420P, /**< YUV 420P (planar format Y....U..V.. */
        VD_YUV_YUYV, /** < YUV YUYV (YUYVYUYV...YUYV */
        VD_YUV_UYVY,  /** < YUV UYVY (UYVYUYVY...UYVY */
		VD_BAYER10 /** 10 bit raw bayer, 8 bytes per pixel*/
    } VDDatatype;
    #endif

    /**
    * Structure for low-light algorithm input parameters
    **/
    typedef struct _VDLowLightParameters
    {
        VDDatatype type;			    /**< Input image format (supported formats VD_YUV_NV21 / VD_YUV_NV12 / VD_YUV_420P / VD_YUV_YUYV / VD_YUV_UYVY) */
        int noiseReductionStrength;		/**< Noise reduction filter strength (1-10), 1 lowest level, 10 highest filtering. */
        int colorFilterStrength;		/**< Separate color filter stregth (0-10), 0 off, 10 highest filtering */
        int dynamicRangeBoost;			/**< Stretches intensity dynamics automatically to available intensity range making dark images brighter (0-10), 0=off, 10=full strength */
		int lowIntensityBoost;			/**< Non-linear intensity adjustment to reveal low-intensity details (brighten image with more weight in dark pixels) (0-10), 0=off, 10=full strength */ 
		int contrastStretching;			/**< Increases image contrast. (0-10), 0=off, 10=full strength.  While dynamicRangeBoost makes sure that full range is utilized by automatic adjustment, contrastStretching can be used as (a possibly additional) value forcing different kind of contrast enhancement.
											ContrastStretching does not introduce saturation or color clipping, but makes ark darker and bright brighter. For dark images, this alone may make dark images less readable, so this parameter should be utilized together with lowIntensityBoost and dynamicRangeBoost parameters.*/
		int colorBoost;					/**< Color boosting strength to increase color saturation. Supported values 0-10: 0=off, 10 = maximum. */
        int colorNormalization;			/**< Strength of Automatic color normalization / white balance in percentages. 0=0ff, 1-99 = 1%-99%, 100= full). Default = 0 */
		int effectiveFrames;			/**< Number of sharpest frames that are used in lowLight process (n=0 => use all, n=[1-totalFrames] = use n sharpest frames, sharpest one as a reference). Default=totalFrames */
        int deghostingStrength;			/**< Additional strength for deghosting. (0-100) 50=default, 0 = off, 1-30 => loose (easily merges moving targets), 70-100 => tight */		
		int movingObjectNoiseRemoval;	/**< Additional removal of noise from moving objects. 0= off, 1= on, 2 = auto (default)*/
		int cols;						/**< Input image width in pixels (note: all input frames need to be same size). */
        int rows;						/**< Input image height in pixels (note: all input frames need to be same size). */
        int extraPadRows;				/**< Number of extra rows after Y channel to be ignored. There are total (rows+extraPadRows) rows before chroma channels */
        int extraPadCols;				/**< Number of extra pixels after each row to be ignored. */
        int totalFrames;				/**< Maximum number of input images to be used with an algorithm. */
		int sharpen;					/**< Intelligent edge sharpening. Supported values 0-10: 0=off, 10 = largest sharpening. */
    } VDLowLightParameters;

    /**
    * Initialize low-light engine
    * @param params Engine paramters to be used with the algorithm
    * @param engine Engine instance pointer
    * @return Error code indicating if initialization was succeed (VD_OK) or failed (VD_NOK)
    **/
    VDErrorType VDInitializeLowLight(VDLowLightParameters params, void ** engine);

	/**
	* Initialize low-light engine with specific number of processing cores
	* @param params Engine initialization parameters to be used with the algorithm
	* @param numberOfCores Number of processing cores (typically 4 or 8)
	* @param engine Engine instance pointer
	* @return Error code indicating if initialization was succeed (VD_OK) or failed (VD_NOK)
	**/
	VDErrorType VDInitializeLowLightWithCores(VDLowLightParameters params, int numberOfCores, void ** engine);

    /**
    * Release low-light engine
    * @param engine Engine instance pointer
    * @return Error code indicating if initialization was succeed (VD_OK) or failed (VD_NOK)
    */
    VDErrorType VDReleaseLowLight(void ** engine);

    /**
    * Add new input frame from burst to the engine
    * @param yuvImage Input frame pixel data in YUV NV12 format
    * @param allocate Flag indicating if own memory for frame is allocated (=1), or use shared memory with an calling application (=0)
    * @param engine Engine instance pointer
    * @return Error code indicating if initialization was succeed (VD_OK) or failed (VD_NOK)
    **/
    VDErrorType VDLowLight_AddFrame(unsigned char *yuvImage,int allocate, void *engine);

    /**
    * Process low-light algorithm with current images and parameters
    * @param outputYUV Pointer to already allocated output buffer containing the result image in YUV NV12 format
    * @param engine Engine instance pointer
    * @return Error code indicating if initialization was succeed (VD_OK) or failed (VD_NOK)
    **/
    VDErrorType VDLowLight_Process(unsigned char *outputYUV, void *engine);

	/**
	* Get LLS library version string.
	* @return Version string
	**/
	char * VDLowLight_GetVersion();

#ifdef __cplusplus
}
#endif

#endif
