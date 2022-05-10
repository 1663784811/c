#include "app.h"
#include "Utils/PageManager/PageManager.h"
#include "Pages/AppFactory.h"
#include "Common/DataProc/DataProc.h"
#include "Resource/ResourcePool.h"
#include "Pages/StatusBar/StatusBar.h"

AppFactory factory;
PageManager manager(&factory);

#define ACCOUNT_SEND_CMD(ACT, CMD)\
do{\
    DataProc::ACT##_Info_t info;\
    memset(&info, 0, sizeof(info));\
    info.cmd = DataProc::CMD;\
    DataProc::Center()->AccountMain.Notify(#ACT, &info, sizeof(info));\
}while(0)

void App_Init(void)
{
    DataProc_Init();

    lv_obj_remove_style_all(lv_scr_act());
    lv_disp_set_bg_color(lv_disp_get_default(), lv_color_black());

    Resource.Init();    //资源池初始化，全局资源管理

    StatusBar::Init(lv_layer_top());  //状态栏初始化

    manager.Install("StartUp",     "Pages/StartUp");
    manager.Install("Dialplate",   "Pages/Dialplate");
    manager.Install("LiveMap",     "Pages/LiveMap");
    manager.Install("SystemInfos", "Pages/SystemInfos");

    manager.SetGlobalLoadAnimType(PageManager::LOAD_ANIM_OVER_TOP, 500);

    manager.Push("Pages/StartUp");
    
    ACCOUNT_SEND_CMD(Storage,   STORAGE_CMD_LOAD);
    ACCOUNT_SEND_CMD(SysConfig, SYSCONFIG_CMD_LOAD);
}
