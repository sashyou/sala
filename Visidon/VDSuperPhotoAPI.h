
/** @file VDSuperPhotoAPI.h 
@brief Contains API for Visidon Super Photo Algorithm


Copyright (C) 2014-2016 Visidon Ltd
This file is part of Visidon Super Photo SDK.

*/ 
#ifndef _VDSUPERPHOTO_H_
#define _VDSUPERPHOTO_H_

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
	* General data types for Visidon API. Note that only VD_YUV_NV21, VD_YUV_NV12, YUV YUYV and VD_YUV_420P
	* are suppported by Super Photo Algorithms.
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
        VD_YUV_YUYV, /** < YUV YUYV */
        VD_YUV_UYVY, /** < YUV UYVY (UYVYUYVY...UYVY */
		VD_BAYER10 /** 10 bit raw bayer, 8 bytes per pixel*/
	} VDDatatype;
#endif

	/**
	* Operating mode of the API
	*/
#ifndef __SuperPhotoMode__DEF__
#define __SuperPhotoMode__DEF__
	typedef enum VDSuperPhotoMode
	{
		VD_SCALE, /**< scale images to desired size (1x -->) */
		VD_ZOOM   /**< zoom images (crop from the center and zoom to original input size)*/
	} VDSuperPhotoMode;
#endif

	/**
    * Structure for super photo algorithm initialization parameters
    **/
	typedef struct _VDSuperPhotoInitializeParams
	{
		int totalFrames;			/**< Maximum number of input images to be used with an algorithm. */
        VDDatatype type;			/**< Input image format (supported formats VD_YUV_NV21 / VD_YUV_NV12 / VD_YUV_NV16 / VD_YUV_420P / VD_YUV_YUYV / VD_YUV_UYVY) */
		int cols;					/**< Input image width in true active pixels (note: all input frames need to be same size). Must be divsible by 2. Ouput size is determined based on this value, extra cols are ignored */
		int rows;					/**< Input image height in true active pixels (note: all input frames need to be same size). Must be divisible by 2. Ouput size is determined based on this value, extra rows are ignored */
		int extraPadRows;			/**< Number of extra rows after Y channel to be ignored. There are total (rows+extraPadRows) rows before chroma channels */
		int extraPadCols;			/**< Number of extra pixels after each row to be ignored. */
		VDSuperPhotoMode mode;		/**< Operation mode (VD_SCALE / VD_ZOOM)*/		
		float zoom;					/**< Zoom/scale factor (must be >= 1.0) */
		int outputCols;				/**< Output image width to be used in VD_SCALE mode */
		int outputRows;				/**< Output image height to be used in VD_SCALE mode */
	} VDSuperPhotoInitializeParams;
	
	/**
    * Structure for super photo algorithm processing parameters
    **/
	typedef struct _VDSuperPhotoRuntimeParams
	{
		int noiseReductionStrength;	/**< Noise reduction filter strength (0-10), 0 lowest level (no filtering), 10 highest filtering. */
		int colorFilterStrength;	/**< Separate color filter stregth (0-10), 0 off, 10 highest filtering */		
		int sharpen;				/**< Intelligent sharpening strenght (0-10), 0 lowest sharpening (no sharpening), 10 highest sharpening */
		int contrastStretching;		/**< Increases image contrast. (0-10), 0=off, 10=full strength. */
		int isoValue;				/**< ISO value used for capturing input photos. Set -1 to ignore*/
		float gain;					/**< Gain value used for capturing input photos (alternative for ISO). Set -1 to ignore */
		int dynamicRangeBoost;		/**< Stretches intensity dynamics automatically to available intensity range making dark images brighter (0-10), 0=off, 10=full strength */
		int lowIntensityBoost;		/**< Non-linear intensity adjustment to reveal low-intensity details (brighten image with more weight in dark pixels) (0-10), 0=off, 10=full strength */
		int colorBoost;				/**< Color boosting strength to increase color saturation. Supported values 0-10: 0=off, 10 = maximum. */
		int deghostingStrength;		/**< Additional strength for deghosting. (0-100) 60=default, 0 = off, 1-30 => loose (easily merges moving targets), 70-100 => tight */
	} VDSuperPhotoRuntimeParams;

	/**
	* Initialize super-photo engine
	* @param params Engine initialization parameters to be used with the algorithm
	* @param engine Engine instance pointer
	* @return Error code indicating if initialization was succeed (VD_OK) or failed (VD_NOK)
	**/
	VDErrorType VDInitializeSuperPhoto( VDSuperPhotoInitializeParams params,	void ** engine );

	/**
	* Initialize super-photo engine with specific number of processing cores
	* @param params Engine initialization parameters to be used with the algorithm
	* @param numberOfCores Number of processing cores (typically 4 or 8)
	* @param engine Engine instance pointer
	* @return Error code indicating if initialization was succeed (VD_OK) or failed (VD_NOK)
	**/
	VDErrorType VDInitializeSuperPhotoWithCores(VDSuperPhotoInitializeParams params, int numberOfCores, void ** engine);

	/**
	* Release super-photo engine
	* @param engine Engine instance pointer
	* @return Error code indicating if initialization was succeed (VD_OK) or failed (VD_NOK)
	*/
	VDErrorType VDReleaseSuperPhoto( void ** engine );

	
	/**
	* Returns indication of low light situation. This is combination of illumination and overall darkness of the scene, which hints about noisiness of the scene (and thus suitable filtering)
	* @param isoValue is the ISO value used when input image is captured
	* @param yuvImage pointer to frame pixel data
	* @param cols width of input image
	* @param rows height of input image
	* @return lowLightValue indicating the illumination of scene (0-10, 0= bright condition, 10= very dark). The exact value depend on camera and contens, but roughly: 0: >100 lux, 2: 300 lux, 5: 100 lux, 8: 10 lux, 10: <1 lux
	**/
	int VDSuperPhoto_GetLowLightSceneValue(int isoValue,unsigned char *yuvImage,int cols, int rows);


	/**
	* Add new input frame from burst to the engine. Can be called sequentially for input frames.
	* @param yuvImage Input frame pixel data
	* @param engine Engine instance pointer
	* @return Error code indicating if initialization was succeed (VD_OK) or failed (VD_NOK)
	**/
	VDErrorType VDSuperPhoto_AddFrame( unsigned char *yuvImage, void *engine );

	/**
	* Add new input frame from burst to the engine. Can be called sequentially for input frames. Internal
	* buffer (size of VDSuperPhotoInitializeParams.totalFrames) will be used for keeping the frames
	* in memory (= will make a copy of input).
	* @param yuvImage Input frame pixel data
	* @param engine Engine instance pointer
	* @return Error code indicating if initialization was succeed (VD_OK) or failed (VD_NOK)
	**/
	VDErrorType VDSuperPhoto_AddFrameAndAllocate(unsigned char * yuvImage, void * engine);

	/**
	* Process super-photo algorithm with current images and parameters
	* @param params Engine paramters to be used with the algorithm within process
	* @param engine Engine instance pointer
	* @return Error code indicating if initialization was succeed (VD_OK) or failed (VD_NOK)
	**/
	VDErrorType VDSuperPhoto_Process( VDSuperPhotoRuntimeParams params, void *engine );	

	/**
	 * Get output image widht that should be allocated by used. Use this function
	 * in SCALE mode to find out what is the actual output image widht after processing.
	 * In zooming mode, output image widht is always the same with the input image width.
	 @param engine Engine instance pointer
	 @return True output image width
	 **/
	int VDSuperPhoto_GetOuputWidth(void * engine);
	
	/**
	 * Get output image height that should be allocated by used. Use this function
	 * in SCALE mode to find out what is the actual output image height after processing.
	 * In zooming mode, output image height is always the same with the input image height.
	 @param engine Engine instance pointer
	 @return True output image height
	 **/
	int VDSuperPhoto_GetOuputHeight(void * engine);

	/**
	* Add buffer to engine where the output is written.
	* @param output Pointer to allocated output buffer
	* @param engine Engine instance pointer
	* @return Error code indicating if buffer set was succeed (VD_OK) or failed (VD_NOK)
	**/
	VDErrorType VDSuperPhoto_AddOutputBuffer(unsigned char * output, void * engine);

	/**
	* Get SuperPhoto library version string.
	* @return Version string
	**/
	char * VDSuperPhoto_GetVersion();

#ifdef __cplusplus
}
#endif

#endif


