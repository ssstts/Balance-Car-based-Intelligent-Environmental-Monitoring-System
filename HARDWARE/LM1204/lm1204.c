#include "lm1204.h"

uint16_t LM1204_ADC_Read(void)
{
	return AD_Value[0];
}


// ����ֱ�ֵ
float calculate_dB(float voltage) {
    if (voltage <= 0.0f) {
        return -INFINITY;  // ��ֹ log10(0) �Ĵ��󣬷��ظ�����
    }
    return 20.0f * log10(voltage / V_REF);
}


