#include "app_soft_pwm.h"

volatile u16 g_pwm_period = 1000;  	//默认周期（假设定时器中断间隔1ms，总周期1秒）
volatile u16 g_pwm_cnt = 0;
volatile u16 g_pwm_high = 500;    	//默认高电平时间（占空比50%）

static void set_pwm_gpio(u8 gpio_state)	//设置用于模拟pwm的gpio电平
{
	if(gpio_state == 1) GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_SET);
	if(gpio_state == 0) GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_RESET);
}

void SoftPwmTimerISR(void)		//用在定时器中断里
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
    //限制最小周期为1，避免除零错误
    if (period == 0) period = 1;

    g_pwm_period = period;
}

void SoftPwmSetHigh(u16 high)
{
    //高电平时间不超过周期
    g_pwm_high = (high > g_pwm_period) ? g_pwm_period : high;
}
