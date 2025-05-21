#ifndef __QUICK_BOTTLE__
#define __QUICK_BOTTLE__

#include "modding.h"
#include "global.h"

#define MAX_BOTTLE_INDEX 5
#define NUMBER_BOTTLE_ITEMS 22
#define FIRST_BOTTLE_INVENTORY_SLOT 18
#define BOTTLE_QUICK_PRESS_TIME 10
#define BOTTLE_POST_RELEASE_TIME 20
#define BOTTLE_AUTO_PUT_AWAY_TIME 3

typedef struct {
    s32 bottleIndex;
    bool triggered;
    u8 quick_press_timer;
    u8 post_release_timer;
    u8 auto_put_away_timer;
    u8 numberOfBottles;
} QuickBottleController; 

extern ItemId bottle_items[NUMBER_BOTTLE_ITEMS];
extern QuickBottleController quickBottle;

int QuickBottle_GetSelectedInventorySlot();
ItemId QuickBottle_GetBottleId(int index);
ItemId QuickBottle_GetSelectedBottleId();
bool QuickBottle_IsValidBottleItem(ItemId item);
int QuickBottle_GetNumberOfBottles();

#endif