/** @file VDBeautyEngine.h
    @brief Contains API for Visidon Face Beautificatiom Algorithms


    Copyright (C) 2013-2015 Visidon Oy
    This file is part of Visidon Beaty Shot SDK

*/
#ifndef _VDBEAUTYENGINE_H_
#define _VDBEAUTYENGINE_H_

#include <stdint.h>

#ifdef __cplusplus
extern "C"
{
#endif
    /**
    Max number of filters
    */
#define NUMBER_OF_BEAUTYFILTERS MAX_BEAUTY_FILTER
#define MAX_BEAUTY_SHOT_FACES_IN_TRACK 5

    /**
    Enumeration for different beauty filters
    */
    typedef enum _VDBeautyFilters {
        SKIN_SMOOTHING=0, /**< Intelligent skin smoothing algorithm*/
        SPOT_SMOOTHING=1,/**< Adaptive spot and wrinkle removal algorithm (part of SKIN_SMOOTHING)*/
        FACE_CONTOURING=2, /**< Face contouring algorithm*/
        TEETH_WHITENING=3,/**< Teeth whitening algorithm*/
        EYE_SHAPING=4,/**< Eye enlargement algorithm*/
        CHEEK_BLUSHING=5,/**< Cheek blusher algorithm*/
        SKIN_TONE_ADJUSTING=6,/**< Skin toning algorithm*/
        MOUTH_SHAPING=7,/**< Mouth shaping algorithm*/
        NOSE_SHAPING = 8,/**< Nose shaping algorithm*/
        EYEBROW_SHAPING=9,/**< Eye-brown shaping algorithm*/
        FACE_SHAPING=10, /**< Face shaping (thinning) algorithm*/
        OUT_FOCUS=11, /**< Sharp face area, blurred background */
        EYE_BRIGHTENING=12, /**< Eye brightening algorithm */
		LIP_TONING=13, /**< Lip enhancement algorithm */
        SKIN_COLOR_ADJUSTING=14, /**< Skin color adjustment algorithm (red tone adjustment) */
		SKIN_COLOR_CHANGE = 15, /**< Skin color changing alogithm */
        MAX_BEAUTY_FILTER /**< Keep this entry last */
    } VDBeautyFilters;

    /**
    Enumeration for different image formats supported by the engine.
    */
    typedef enum _VDBeatyShotImageFormat
    {
        YUV_420_NV12=0, /**< YUV420SPNV12 format*/
        YUV_420_NV21=1, /**< YUV420SPNV21 format*/
        RGB_888=2,  /**< 24-bit RGB format*/
        JPEG_COMPRESSED=3, /**< JPEG compressed format*/
		YUV_YUYV = 4, /**< YUYV format (YUYV...YUYV..YUYV)*/
		YUV_YV12 = 5, /**< YUV YV12 format */
    } VDBeatyShotImageFormat;


    /**
    Enumeration for different memory modes to be used with the GPU processing.
    Some devices do not support GraphicBuffer memory copying (MEMCPY_GRAPHICBUFFER)
    and one needs to use read pixels approach (MEMCPY_READPIXELS).
    */
    typedef enum _VDBeautyEngineGPUMemoryMode
    {
        MEMCPY_READPIXELS=0, /**< Use glReadPixels to reed data from GPU*/
        MEMCPY_GRAPHICBUFFER=1,/**< Use GraphicBuffer (gralloc) memory copying to read data from GPU*/
        RENDER_TO_WINDOW=2,/**< Render to window. Do not read data from GPU */
    } VDBeautyEngineGPUMemoryMode;

    /**
    Enumeration for different operation modes supported by the engine.
    */
    typedef enum _VDBeautyEngineProcessingMode
    {
        VDBEAUTY_REALTIME_WITH_FACETRACKING_AND_STILL = 0, /**< Use both preview and still processing and apply face tracking in preview mode.*/
        VDBEAUTY_REALTIME_AND_STILL = 1,/**< Use both preview and still processing but do not apply face tracking in preview mode (filtering is applied to skin like areas instead of detected face)*/
        VDBEAUTY_STILL = 2,/**< Use only still image processing*/
    } VDBeautyEngineProcessingMode;

    /**
    Structure for face coordinates that may be provided outside beauty engine (i.e. from
    hardware face tracker).
    */
    typedef struct _VDBeautyFace
    {
        int left; /**< Left x coordinate of face in the image*/
        int top; /**< Top y coordinate of face in the image*/
        int right; /**< Rigt x coordinate of face in the image*/
        int bottom;/**< Bottom y coordinate of face in the image*/
        int leftEyeX; /**< Left eye x coordinate if available*/
        int leftEyeY;/**< Left eye y coordinate if available*/
        int rightEyeX; /**< Right eye x coordinate if available*/
        int rightEyeY; /**< Right eye t coordinate if available*/
		int smile;
    }VDBeautyFace;

    /**
    Structure for input/output data to be used with processing functions.
    */
    typedef struct _VDBeautyShotData
    {
        unsigned char *imagePtr; /**< Pointer to image data (input/output) */
        int imageCols; /**< Image width*/
        int imageRows;/**< Image height*/
        int nbrOfFaces; /**< Number of faces in the faces array */
        VDBeautyFace faces[MAX_BEAUTY_SHOT_FACES_IN_TRACK];
        int jpegDataSize;/**< Compressed data size in case of JPEG format*/
        int jpegSavingQuality; /**< JPEG saving quality (0-100)*/
        unsigned char *outputJpeg; /**< Newly allocated JPEG compressed data in case of JPEG format*/
        int ouputJpegDataSize; /**< Compressed output data size in case of JPEG format*/
        VDBeatyShotImageFormat imageFormat; /**< Used image format*/
        int32_t beautyFilterParams[NUMBER_OF_BEAUTYFILTERS]; /**< Filter strength parameters*/
        int forceSmoothing; /**< Force face smoothing even though no face is found (apply to skin like regions)*/
		int colorNoiseReduction; /**< Color noise (chroma) reduction strength (0-10) */
		int luminanceNoiseReduction; /**< Luminance noise reduction strength (0-10) */
		int sharpening; /**< Sharpening strength (0-10) */
		int padRows;	/**< Number of extra rows after valid Y rows before chroma channel. */
		int padCols;	/**< Number of extra pixels after each row to be ignored. */
		int stillImageRotation; /**< Defines how still image and preview frame are oriented w.r.t. each other. Supported values are 90 (=preview rotated toward right), and 270 (=preview rotated towards left). */
    }VDBeautyShotData;

    /**
    Enumeration for different error codes returend by API functions.
    */
    typedef enum _VDBeautyErrorCode
    {
        VDBEAUTY_OK=0, /**< Operation ok*/
        VDBEAUTY_NOK=1, /**< Operation not ok*/
    } VDBeautyErrorCode;

	/**
	Initialize Visidon Beauty Shot Engine. This function allocates the resources needed by the engine
	depending on the operation mode. User needs to provide the current preview image resolution as well as
	desired opration mode and memory mode to exhange data between CPU and GPU. Engine is initialized to
	user provided pointer and all later usage is needed to be done by this instance pointer.
	@param previewCols Preview image widht
	@param previewRows Preview image height
	@param processingMode Operation mode, see VDBeautyEngineProcessingMode for mode detail
	@param gpuMemoryMode Memory copying mode between GPU and CPU, see VDBeautyEngineGPUMemoryMode for mode details
	@param numberOfCores Number of processing cores to be used by algorithm
	@param engine Instance of initialized engine. The later API usage is done via this pointer.
	@return BEAUTY_OK (0) if initialization succeeded, otherwise BEAUTY_NOK
	*/
	VDBeautyErrorCode VDInitializeBeautyShotWithCores(int previewCols, int previewRows, VDBeautyEngineProcessingMode processingMode, VDBeautyEngineGPUMemoryMode gpuMemoryMode, int numberOfCores, void ** engine);


    /**
    Initialize Visidon Beauty Shot Engine. This function allocates the resources needed by the engine
    depending on the operation mode. User needs to provide the current preview image resolution as well as
    desired opration mode and memory mode to exhange data between CPU and GPU. Engine is initialized to
    user provided pointer and all later usage is needed to be done by this instance pointer.
    @param previewCols Preview image widht
    @param previewRows Preview image height
    @param processingMode Operation mode, see VDBeautyEngineProcessingMode for mode detail
    @param gpuMemoryMode Memory copying mode between GPU and CPU, see VDBeautyEngineGPUMemoryMode for mode details
    @param engine Instance of initialized engine. The later API usage is done via this pointer.
    @return BEAUTY_OK (0) if initialization succeeded, otherwise BEAUTY_NOK
    */
    VDBeautyErrorCode VDInitializeBeautyShot(int previewCols,int previewRows,VDBeautyEngineProcessingMode processingMode, VDBeautyEngineGPUMemoryMode gpuMemoryMode, void ** engine);

	/**
	Initialize Visidon Beauty Shot Engine to render ouput directly to the window. This function allocates the resources needed by the engine
	depending on the operation mode. User needs to provide the current preview image resolution as well as
	desired opration mode and memory mode to exhange data between CPU and GPU. User also provides pointer to the ANativeWindow
	where the output is put. Engine is initialized to user provided pointer and all later usage is needed to be done by this instance pointer.
	@param previewCols Preview image widht
	@param previewRows Preview image height
	@param processingMode Operation mode, see VDBeautyEngineProcessingMode for mode detail
	@param gpuMemoryMode Memory copying mode between GPU and CPU, see VDBeautyEngineGPUMemoryMode for mode details
	@param window void type pointer to ANativeWindow where the output is renderer.
	@param numberOfCores Number of processing cores to be used by algorithm
	@param engine Instance of initialized engine. The later API usage is done via this pointer.
	@return BEAUTY_OK (0) if initialization succeeded, otherwise BEAUTY_NOK
	*/
	VDBeautyErrorCode VDInitializeBeautyShotWithWindowAndCores(int previewCols, int previewRows, VDBeautyEngineProcessingMode processingMode, VDBeautyEngineGPUMemoryMode gpuMemoryMode, void* window, int numberOfCores, void ** engine);
    
	/**
    Initialize Visidon Beauty Shot Engine to render ouput directly to the window. This function allocates the resources needed by the engine
    depending on the operation mode. User needs to provide the current preview image resolution as well as
    desired opration mode and memory mode to exhange data between CPU and GPU. User also provides pointer to the ANativeWindow
    where the output is put. Engine is initialized to user provided pointer and all later usage is needed to be done by this instance pointer.
    @param previewCols Preview image widht
    @param previewRows Preview image height
    @param processingMode Operation mode, see VDBeautyEngineProcessingMode for mode detail
    @param gpuMemoryMode Memory copying mode between GPU and CPU, see VDBeautyEngineGPUMemoryMode for mode details
    @param window void type pointer to ANativeWindow where the output is renderer.
    @param engine Instance of initialized engine. The later API usage is done via this pointer.
    @return BEAUTY_OK (0) if initialization succeeded, otherwise BEAUTY_NOK
    */
    VDBeautyErrorCode VDInitializeBeautyShotWithWindow(int previewCols,int previewRows, VDBeautyEngineProcessingMode processingMode, VDBeautyEngineGPUMemoryMode gpuMemoryMode, void* window, void ** engine);

    /**
    Setup window rendering ouput with desired size and orientation. User needs to call this after engine is initialized
    to window rendering mode for setting up correct preview image orientation and size.
    @param outWidth Render windows widhth
    @param outHeight Render windows height
    @param transformMatrix 4x4 transformation matrix to flip camera preview image in correct orientation in the window
    @param engine Instance of initialized engine.
    @return BEAUTY_OK (0) if setup succeeded, otherwise BEAUTY_NOK
    */
    VDBeautyErrorCode VDSetupPreviewOutput(int outWidth, int outHeight, float *transformMatrix, int *scissorBox, void *engine);

    /**
    Release allocaed resources and stop engine.
    @param engine Instance of engine (pointer to initialized pointer).
    @return BEAUTY_OK (0) if initialization succeeded, otherwise BEAUTY_NOK
    */
    VDBeautyErrorCode VDReleaseBeautyShot(void ** engine);

    /**
    Apply preview processing to input data. Image format needs to be YUV.
    @param data Pointer to input data structure
    @param engine Pointer to instance of initialized engine
    @return BEAUTY_OK (0) if initialization succeeded, otherwise BEAUTY_NOK
    */
    VDBeautyErrorCode VDProcessPreviewFrame(VDBeautyShotData *data, void *engine);

    /**
    Apply beautification to captured photo.
    @param data Pointer to input data structure
    @param engine Pointer to instance of initialized engine
    @return BEAUTY_OK (0) if initialization succeeded, otherwise BEAUTY_NOK
    */
    VDBeautyErrorCode VDProcessStillImage(VDBeautyShotData *data, void *engine);

	/**
	Set preview beauty shot filter mode (fast / normal).
	@param isSetMinorMode 1=light (fast) filters, 0= full (slower) filters.
	@param engine Pointer to instance of initialized engine
	@return BEAUTY_OK (0) if succeeded, otherwise BEAUTY_NOK
	*/
	VDBeautyErrorCode VDSetMinorMode(int isSetMinorMode, void *engine);
	
	/**
	* Get beauty shot library version string.
	* @return Version string
	**/
	char * VDBeautyShot_GetVersion();

	/**
	* Get latest face tracking result from the preview. Coordinates of the rectange are in original image
	* coordinate system. 
	* @param faceIndex Face index to be (= 0 -> numberOfFaces (see int VDGetNumberOfBeautyFaces(void *engine);)
	* @param engine Pointer to instance of initialized engine
	* @return Latest face coordinates
	**/
	VDBeautyFace VDGetBeautyFace(int faceIndex, void *engine);

	/**
	* Get number of faces currently in track (max number of faces is MAX_BEAUTY_SHOT_FACES_IN_TRACK)
	* @param engine Pointer to instance of initialized engine
	* @return Number of faces
	**/
	int VDGetNumberOfBeautyFaces(void *engine);

#ifdef __cplusplus
}
#endif

#endif
