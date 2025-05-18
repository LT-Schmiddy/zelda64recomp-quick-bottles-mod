#include "input_handling.h"
#include "recomputils.h"
#include "recompconfig.h"

#include "sys_cmpdma.h"
#include "rt64_extended_gbi.h"
#include "recompconfig.h"
#include "recomputils.h"
#include "z64item.h"
#include "z64interface.h"
#include "kaleido_manager.h"
#include "macros.h"
#include "variables.h"
#include "functions.h"
#include "z64player.h"

BtnState BtnStateL;
BtnState BtnStateDRight;
BtnState BtnStateDLeft;

void BtnState_Record( Input* input, BtnState* state, u16 btn, bool should_mask) {
    state->cur = input->cur.button & btn;
    state->prev = input->prev.button & btn;
    state->press = input->press.button & btn;
    state->rel = input->rel.button & btn;

    if (should_mask) {
        input->cur.button &= ~btn;
        input->prev.button &= ~btn;
        input->press.button &= ~btn;
        input->rel.button &= ~btn;
    }
}


RECOMP_HOOK("Player_UpdateCommon") void pre_Player_UpdateCommon(Player* this, PlayState* play, Input* input) {
    // Kafei Prevention:
    if (this->actor.id != ACTOR_PLAYER) {
        return;
    }
    
    // Capture input:
    BtnState_Record(input, &BtnStateL, BTN_L, false);
    BtnState_Record(input, &BtnStateDRight, BTN_DRIGHT, BtnStateL.cur);
    BtnState_Record(input, &BtnStateDLeft, BTN_DLEFT, BtnStateL.cur);

    // if (BtnStateL.press) {
    //     recomp_printf("L Pressed\n");
    // }
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


