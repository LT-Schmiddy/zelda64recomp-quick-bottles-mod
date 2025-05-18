#ifndef __MASK_L_BUTTON__
#define __MASK_L_BUTTON__

#include "modding.h"
#include "global.h"

typedef struct {
    bool cur;
    bool prev;
    bool press;
    bool rel;
} BtnState;
extern BtnState BtnStateL;

#endif