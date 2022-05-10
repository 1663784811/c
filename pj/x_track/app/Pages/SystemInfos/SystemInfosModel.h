#ifndef __SYSTEM_INFOS_MODEL_H
#define __SYSTEM_INFOS_MODEL_H

#include "Common/DataProc/DataProc.h"

namespace Page
{

class SystemInfosModel
{
public:
    void Init();
    void Deinit();

    void GetSportInfo(
        float* trip,
        char* time, uint32_t len,
        float* maxSpd
    );

    void GetGPSInfo(
        float* lat,
        float* lng,
        float* alt,
        char* utc, uint32_t len,
        float* course,
        float* speed
    );

    void GetMAGInfo(
        float* dir,
        int* x,
        int* y,
        int* z
    );

    void GetIMUInfo(
        int* step,
        char* info, uint32_t len
    );

    void GetRTCInfo(
        char* dateTime, uint32_t len
    );

    void GetBatteryInfo(
        int* usage,
        float* voltage,
        char* state, uint32_t len
    );

    void GetStorageInfo(
        bool* detect,
        char* usage, uint32_t len
    );

private:
    Account* account;

private:

};

}

#endif
