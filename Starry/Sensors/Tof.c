#include "Tof.h"
#include "Vl53l0X.h"
#include "board.h"

typedef struct {
    int32_t alt;
    int32_t lastAlt;
    float   velocity;
    int32_t alt_offset;
    float temperature;
} TOF_t;

TOF_t tof;

/**********************************************************************************************************
*�� �� ��: BaroDataPreTreat
*����˵��: ��ѹ�߶�����Ԥ����
*��    ��: ��
*�� �� ֵ: ��
**********************************************************************************************************/
void TofDataPreTreat(void)
{
    static uint64_t lastTime = 0;
    uint16_t tofAltTemp;
		float tofAlt;

    float deltaT = (getSysTimeUs() - lastTime) * 1e-6;
    lastTime = getSysTimeUs();

    //��ȡTOF�߶�
		vl53l0xRead(&tofAltTemp);
		tofAlt = 0.1f * tofAltTemp;

    //��ѹ�߶ȵ�ͨ�˲�
    tof.alt = tof.alt * 0.5f + tofAlt * 0.5f;

    //����TOF�仯�ٶȣ������е�ͨ�˲�
    tof.velocity = tof.velocity * 0.65f + ((tof.alt - tof.lastAlt) / deltaT) * 0.35f;
    tof.lastAlt = tof.alt;
}

/**********************************************************************************************************
*�� �� ��: TofGetAlt
*����˵��: ��ȡTOF�߶�����
*��    ��: ��
*�� �� ֵ: ��ѹ�߶�
**********************************************************************************************************/
int32_t TofGetAlt(void)
{
    return tof.alt;
}

/**********************************************************************************************************
*�� �� ��: TofGetVelocity
*����˵��: ��ȡTOF�߶ȱ仯�ٶ�
*��    ��: ��
*�� �� ֵ: ��ѹ�߶ȱ仯�ٶ�
**********************************************************************************************************/
float TofGetVelocity(void)
{
    return tof.velocity;
}