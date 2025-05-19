#ifndef __QUICK_BOTTLE__
#define __QUICK_BOTTLE__

#include "modding.h"
#include "global.h"

#define MAX_BOTTLE_INDEX 5
#define NUMBER_BOTTLE_ITEMS 22
#define FIRST_BOTTLE_INVENTORY_SLOT 18
#define BOTTLE_QUICK_PRESS_TIME 10

typedef struct {
    s8 bottleIndex;
    bool triggered;
    u8 quick_press_timer;
} QuickBottleController; 

extern ItemId bottle_items[NUMBER_BOTTLE_ITEMS];
extern QuickBottleController quickBottle;

int QuickBottle_GetSelectedInventorySlot();
ItemId QuickBottle_GetBottleId(int index);
ItemId QuickBottle_GetSelectedBottleId();
int QuickBottle_GetNumberOfBottles();

#endif