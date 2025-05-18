#include "bottle_hud.h"
#include "mask_l_button.h"

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

#define ICON_IMG_SIZE 32
#define ICON_SIZE 16
#define ICON_DIST 14
#define ICON_POS_X 24
#define ICON_POS_Y 187

#define ICON_DSDX (s32)(1024.0f * (float)(ICON_IMG_SIZE) / (ICON_SIZE))
#define ICON_DTDY (s32)(1024.0f * (float)(ICON_IMG_SIZE) / (ICON_SIZE))


bool bottle_item_icons_loaded = false;
u8 bottle_item_textures[NUMBER_BOTTLE_ITEMS][ICON_IMG_SIZE * ICON_IMG_SIZE * 4] __attribute__((aligned(8)));
u8 extra_item_slot_statuses[NUMBER_BOTTLE_ITEMS];
s16 extra_item_slot_alpha = 0b011111111111111;
ItemId bottle_items[NUMBER_BOTTLE_ITEMS] = {
    /* 0x12 */ ITEM_BOTTLE,
    /* 0x13 */ ITEM_POTION_RED,
    /* 0x14 */ ITEM_POTION_GREEN,
    /* 0x15 */ ITEM_POTION_BLUE,
    /* 0x16 */ ITEM_FAIRY,
    /* 0x17 */ ITEM_DEKU_PRINCESS,
    /* 0x18 */ ITEM_MILK_BOTTLE,
    /* 0x19 */ ITEM_MILK_HALF,
    /* 0x1A */ ITEM_FISH,
    /* 0x1B */ ITEM_BUG,
    /* 0x1C */ ITEM_BLUE_FIRE,
    /* 0x1D */ ITEM_POE,
    /* 0x1E */ ITEM_BIG_POE,
    /* 0x1F */ ITEM_SPRING_WATER,
    /* 0x20 */ ITEM_HOT_SPRING_WATER,
    /* 0x21 */ ITEM_ZORA_EGG,
    /* 0x22 */ ITEM_GOLD_DUST,
    /* 0x23 */ ITEM_MUSHROOM,
    /* 0x24 */ ITEM_SEAHORSE,
    /* 0x25 */ ITEM_CHATEAU,
    /* 0x26 */ ITEM_HYLIAN_LOACH,
    /* 0x27 */ ITEM_OBABA_DRINK,
};

int bottle_icon_index = 0;

RECOMP_HOOK("Interface_DrawCButtonIcons") void draw_dpad_icons(PlayState* play) {
    // Just in case:

    if (BtnStateL.rel) {
        if (++bottle_icon_index == NUMBER_BOTTLE_ITEMS) {
            bottle_icon_index = 0;
        }
    }

    PauseContext* pauseCtx = &play->pauseCtx;
    if (pauseCtx->state == PAUSE_STATE_OFF) {
        if (!bottle_item_icons_loaded) {
            for (int i = 0; i < NUMBER_BOTTLE_ITEMS; i++) {
                CmpDma_LoadFile(SEGMENT_ROM_START(icon_item_static_yar), bottle_items[i], bottle_item_textures[i], sizeof(bottle_item_textures[i]));
            }
            recomp_printf("Bottle Icons Loaded\n");
            bottle_item_icons_loaded = true;
        }

        OPEN_DISPS(play->state.gfxCtx);

        gEXForceUpscale2D(OVERLAY_DISP++, 1);
        // Set a fullscreen scissor.
        gEXPushScissor(OVERLAY_DISP++);
        gEXSetScissor(OVERLAY_DISP++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0, SCREEN_HEIGHT);

        gDPLoadTextureBlock(OVERLAY_DISP++, bottle_item_textures[bottle_icon_index], G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
                            G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, extra_item_slot_alpha);
        gEXTextureRectangle(OVERLAY_DISP++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_LEFT,
            (ICON_POS_X - (ICON_SIZE/2)) * 4, (ICON_POS_Y - (ICON_SIZE/2)) * 4,
            (ICON_POS_X + (ICON_SIZE/2)) * 4, (ICON_POS_Y + (ICON_SIZE/2)) * 4,
            0,
            0, 0,
            ICON_DSDX, ICON_DTDY);

        gEXForceUpscale2D(OVERLAY_DISP++, 0);
        // Restore the previous scissor.
        gEXPopScissor(OVERLAY_DISP++);

        CLOSE_DISPS(play->state.gfxCtx);
    }
}