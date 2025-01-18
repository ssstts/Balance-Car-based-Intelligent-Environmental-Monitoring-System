#include "lm1204.h"

uint16_t LM1204_ADC_Read(void)
{
	return AD_Value[0];
}


// 计算分贝值
float calculate_dB(float voltage) {
    if (voltage <= 0.0f) {
        return -INFINITY;  // 防止 log10(0) 的错误，返回负无穷
    }
    return 20.0f * log10(voltage / V_REF);
}


