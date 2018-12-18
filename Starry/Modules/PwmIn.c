#include "tim.h"
#include "PwmIn.h"
#include "gpio.h"
#include "Rc.h"

uint16_t receiverPwmIn[8];
static uint16_t channelWatchDogCount[10] ;
uint8_t channelEnableBit = 0;

void pwmInInit( void )
{
	HAL_TIM_IC_Start_IT( &htim3, TIM_CHANNEL_1 );   /* ��ʼ����TIM5��ͨ��1 */
	HAL_TIM_IC_Start_IT( &htim3, TIM_CHANNEL_2 );   /* ��ʼ����TIM5��ͨ��1 */
	HAL_TIM_IC_Start_IT( &htim3, TIM_CHANNEL_3 );   /* ��ʼ����TIM5��ͨ��1 */
	HAL_TIM_IC_Start_IT( &htim3, TIM_CHANNEL_4 );   /* ��ʼ����TIM5��ͨ��1 */
	__HAL_TIM_ENABLE_IT( &htim3, TIM_IT_UPDATE );   /* ʹ�ܸ����ж� */
	HAL_TIM_IC_Start_IT( &htim4, TIM_CHANNEL_1 );   /* ��ʼ����TIM5��ͨ��1 */
	HAL_TIM_IC_Start_IT( &htim4, TIM_CHANNEL_2 );   /* ��ʼ����TIM5��ͨ��1 */
	HAL_TIM_IC_Start_IT( &htim4, TIM_CHANNEL_3 );   /* ��ʼ����TIM5��ͨ��1 */
	HAL_TIM_IC_Start_IT( &htim4, TIM_CHANNEL_4 );   /* ��ʼ����TIM5��ͨ��1 */
	__HAL_TIM_ENABLE_IT( &htim4, TIM_IT_UPDATE );   /* ʹ�ܸ����ж� */
}

void channelWatchDog(uint8_t dT_ms)
{
	for(uint8_t i = 0;i<8;i++)
	{
		if(channelWatchDogCount[i]<500)
		{
			channelWatchDogCount[i] += dT_ms;
			channelEnableBit |= 0x01<<i;
		}
		else
		{
			channelEnableBit &= ~(0x01<<i);
			receiverPwmIn[i] = 0;
		}
	}
}

void channelFeedWatchDog(uint8_t ch_n)
{
	channelWatchDogCount[ch_n] = 0;
}


/* ��ʱ�������жϣ�����������жϴ���ص����� */
void HAL_TIM3_Callback( TIM_HandleTypeDef *htim )       /* �����жϣ����������ʱִ�� */
{
	static uint16_t temp_cnt1, temp_cnt1_2, temp_cnt2, temp_cnt2_2, temp_cnt3, temp_cnt3_2, temp_cnt4, temp_cnt4_2;
	if ( htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1 )
	{
                channelFeedWatchDog(CH1);//ͨ�����ι��
		htim->Instance->SR	= ~TIM_IT_CC1;  /* TIM_ClearITPendingBit(TIM3, TIM_IT_CC1); */
		htim->Instance->SR	= ~TIM_FLAG_CC1OF;
		if ( GPIOC->IDR & GPIO_PIN_6 )
		{
			temp_cnt1 = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_1 );
		}else  {
			temp_cnt1_2 = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_1 );
			if ( temp_cnt1_2 >= temp_cnt1 )
				receiverPwmIn[0] = temp_cnt1_2 - temp_cnt1;
			else
				receiverPwmIn[0] = 0xffff - temp_cnt1 + temp_cnt1_2 + 1;
		}
	}
	if ( htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2 )
	{
                channelFeedWatchDog(CH2);//ͨ�����ι��
		htim->Instance->SR	= ~TIM_IT_CC2;
		htim->Instance->SR	= ~TIM_FLAG_CC2OF;
		if ( GPIOC->IDR & GPIO_PIN_7 )
		{
			temp_cnt2 = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_2 );
		}else  {
			temp_cnt2_2 = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_2 );
			if ( temp_cnt2_2 >= temp_cnt2 )
				receiverPwmIn[1] = temp_cnt2_2 - temp_cnt2;
			else
				receiverPwmIn[1] = 0xffff - temp_cnt2 + temp_cnt2_2 + 1;
		}
	}
	if ( htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3 )
	{
                channelFeedWatchDog(CH3);//ͨ�����ι��
		htim->Instance->SR	= ~TIM_IT_CC3;
		htim->Instance->SR	= ~TIM_FLAG_CC3OF;
		if ( GPIOB->IDR & GPIO_PIN_0 )
		{
			temp_cnt3 = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_3 );
		}else  {
			temp_cnt3_2 = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_3 );
			if ( temp_cnt3_2 >= temp_cnt3 )
				receiverPwmIn[2] = temp_cnt3_2 - temp_cnt3;
			else
				receiverPwmIn[2] = 0xffff - temp_cnt3 + temp_cnt3_2 + 1;
		}
	}
	if ( htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4 )
	{
                channelFeedWatchDog(CH4);//ͨ�����ι��
		htim->Instance->SR	= ~TIM_IT_CC4;
		htim->Instance->SR	= ~TIM_FLAG_CC4OF;
		if ( GPIOB->IDR & GPIO_PIN_1 )
		{
			temp_cnt4 = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_4 );
		}else  {
			temp_cnt4_2 = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_4 );
			if ( temp_cnt4_2 >= temp_cnt4 )
				receiverPwmIn[3] = temp_cnt4_2 - temp_cnt4;
			else
				receiverPwmIn[3] = 0xffff - temp_cnt4 + temp_cnt4_2 + 1;
		}
	}
}


void HAL_TIM4_Callback( TIM_HandleTypeDef *htim )       /* �����жϣ����������ʱִ�� */
{
	static uint16_t temp_cnt1, temp_cnt1_2, temp_cnt2, temp_cnt2_2, temp_cnt3, temp_cnt3_2, temp_cnt4, temp_cnt4_2;
	if ( htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1 )
	{
                channelFeedWatchDog(CH5);//ͨ�����ι��
		htim->Instance->SR	= ~TIM_IT_CC1;  /* TIM_ClearITPendingBit(TIM3, TIM_IT_CC1); */
		htim->Instance->SR	= ~TIM_FLAG_CC1OF;
		if ( GPIOD->IDR & GPIO_PIN_12 )
		{
			temp_cnt1 = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_1 );
		}else  {
			temp_cnt1_2 = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_1 );
			if ( temp_cnt1_2 >= temp_cnt1 )
				receiverPwmIn[4] = temp_cnt1_2 - temp_cnt1;
			else
				receiverPwmIn[4] = 0xffff - temp_cnt1 + temp_cnt1_2 + 1;
		}
	}
	if ( htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2 )
	{
		channelFeedWatchDog(CH6);//ͨ�����ι��
                htim->Instance->SR	= ~TIM_IT_CC2;
		htim->Instance->SR	= ~TIM_FLAG_CC2OF;
		if ( GPIOD->IDR & GPIO_PIN_13 )
		{
			temp_cnt2 = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_2 );
		}else  {
			temp_cnt2_2 = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_2 );
			if ( temp_cnt2_2 >= temp_cnt2 )
				receiverPwmIn[5] = temp_cnt2_2 - temp_cnt2;
			else
				receiverPwmIn[5] = 0xffff - temp_cnt2 + temp_cnt2_2 + 1;
		}
	}
	if ( htim->Channel == HAL_TIM_ACTIVE_CHANNEL_3 )
	{
                channelFeedWatchDog(CH7);//ͨ�����ι��
		htim->Instance->SR	= ~TIM_IT_CC3;
		htim->Instance->SR	= ~TIM_FLAG_CC3OF;
		if ( GPIOD->IDR & GPIO_PIN_14 )
		{
			temp_cnt3 = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_3 );
		}else  {
			temp_cnt3_2 = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_3 );
			if ( temp_cnt3_2 >= temp_cnt3 )
				receiverPwmIn[6] = temp_cnt3_2 - temp_cnt3;
			else
				receiverPwmIn[6] = 0xffff - temp_cnt3 + temp_cnt3_2 + 1;
		}
	}
	if ( htim->Channel == HAL_TIM_ACTIVE_CHANNEL_4 )
	{
                channelFeedWatchDog(CH8);//ͨ�����ι��
		htim->Instance->SR	= ~TIM_IT_CC4;
		htim->Instance->SR	= ~TIM_FLAG_CC4OF;
		if ( GPIOD->IDR & GPIO_PIN_15 )
		{
			temp_cnt4 = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_4 );
		}else  {
			temp_cnt4_2 = HAL_TIM_ReadCapturedValue( htim, TIM_CHANNEL_4 );
			if ( temp_cnt4_2 >= temp_cnt4 )
				receiverPwmIn[7] = temp_cnt4_2 - temp_cnt4;
			else
				receiverPwmIn[7] = 0xffff - temp_cnt4 + temp_cnt4_2 + 1;
		}
	}
}

uint16_t getDutyFromPwmIn(uint8_t ch)
{
	return receiverPwmIn[ch];
}

/* ��ʱ�����벶���жϴ���ص��������ú�����HAL_TIM_IRQHandler�лᱻ���� */
void HAL_TIM_IC_CaptureCallback( TIM_HandleTypeDef *htim ) /* �����жϷ���ʱִ�� */
{
	if ( htim->Instance == TIM3 )
	{
		HAL_TIM3_Callback( htim );
		RcDataUpdate();
	}
	if ( htim->Instance == TIM4 )
	{
		HAL_TIM4_Callback( htim );
		RcDataUpdate();
	}
}
