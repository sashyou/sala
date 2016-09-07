/** @file VDHDRApi.h
@brief Contains API for Visidon HDR Algorithm


Copyright (C) 2014-2015 Visidon Oy
This file is part of Visidon HDR SDK.

*/
#ifndef _VDHDRAPI_H_
#define _VDHDRAPI_H_

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
		VD_OK = 0, /**< Operation ok*/
		VD_NOK /**< Operation not ok*/
	} VDErrorType;
#endif

    /**
    * General data types for Visidon API. Note that only VD_YUV_NV21 and VD_YUV_NV12
    * are suppported by HDR algorithm.
    */
    #ifndef __Datatype__DEF__
    #define __Datatype__DEF__
    typedef enum _VDDatatype {
		VD_RawGray = 1, /**< 8-bit grayscale image*/
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
        VD_YUV_UYVY /** < YUV UYVY (UYVYUYVY...UYVY */
    } VDDatatype;
    #endif

	/**
	* Structure for HDR algorithm input parameters
	**/
	typedef struct _VDHDRParameters
	{
        VDDatatype type;			    /**< Input image format (supported formats VD_YUV_NV21 / VD_YUV_NV12 / VD_YUV_NV16 / VD_YUV_420P / VD_YUV_YUYV / VD_YUV_UYVY) */
		int cols;						/**< Input image width in pixels (note: all input frames need to be same size). */
		int rows;						/**< Input image height in pixels (note: all input frames need to be same size). */
		int extraPadRows;				/**< Number of extra rows after Y channel to be ignored. There are total (rows+extraPadRows) rows before chroma channels */
		int extraPadCols;				/**< Number of extra pixels after each row to be ignored. */
		int totalFrames;				/**< Maximum number of input images to be used with an algorithm. */
		int contrastEnhancement;		/**< Additional contrast enhancement strength (0-10)/ */
        int colorBoost;					/**< Color boosting strength to increase color saturation. Supported values 0-100: 0=off, 100 = maximum. */
        int noiseReductionStrength;		/**< Noise reduction filter strength (1-10), 1 lowest level, 10 highest filtering. */
        int colorFilterStrength;		/**< Separate color filter stregth (0-10), 0 off, 10 highest filtering */
        int sharpen;					/**< Intelligent edge sharpening. Supported values 0-10: 0=off, 10 = largest sharpening. */
	} VDHDRParameters;

    /**
     * @brief VDHDRGetDefaultParams initialize params with default values. Use this
     *        function to initialize parameters, modify returned params if
     *        needed and pass to VDInitializeHDR()
     * @param cols width of the images
     * @param rows height of the images
     * @return params struct with default values
     */
    VDHDRParameters VDHDRGetDefaultParams( int cols, int rows );

	/**
	* Initialize HDR engine
	* @param params Engine parameters to be used with the algorithm
	* @param engine Engine instance pointer
	* @return Error code indicating if initialization was succeed (VD_OK) or failed (VD_NOK)
	**/
	VDErrorType VDInitializeHDR(VDHDRParameters params, void ** engine);

	/**
	* Release HDR engine
	* @param engine Engine instance pointer
	* @return Error code indicating if initialization was succeed (VD_OK) or failed (VD_NOK)
	*/
	VDErrorType VDReleaseHDR(void ** engine);

	/**
	* Add new input frame from burst to the engine
	*
	* For stereo HDR:
	* frames fed to algorithm have to obey the following restrictions:
	*	-NV21 format (YUV)
	*	-Y range must be 0-255 (full swing), i.e. saturation occurs around intensity of 255
	*	-Every other image must be from same sensor [left or right] (e.g. Left_t1,Right_t1,Left_t2,Right_t2 ; or Right_t1,Left:t1,Right_t2, etc.)
	*
    * @param yuvImage Input frame pixel data in YUV NV12 format
	* @param engine Engine instance pointer
	* @return Error code indicating if initialization was succeed (VD_OK) or failed (VD_NOK)
	**/
	VDErrorType VDHDR_AddFrame(unsigned char *yuvImage, void *engine);

	/**
    * Process HDR algorithm with current images and parameters
    * @param outputYUV Pointer to already allocated output buffer containing the result image in YUV NV12 format
	* @param engine Engine instance pointer
    * @return Number of frames used to construct HDR output, on failure negative value is returned
	**/
    int VDHDR_Process(unsigned char *outputYUV, void *engine);

	/**
	* Get HDR library version string.
	* @return Version string
	**/
	char * VDHDR_GetVersion();

#ifdef __cplusplus
}
#endif

#endif
