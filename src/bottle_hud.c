#include "bottle_hud.h"
#include "input_handling.h"
#include "quick_bottle.h"

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

extern u8 gEquippedItemOutlineTex[];

static bool bottle_item_icons_loaded = false;
static u8 bottle_item_textures[NUMBER_BOTTLE_ITEMS][ICON_IMG_SIZE * ICON_IMG_SIZE * 4] __attribute__((aligned(8)));
static u8 extra_item_slot_statuses[NUMBER_BOTTLE_ITEMS];
static s16 extra_item_slot_alpha = 0b011111111111111;

int GetBottleIconIndex() {
    ItemId bottle = gSaveContext.save.saveInfo.inventory.items[FIRST_BOTTLE_INVENTORY_SLOT + quickBottle.bottleIndex];
    return bottle - 0x12;
}

RECOMP_HOOK("Interface_DrawCButtonIcons") void DrawBottleIcon(PlayState* play) {
    // Just in case:

    // if (BtnStateL.rel) {
    //     if (++bottle_icon_index == NUMBER_BOTTLE_ITEMS) {
    //         bottle_icon_index = 0;
    //     }
    // }

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

        // gDPLoadTextureBlock(OVERLAY_DISP++, gEquippedItemOutlineTex, G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
        //                     G_TX_NOMIRROR | G_TX_WRAP, G_TX_NOMASK, G_TX_NOMASK, G_TX_NOLOD, G_TX_NOLOD);
        // gDPSetPrimColor(OVERLAY_DISP++, 0, 0, 255, 255, 255, extra_item_slot_alpha);
        // gEXTextureRectangle(OVERLAY_DISP++, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_LEFT,
        //     (ICON_POS_X - (ICON_SIZE/2)) * 4, (ICON_POS_Y - (ICON_SIZE/2)) * 4,
        //     (ICON_POS_X + (ICON_SIZE/2)) * 4, (ICON_POS_Y + (ICON_SIZE/2)) * 4,
        //     0,
        //     0, 0,
        //     ICON_DSDX, ICON_DTDY);

        gEXSetScissor(OVERLAY_DISP++, G_SC_NON_INTERLACE, G_EX_ORIGIN_LEFT, G_EX_ORIGIN_RIGHT, 0, 0, 0, SCREEN_HEIGHT);

        gDPLoadTextureBlock(OVERLAY_DISP++, bottle_item_textures[GetBottleIconIndex()], G_IM_FMT_RGBA, G_IM_SIZ_32b, 32, 32, 0, G_TX_NOMIRROR | G_TX_WRAP,
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