#include "quick_bottle.h"
#include "bottle_hud.h"
#include "input_handling.h"

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

QuickBottleController quickBottle;

void Player_UseItem(PlayState* play, Player* this, ItemId item);
void Interface_StartBottleTimer(s16 seconds, s16 timerId);

static bool skip_regular_processing = false;
static Player* captured_player = NULL;
static u32 old_equip;


RECOMP_HOOK("Player_ProcessItemButtons") void pre_Player_ProcessItemButtons(Player* this, PlayState* play) {
    captured_player = this;
    if (BtnStateL.press) {
        // Message_StartTextbox(play, 0x0002, NULL);

        Player_UseItem(play, this, INV_CONTENT(ITEM_POTION_RED));


        skip_regular_processing = true;
        this->stateFlags1 |= PLAYER_STATE1_20000000;

    } else if (BtnStateL.rel) {
    }
}

RECOMP_HOOK_RETURN("Player_ProcessItemButtons") void post_Player_ProcessItemButtons() {
    if (skip_regular_processing) {
        captured_player->stateFlags1 &= ~PLAYER_STATE1_20000000;
        
        skip_regular_processing = false;
    }
}

RECOMP_PATCH void Inventory_UpdateBottleItem(PlayState* play, u8 item, u8 btn) {
    // gSaveContext.save.saveInfo.inventory.items[GET_CUR_FORM_BTN_SLOT(btn)] = item;
    // SET_CUR_FORM_BTN_ITEM(btn, item);

    Interface_LoadItemIconImpl(play, btn);

    play->pauseCtx.cursorItem[PAUSE_ITEM] = item;
    gSaveContext.buttonStatus[btn] = BTN_ENABLED;

    if (item == ITEM_HOT_SPRING_WATER) {
        Interface_StartBottleTimer(60, GET_CUR_FORM_BTN_SLOT(btn) - SLOT_BOTTLE_1);
    }
}