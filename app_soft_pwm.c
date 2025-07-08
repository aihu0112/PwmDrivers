#include "app_soft_pwm.h"

volatile u16 g_pwm_period = 1000;  	//Ĭ�����ڣ����趨ʱ���жϼ��1ms��������1�룩
volatile u16 g_pwm_cnt = 0;
volatile u16 g_pwm_high = 500;    	//Ĭ�ϸߵ�ƽʱ�䣨ռ�ձ�50%��

static void set_pwm_gpio(u8 gpio_state)	//��������ģ��pwm��gpio��ƽ
{
	if(gpio_state == 1) GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_SET);
	if(gpio_state == 0) GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_RESET);
}

void SoftPwmTimerISR(void)		//���ڶ�ʱ���ж���
{
    g_pwm_cnt++;

    if(g_pwm_cnt <= g_pwm_high)
	{
		set_pwm_gpio(1);
    }
	else
	{
		set_pwm_gpio(0);
    }

	if(g_pwm_cnt >= g_pwm_period)
	{
		g_pwm_cnt = 0;
	}
}

void SoftPwmSetPeriod(u16 period)
{
    //������С����Ϊ1������������
    if (period == 0) period = 1;

    g_pwm_period = period;
}

void SoftPwmSetHigh(u16 high)
{
    //�ߵ�ƽʱ�䲻��������
    g_pwm_high = (high > g_pwm_period) ? g_pwm_period : high;
}
