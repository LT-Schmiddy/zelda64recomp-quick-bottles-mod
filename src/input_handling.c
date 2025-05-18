#include "input_handling.h"
#include "recomputils.h"
#include "recompconfig.h"

BtnState BtnStateL;

RECOMP_HOOK("Player_UpdateCommon") void pre_Player_UpdateCommon(Player* this, PlayState* play, Input* input) {
    // Kafei Prevention:
    if (this->actor.id != ACTOR_PLAYER) {
        return;
    }
    
    // Capture input:
    BtnStateL.cur = input->cur.button & BTN_L;
    BtnStateL.prev = input->prev.button & BTN_L;
    BtnStateL.press = input->press.button & BTN_L;
    BtnStateL.rel = input->rel.button & BTN_L;

    if (BtnStateL.press) {
        recomp_printf("L Pressed\n");
    }
}

// Prevent minimap toggle while aiming if L config selected
RECOMP_HOOK("MapDisp_Update")
void pre_MapDisp_Update(PlayState* play) {
    if (BtnStateL.press) {
        AudioSfx_StopById(NA_SE_SY_CAMERA_ZOOM_UP);
        AudioSfx_StopById(NA_SE_SY_CAMERA_ZOOM_DOWN);
    }

    R_MINIMAP_DISABLED = (u8)recomp_get_config_u32("minimap-visible");
}