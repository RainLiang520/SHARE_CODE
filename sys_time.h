#ifndef _SYS_TIME_H
#define _SYS_TIME_H

#include "Common.h"

#define BIN_UP 1
#define BIN_DOWN 0
#define Ture 1
#define Faile 0


static uint32_t time1_ms =0;

void sys_tim_Init(void);
uint32_t TimeGetTime(void);
uint32_t Tim1GetDuration(uint32_t last);
void BoxStates(void);
uint8_t ButtonState(void);
#endif