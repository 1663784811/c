#ifndef __PAGE_MANAGER_H
#define __PAGE_MANAGER_H

#include "PageBase.h"
#include "PageFactory.h"
#include <vector>
#include <stack>

class PageManager
{
public:

    /* Page switching animation type  */
    typedef enum
    {
        /* Default (global) animation type  */
        LOAD_ANIM_GLOBAL = 0,

        /* New page overwrites old page  */
        LOAD_ANIM_OVER_LEFT,
        LOAD_ANIM_OVER_RIGHT,
        LOAD_ANIM_OVER_TOP,
        LOAD_ANIM_OVER_BOTTOM,

        /* New page pushes old page  */
        LOAD_ANIM_MOVE_LEFT,
        LOAD_ANIM_MOVE_RIGHT,
        LOAD_ANIM_MOVE_TOP,
        LOAD_ANIM_MOVE_BOTTOM,

        /* The new interface fades in, the old page fades out */
        LOAD_ANIM_FADE_ON,

        /* No animation */
        LOAD_ANIM_NONE,

        _LOAD_ANIM_LAST = LOAD_ANIM_NONE
    } LoadAnim_t;

    /* Page dragging direction */
    typedef enum
    {
        ROOT_DRAG_DIR_NONE,
        ROOT_DRAG_DIR_HOR,
        ROOT_DRAG_DIR_VER,
    } RootDragDir_t;

    /* Animated setter  */
    typedef lv_anim_exec_xcb_t lv_anim_setter_t;

    /* Animated getter  */
    typedef lv_coord_t(*lv_anim_getter_t)(void*);

    /* Animation switching record  */
    typedef struct 
    {
        /* As the entered party */
        struct
        {
            lv_coord_t start;
            lv_coord_t end;
        } enter;
        
        /* As the exited party */
        struct
        {
            lv_coord_t start;
            lv_coord_t end;
        } exit;
    } AnimValue_t;
    
    /* Page loading animation properties */
    typedef struct
    {
        lv_anim_setter_t setter;
        lv_anim_getter_t getter;
        RootDragDir_t dragDir;
        AnimValue_t push;
        AnimValue_t pop;
    } LoadAnimAttr_t;

public:
    PageManager(PageFactory* factory = nullptr);
    ~PageManager();

    /* Loader */
    PageBase* Install(const char* className, const char* appName);
    bool Uninstall(const char* appName);
    bool Register(PageBase* base, const char* name);
    bool Unregister(const char* name);

    /* Router */
    PageBase* Push(const char* name, const PageBase::Stash_t* stash = nullptr);
    PageBase* Pop();
    bool BackHome();
    const char* GetPagePrevName();

    /* Global Anim */
    void SetGlobalLoadAnimType(
        LoadAnim_t anim = LOAD_ANIM_OVER_LEFT,
        uint16_t time = 500,
        lv_anim_path_cb_t path = lv_anim_path_ease_out
    );

private:
    /* Page Pool */
    PageBase* FindPageInPool(const char* name);

    /* Page Stack */
    PageBase* FindPageInStack(const char* name);
    PageBase* GetStackTop();
    PageBase* GetStackTopAfter();
    void SetStackClear(bool keepBottom = true);
    bool FourceUnload(PageBase* base);

    /* Anim */
    bool GetLoadAnimAttr(uint8_t anim, LoadAnimAttr_t* attr);
    bool GetIsOverAnim(uint8_t anim)
    {
        return (anim >= LOAD_ANIM_OVER_LEFT && anim <= LOAD_ANIM_OVER_BOTTOM);
    }
    bool GetIsMoveAnim(uint8_t anim)
    {
        return (anim >= LOAD_ANIM_MOVE_LEFT && anim <= LOAD_ANIM_MOVE_BOTTOM);
    }
    void AnimDefaultInit(lv_anim_t* a);
    bool GetCurrentLoadAnimAttr(LoadAnimAttr_t* attr)
    {
        return GetLoadAnimAttr(GetCurrentLoadAnimType(), attr);
    }
    LoadAnim_t GetCurrentLoadAnimType()
    {
        return (LoadAnim_t)AnimState.Current.Type;
    }

    /* Root */
    static void onRootDragEvent(lv_event_t* event);
    static void onRootAnimFinish(lv_anim_t* a);
    static void onRootAsyncLeave(void* base);
    void RootEnableDrag(lv_obj_t* root);
    static void RootGetDragPredict(lv_coord_t* x, lv_coord_t* y);

    /* Switch */
    void SwitchTo(PageBase* base, bool isPushAct, const PageBase::Stash_t* stash = nullptr);
    static void onSwitchAnimFinish(lv_anim_t* a);
    void SwitchAnimCreate(PageBase* base);
    void SwitchAnimTypeUpdate(PageBase* base);
    bool SwitchReqCheck();
    bool SwitchAnimStateCheck();

    /* State */
    PageBase::State_t StateLoadExecute(PageBase* base);
    PageBase::State_t StateWillAppearExecute(PageBase* base);
    PageBase::State_t StateDidAppearExecute(PageBase* base);
    PageBase::State_t StateWillDisappearExecute(PageBase* base);
    PageBase::State_t StateDidDisappearExecute(PageBase* base);
    PageBase::State_t StateUnloadExecute(PageBase* base);
    void StateUpdate(PageBase* base);
    PageBase::State_t GetState()
    {
        return PageCurrent->priv.State;
    }

private:

    /* Page factory */
    PageFactory* Factory;

    /* Page pool */
    std::vector<PageBase*> PagePool;

    /* Page stack */
    std::stack<PageBase*> PageStack;

    /* Previous page */
    PageBase* PagePrev;

    /* The current page */
    PageBase* PageCurrent;

    /* Page animation status */
    struct
    {
        bool IsSwitchReq;              // Whether to switch request
        bool IsBusy;                   // Is switching
        bool IsPushing;                // Whether it is in push state

        PageBase::AnimAttr_t Current;  // Current animation properties
        PageBase::AnimAttr_t Global;   // Global animation properties
    } AnimState;
};


#endif
