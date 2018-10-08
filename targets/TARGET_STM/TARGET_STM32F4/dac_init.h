#pragma once

#include "cmsis.h"

void __DAC_Init(void);

extern TIM_HandleTypeDef htim7;
extern DAC_HandleTypeDef hdac;