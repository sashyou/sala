#ifndef VDSINGLESHOTHDRAPI_H_
#define VDSINGLESHOTHDRAPI_H_

#ifdef __cplusplus
extern "C" {
#endif
	/**
	Enumeration for different error codes returend by API functions.
	*/
	typedef enum _VDSingleShotHDRErrorCode
	{
		VDSINGLESHOTHDR_OK=0, /**< Operation ok*/
		VDSINGLESHOTHDR_NOK=1, /**< Operation not ok*/
	} VDSingleShotHDRErrorCode;


	/**
	Enumeration for different parameters that can be passed to engine via API.
	These parameters are used to setup preview display when rendering directly
	on the screen.
	*/
	typedef enum _VDSingleShotHDRParam
	{
		VDDRO_TRANSFORM_MATRIX = 0, /**< Transform matrix for transforming image pixels to screen (value is type of float[16] */
		VDDRO_PREVIEW_WIDTH    = 1, /**< Preview display widht */
		VDDRO_PREVIEW_HEIGHT   = 2, /**< Preview display height */
        VDDRO_RECORDING_SURFACE= 3,
        VDDRO_RECORD           = 4,
        VDDRO_RECORDING_TIMESTAMP =5,
        VDDRO_RECORDING_TRANSFORM_MATRIX = 6,
        VDDRO_RECORDING_WIDTH    = 7,
        VDDRO_RECORDING_HEIGHT   = 8,
		VDDRO_IMAGE_FORMAT = 9, /**< Input YUV format*/
		VDDRO_NOISEREMOVAL_STRENGTH =10, /**< Additional noise removal. 0 (off) - 100 (max)*/
	} VDSingleShotHDRParam;

    typedef enum _VDSingleShotHDRImageFormat
    {
		IMAGE_FORMAT_NV21 = 17, /**< Input image is NV21*/
        IMAGE_FORMAT_YV12 = 842094169, /**< Input image is YV12*/
        IMAGE_FORMAT_YUYV = 20, /**< Input image is YUYV (only supported in still processing!)*/
    }VDSingleShotHDRImageFormat;

	/**
	Enumeration for different memory modes to be used with the GPU processing.
	*/
	typedef enum _VDSingleShotHDRGPUMemoryMode
	{
		MEMCPY_PIXELBUFFEROBJECT=0, /**< Use Pixel Buffer Object (PBO) to read data from GPU */
		RENDER_TO_WINDOW=1,/**< Render to window. Do not read data from GPU */
	} VDSingleShotHDRGPUMemoryMode;
	
	/**
    Initialize Visidon Single Shot HDR Engine. This function allocates the resources needed by the engine
    depending on the operation mode. User needs to provide the current preview image resolution as well as
    desired opration mode and memory mode (must be MEMCPY_PIXELBUFFEROBJECT) to exhange data between CPU and GPU.
	Engine is initialized to user provided pointer and all later usage is needed to be done by this instance pointer.
  	@param cols Preview image widht
	@param rows Preview image height    
    @param gpuMemoryMode Memory copying mode between GPU and CPU, see VDSingleShotHDRGPUMemoryMode for mode details
    @param engine Instance of initialized engine. The later API usage is done via this pointer.
    @return VDSINGLESHOTHDR_OK (0) if initialization succeeded, otherwise VDSINGLESHOTHDR_NOK
    */
	VDSingleShotHDRErrorCode VDInitializeSingleShotHDR(int cols, int rows, VDSingleShotHDRGPUMemoryMode gpuMemoryMode, void **engine);

	/**
	Initialize Visidon Single Shot HDR Engine to render ouput directly to the window. This function allocates the resources needed by the engine
	depending on the operation mode. User needs to provide the current preview image resolution as well as
	desired memory mode (must be RENDER_TO_WINDOW) to exhange data between CPU and GPU. User also provides pointer to the ANativeWindow
	where the output is put. Engine is initialized to user provided pointer and all later usage is needed to be done by this instance pointer.
    @param cols Preview image width
	@param rows Preview image height
	@param gpuMemoryMode Memory copying mode between GPU and CPU, see VDSingleShotHDRGPUMemoryMode for mode details
	@param window void type pointer to ANativeWindow where the output is renderer.
	@param engine Instance of initialized engine. The later API usage is done via this pointer.
	@return VDSINGLESHOTHDR_OK (0) if initialization succeeded, otherwise VDSINGLESHOTHDR_NOK
	*/
	VDSingleShotHDRErrorCode VDInitializeSingleShotHDRWithWindow(int cols, int rows, void *window, VDSingleShotHDRGPUMemoryMode gpuMemoryMode, void **engine);

	/**
    Release allocaed resources and stop engine.
    @param engine Instance of engine (pointer to initialized engine pointer).
    @return VDSINGLESHOTHDR_OK (0) if release succeeded, otherwise VDSINGLESHOTHDR_NOK
    */
	VDSingleShotHDRErrorCode VDReleaseSingleShotHDR(void **engine);

	/**
	Set paramters to configure preview display. Possible paramters are defined in enumeration VDSingleShotHDRParam.
	@param what Paremeter to be configured (see VDSingleShotHDRParam).
	@param value Paramter value.
	@param engine Instance of initialized engine.
	@return VDSINGLESHOTHDR_OK (0) if parameter set succeeded, otherwise VDSINGLESHOTHDR_NOK
	*/
	VDSingleShotHDRErrorCode VDSetSingleShotHDRParameter(int what, void* value, void *engine);

	/**
	Process input image with Single Shot HDR algorithm.
	@param image Input / output YUV image (preview frame from camera)
	@param intensityStrength Algorithm intensity boosting strength value (0 - 100)
	@param contrastStrength Algorithm contrast boosting strength value (0 - 100)
	@param engine Instance of initialized engine.
	@return VDSINGLESHOTHDR_OK (0) if processing succeeded, otherwise VDSINGLESHOTHDR_NOK
	*/
	VDSingleShotHDRErrorCode VDProcessSingleShotHDRFrame(unsigned char *image, int intensityStrength,int contrastStrength, void* engine);

    /**
    Process still input image with Single Shot HDR algorithm.
    @param image Input / output YUV image (full resolution still image)
    @param cols Still image width
    @param rows Still image height
    @param format YUV format of input/output (also supports YUYV)
    @param intensityStrength Algorithm intensity boosting strength value (0 - 100)
    @param contrastStrength Algorithm contrast boosting strength value (0 - 100)
    @return VDSINGLESHOTHDR_OK (0) if processing succeeded, otherwise VDSINGLESHOTHDR_NOK
    */
    VDSingleShotHDRErrorCode VDProcessSingleShotHDRStill(unsigned char *image, int cols, int rows, VDSingleShotHDRImageFormat format, int intensityStrength,int contrastStrength);
		
	/**
	* Get Single Shot HDR library version string.
	* @return Version string
	**/
	char * VDSingleShotHDR_GetVersion();
#ifdef __cplusplus
}
#endif
#endif // VDSINGLESHOTHDRAPI_H_
