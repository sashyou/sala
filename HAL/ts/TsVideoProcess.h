#ifndef __INCLUDE_TS_VIDEO_PROCESS_H__
#define __INCLUDE_TS_VIDEO_PROCESS_H__
#include "tscommdef.h"

class VideoProcess;
class TsVideoProcess
{
public:
    TsVideoProcess();
    ~TsVideoProcess();

    bool setProcessType(VideoType type, ALLOCATECALLBACK allocMem, DEALLOCATECALLBACK deallocMem);
    int setParam(char* key, int value);
    int doProcess(unsigned char *yBuffer, unsigned char *uvBuffer, const int &width, const int &height, void *caller);
    int doProcess(unsigned char *yBuffer, unsigned char *uvBuffer, const int &width, const int &height, const int &stride, void *caller);
    int finishProcess();
private:
    VideoProcess *m_pVideoProcess;
};

#endif // end __INCLUDE_TS_VIDEO_PROCESS_H__
