#ifndef __TS_COMM_DEF_H__
#define __TS_COMM_DEF_H__
typedef enum enumVideoType
{
    VIDEO_PROCSS_NONE = -1,
    VIDEO_PROCESS_VIDEO_DENOISER = 0,
    VIDEO_PROCESS_MOVIE_SOLID,
    VIDEO_PROCESS_MOVIE_SOLID_TS,
    VIDEO_PROCESS_VIDEO_WDR,
    VIDEO_PROCESS_HDR_CHECKER,
}VideoType;
typedef enum enumPhotoType
{
    PHOTO_PROCESS_NONE = -1,
    PHOTO_PROCESS_NIGHTPORTRAIT = 0,
    PHOTO_PROCESS_HDR,
    PHOTO_PROCESS_PHOTOSOLID,
    PHOTO_PROCESS_DENOISER,
    PHOTO_PROCESS_SUPERRESOLUTION,
    PHOTO_PROCESS_SUPERZOOM,
    PHOTO_PROCESS_SUPERRESOLUTIONFAST,
    PHOTO_PROCESS_ULTRANR,
    PHOTO_PROCESS_SMARTSELECT,
}PhotoType;
typedef void (*ALLOCATECALLBACK)(void **buffer, int bufferSize, void *caller);
typedef void (*DEALLOCATECALLBACK)(void *caller);

// public param key
// ts hdr feature param key
#define TS_HDR_CONTRAST  "ts_hdr_contrast"
#define TS_HDR_SATURATION  "ts_hdr_saturation"
#define TS_HDR_THREADNUM  "ts_hdr_threadnum"
#define TS_HDR_GHOST_SENSITIVITY  "ts_hdr_ghost_sensitivity"
#define TS_HDR_GHOST_Removal_StrengthLevel  "ts_hdr_ghost_Removal_StrengthLevel"
#define TS_HDR_GHOST_RATE_Threshold  "ts_hdr_ghost_rate_Threshold"
// end

// end
#endif // __TS_COMM_DEF_H__
