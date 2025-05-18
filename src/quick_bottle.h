#ifndef __QUICK_BOTTLE__
#define __QUICK_BOTTLE__

#include "modding.h"
#include "global.h"


#define NUMBER_BOTTLE_ITEMS 22
typedef struct {
    u8 bottle_index;
    bool triggered;
} QuickBottleController; 
extern ItemId bottle_items[NUMBER_BOTTLE_ITEMS];
extern QuickBottleController quickBottle;

#endif