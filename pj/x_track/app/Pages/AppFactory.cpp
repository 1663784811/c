#include "AppFactory.h"
#include "StartUp/StartUp.h"
#include "Dialplate/Dialplate.h"
#include "LiveMap/LiveMap.h"
#include "SystemInfos/SystemInfos.h"

#define APP_CLASS_MATCH(className)\
do {\
    if (strcmp(name, #className) == 0)\
    {\
        return new Page::className;\
    }\
} while (0)

PageBase *AppFactory::CreatePage(const char *name)
{
    APP_CLASS_MATCH(StartUp);
    APP_CLASS_MATCH(Dialplate);
    APP_CLASS_MATCH(LiveMap);
    APP_CLASS_MATCH(SystemInfos);
    
    return nullptr;
}
