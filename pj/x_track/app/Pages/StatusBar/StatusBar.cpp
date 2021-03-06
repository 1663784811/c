#include "StatusBar.h"
#include "Common/DataProc/DataProc.h"

#define BATT_USAGE_HEIGHT   (lv_obj_get_style_height(ui.battery.img, 0) - 6)
#define BATT_USAGE_WIDTH    (lv_obj_get_style_width(ui.battery.img, 0) - 4)

#define STATUS_BAR_HEIGHT 22

static Account* actStatusBar;

static void StatusBar_AnimCreate(lv_obj_t* contBatt);

struct
{
   lv_obj_t *cont;
   
   /* 卫星 */
   struct 
   {
       lv_obj_t *img;
       lv_obj_t *label;
   } satellite;
   
   lv_obj_t *imgSD;
   lv_obj_t *labelClock;
   lv_obj_t *labelRec;

    /* 电池 */
   struct 
   {
       lv_obj_t *img;
       lv_obj_t *objUsage;
       lv_obj_t *label;
   } battery;
} ui;

static int onEvent(Account *account, Account::EventParam_t *param)
{
    if (param->event != Account::EVENT_NOTIFY)
    {
        return Account::ERROR_UNSUPPORTED_REQUEST;
    }

    if (param->size != sizeof(DataProc::StatusBar_Info_t))
    {
        return Account::ERROR_SIZE_MISMATCH;
    }

    DataProc::StatusBar_Info_t *info = (DataProc::StatusBar_Info_t *)param->data_p;

    if (info->showLabelRec)
    {
        lv_obj_clear_flag(ui.labelRec, LV_OBJ_FLAG_HIDDEN);
        const char* str = info->labelRecStr;
        
        if (str)
        {
            lv_label_set_text(ui.labelRec, str);
        }
    }
    else
    {
        lv_obj_add_flag(ui.labelRec, LV_OBJ_FLAG_HIDDEN);
    }
    
    return 0;
}

static void StatusBar_ConBattSetOpa(lv_obj_t* obj, int32_t opa)
{
    lv_obj_set_style_opa(obj, opa, 0);
}

static void StatusBar_onAnimOpaFinish(lv_anim_t* a)
{
    lv_obj_t* obj = (lv_obj_t*)a->var;
    StatusBar_ConBattSetOpa(obj, LV_OPA_COVER);
    StatusBar_AnimCreate(obj);
}

static void StatusBar_onAnimHeightFinish(lv_anim_t* a)
{
    lv_anim_t a_opa;
    lv_anim_init(&a_opa);
    lv_anim_set_var(&a_opa, a->var);
    lv_anim_set_exec_cb(&a_opa, (lv_anim_exec_xcb_t)StatusBar_ConBattSetOpa);
    lv_anim_set_ready_cb(&a_opa, StatusBar_onAnimOpaFinish);
    lv_anim_set_values(&a_opa, LV_OPA_COVER, LV_OPA_TRANSP);
    lv_anim_set_early_apply(&a_opa, true);
    lv_anim_set_delay(&a_opa, 500);
    lv_anim_set_time(&a_opa, 500);
    lv_anim_start(&a_opa);
}

static void StatusBar_AnimCreate(lv_obj_t* contBatt)
{
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, contBatt);
    lv_anim_set_exec_cb(&a, [](void* var, int32_t v) {
        lv_obj_set_height((lv_obj_t*)var, v);
    });
    lv_anim_set_values(&a, 0, BATT_USAGE_HEIGHT);
    lv_anim_set_time(&a, 1000);
    lv_anim_set_ready_cb(&a, StatusBar_onAnimHeightFinish);
    lv_anim_start(&a);
}

/*****************************************************************************
* @brief  更新状态栏
* ex:
* @par
* None
* @retval
*****************************************************************************/
static void StatusBar_Update(lv_timer_t *timer)
{
    /* satellite */
    HAL::GPS_Info_t gps;
    actStatusBar->Pull("GPS", &gps, sizeof(gps));
    lv_label_set_text_fmt(ui.satellite.label, "%d", gps.satellites);

    DataProc::Storage_Basic_Info_t sdInfo;
    actStatusBar->Pull("Storage", &sdInfo, sizeof(sdInfo));
    sdInfo.isDetect ? lv_obj_clear_flag(ui.imgSD, LV_OBJ_FLAG_HIDDEN) : lv_obj_add_flag(ui.imgSD, LV_OBJ_FLAG_HIDDEN);

    /* clock */
    HAL::Clock_Info_t clock;
    actStatusBar->Pull("Clock", &clock, sizeof(clock));
    lv_label_set_text_fmt(ui.labelClock, "%02d:%02d", clock.hour, clock.minute);

    /* battery */
    HAL::Power_Info_t power;
    actStatusBar->Pull("Power", &power, sizeof(power));
    lv_label_set_text_fmt(ui.battery.label, "%d", power.usage);

    bool Is_BattCharging = power.isCharging;
    lv_obj_t* contBatt = ui.battery.objUsage;
    static bool Is_BattChargingAnimActive = false;
    if(Is_BattCharging)
    {
        if(!Is_BattChargingAnimActive)
        {
            StatusBar_AnimCreate(contBatt);
            Is_BattChargingAnimActive = true;
        }
    }
    else
    {
        if(Is_BattChargingAnimActive)
        {
            lv_anim_del(contBatt, nullptr);
            StatusBar_ConBattSetOpa(contBatt, LV_OPA_COVER);
            Is_BattChargingAnimActive = false;
        }
        lv_coord_t height = lv_map(power.usage, 0, 100, 0, BATT_USAGE_HEIGHT);
        lv_obj_set_height(contBatt, height);
    }
}

/*****************************************************************************
* @brief  创建状态栏
* ex:
* @par
* None
* @retval
*****************************************************************************/
static lv_obj_t* StatusBar_Create(lv_obj_t* par)
{
    lv_obj_t *cont = lv_obj_create(par);
    lv_obj_remove_style_all(cont);

    lv_obj_set_size(cont, LV_HOR_RES, STATUS_BAR_HEIGHT);
    lv_obj_set_y(cont, -STATUS_BAR_HEIGHT); /* 先不让状态栏显示出来，待会用动画的方式从上向下滑出 */

    /* style1 */
    lv_obj_set_style_bg_opa(cont, LV_OPA_TRANSP, LV_STATE_DEFAULT); /* 设置不透明度 */
    lv_obj_set_style_bg_color(cont, lv_color_hex(0x33333), LV_STATE_DEFAULT);

    /* style2 */
    lv_obj_set_style_bg_opa(cont, LV_OPA_60, LV_STATE_USER_1);
    lv_obj_set_style_bg_color(cont, lv_color_black(), LV_STATE_USER_1);
    lv_obj_set_style_shadow_color(cont, lv_color_black(), LV_STATE_USER_1); /* 设置影子颜色 */
    lv_obj_set_style_shadow_width(cont, 10, LV_STATE_USER_1);               /* 设置影子宽度 */

    static lv_style_transition_dsc_t tran;  /* 风格转换描述符 */
    static const lv_style_prop_t prop[] = 
    {
        LV_STYLE_BG_COLOR,
        LV_STYLE_OPA,
        LV_STYLE_PROP_INV
    };
    lv_style_transition_dsc_init(&tran, prop, lv_anim_path_ease_out, 200, 0, nullptr);
    lv_obj_set_style_transition(cont, &tran, LV_STATE_USER_1);

    ui.cont = cont;

    static lv_style_t style;
    lv_style_init(&style);
    lv_style_set_text_color(&style, lv_color_white());
    lv_style_set_text_font(&style, Resource.GetFont("bahnschrift_13"));

    /* satellite 卫星 */
    lv_obj_t *img = lv_img_create(cont);
    lv_img_set_src(img, Resource.GetImage("satellite"));
    lv_obj_align(img, LV_ALIGN_LEFT_MID, 14, 0);
    ui.satellite.img = img;

    lv_obj_t *label = lv_label_create(cont);
    lv_obj_add_style(label, &style, 0);
    lv_obj_align_to(label, ui.satellite.img, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_label_set_text(label, "0");  /* 卫星数量 */
    ui.satellite.label = label;

    /* sd card */
    img = lv_img_create(cont);
    lv_img_set_src(img, Resource.GetImage("sd_card"));\
    lv_obj_align(img, LV_ALIGN_LEFT_MID, 50, -1);
    lv_obj_add_flag(img, LV_OBJ_FLAG_HIDDEN);
    ui.imgSD = img;

    /* clock */
    label = lv_label_create(cont);
    lv_obj_add_style(label, &style, 0);
    lv_label_set_text(label, "00:00");
    lv_obj_center(label);
    ui.labelClock = label;

    /* recorder 记录器 */
    label = lv_label_create(cont);
    lv_obj_add_style(label, &style, 0);
    lv_obj_align(label, LV_ALIGN_RIGHT_MID, -50, 0);
    lv_label_set_text(label, "");
    lv_obj_add_flag(label, LV_OBJ_FLAG_HIDDEN);
    ui.labelRec = label;

    /* battery */
    img = lv_img_create(cont);
    lv_img_set_src(img, Resource.GetImage("battery"));
    lv_obj_align(img, LV_ALIGN_RIGHT_MID, -30, 0);
    lv_img_t *img_ext = (lv_img_t *)img;
    lv_obj_set_size(img, img_ext->w, img_ext->h);
    ui.battery.img = img;

    lv_obj_t *obj = lv_obj_create(img);
    lv_obj_remove_style_all(obj);
    lv_obj_set_style_bg_color(obj, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_style_opa(obj, LV_OPA_COVER, 0);
    lv_obj_set_size(obj, BATT_USAGE_WIDTH, BATT_USAGE_HEIGHT);
    lv_obj_align(obj, LV_ALIGN_BOTTOM_MID, 0, -2);
    ui.battery.objUsage = obj;

    label = lv_label_create(cont);
    lv_obj_add_style(label, &style, 0);
    lv_obj_align_to(label, ui.battery.img, LV_ALIGN_OUT_RIGHT_MID, 5, 0);
    lv_label_set_text(label, "100%");
    ui.battery.label = label;

    StatusBar::SetStyle(StatusBar::STYLE_TRANSP);

    lv_timer_t *timer = lv_timer_create(StatusBar_Update, 1000, nullptr);
    lv_timer_ready(timer);

    return ui.cont;
}

/*****************************************************************************
* @brief  状态栏初始化
* ex:
* @par
* None
* @retval
*****************************************************************************/
void StatusBar::Init(lv_obj_t* par)
{
    StatusBar_Create(par);
}

/*****************************************************************************
* @brief  
* ex:
* @par
* None
* @retval
*****************************************************************************/
void StatusBar::SetStyle(Style_t style)
{
    lv_obj_t *cont = ui.cont;
    if (style == STYLE_TRANSP)
    {
        lv_obj_add_state(cont, LV_STATE_DEFAULT);
        lv_obj_clear_state(cont, LV_STATE_USER_1);
    }
    else if (style == STYLE_BLACK)
    {
        lv_obj_add_state(cont, LV_STATE_USER_1);
    }
    else
    {
        return;
    }
}

/*****************************************************************************
* @brief  
* ex:
* @par
* None
* @retval
*****************************************************************************/
void StatusBar::Appear(bool en)
{
#define ANIM_DEF(start_time, obj, attr, start, end) \
    {start_time, obj, LV_ANIM_EXEC(attr), start, end, 500, lv_anim_path_ease_out, true}

    lv_anim_timeline_t anim_timeline[] =
    {
        ANIM_DEF(1000, ui.cont, y, -STATUS_BAR_HEIGHT, 0),
        LV_ANIM_TIMELINE_END
    };

    lv_anim_timeline_start(anim_timeline, !en);
}

DATA_PROC_INIT_DEF(StatusBar)
{
    account->Subscribe("GPS");
    account->Subscribe("Power");
    account->Subscribe("Clock");
    account->Subscribe("Storage");
    account->SetEventCallback(onEvent);

    actStatusBar = account;
}
