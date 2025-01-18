#ifndef __LM1204_H_
#define __LM1204_H_
#include "stm32f10x.h"                  // Device header
#include "math.h"
#include "AD.h"

#define V_REF              3.3      // Reference voltage (3.3V)

uint16_t LM1204_ADC_Read(void);
float calculate_dB(float voltage);

#endif
