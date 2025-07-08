# soft_pwm

# 为什么要软件模拟PWM

通常我们使用PWM都是通过MCU的定时器比较功能或者自带的PWM外设来实现的，可以达到很高的频率和精度；但是最近做了一个温控项目，需要控制的设备温度变化非常缓慢，且控制开关用到了可控硅，这就导致PWM频率太高的话关断时间无法保证，实际使用时PWM的频率为1Hz就能满足需求了，这时候直接使用软件模拟PWM就显得很合适，MCU不一定支持这么低频的PWM以及不浪费资源了。

# 软件模拟PWM代码解析

![在这里插入图片描述](https://i-blog.csdnimg.cn/direct/720129dd376247efb09ea24b7574f710.png#pic_center)

- PWM输出引脚配置，根据使用MCU的不同，将代码中输出高低电平的函数替换为你自己的函数。
```javascript
static void set_pwm_gpio(u8 gpio_state)	//设置用于模拟pwm的gpio电平
{
	if(gpio_state == 1) GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_SET);
	if(gpio_state == 0) GPIO_WriteBit(GPIOC, GPIO_Pin_7, Bit_RESET);
}
```
- 定时器中断函数，将下面的函数在定时器中断里进行调用，PWM周期与定时器中断时间相关。
```javascript
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
```
- 设置PWM周期和占空比，假设定时器中断为1ms，则设置1000代表周期为1s，其他定时器中断对应周期可自行推导。高电平时间不超过周期
```javascript
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
```
- .h文件中声明函数
```javascript
void SoftPwmTimerISR(void);
void SoftPwmSetPeriod(u16 period);
void SoftPwmSetHigh(u16 high);
```