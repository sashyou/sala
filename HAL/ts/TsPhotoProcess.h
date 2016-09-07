#ifndef __INCLUDE_TS_PHOTO_PROCESS_H__
#define __INCLUDE_TS_PHOTO_PROCESS_H__
#include "tscommdef.h"

class PhotoProcess;
class TsPhotoProcess
{
public:
    TsPhotoProcess();
    ~TsPhotoProcess();

    int setProcessType(PhotoType type);
    int addYuvBuf(unsigned char *yBuf, unsigned char *uvBuf, int &width, int &height);
    int doProcess(unsigned char *dstYBuf, unsigned char *dstUVBuf, int &w, int &h);
    int setParam(const char* key, int value);
private:
    PhotoProcess *m_pPhotoProcess;
};

#endif // end __INCLUDE_TS_PHOTO_PROCESS_H__
