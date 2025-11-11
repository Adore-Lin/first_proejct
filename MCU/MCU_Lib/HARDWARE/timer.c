
#include "timer.h"
#include "led.h"

//---------------------------定时器TIM3 中断初始化------------------------------------------------------------------------------------------
//时钟选择为APB1的2倍， 而APB1为36M
//arr: 自动重装值
//pas: 时钟预分频数
void TIM3_Int_Init(u16 arr, u16 psc)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

/*定时器设置*/
	//1、时钟 TIM3 使能	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);

	//2、定时器 TIM3 初始化	
	TIM_TimeBaseStructure.TIM_Period                = arr;
	TIM_TimeBaseStructure.TIM_Prescaler             = psc;
	TIM_TimeBaseStructure.TIM_ClockDivision         = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode           = TIM_CounterMode_Up;
	//3、初始化 TIM3	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); 

/*中断设置*/
	//允许更新中断
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	//中断优先级 NVIC 设置	
	NVIC_InitStructure.NVIC_IRQChannel                   = TIM3_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority        = 3;
	NVIC_InitStructure.NVIC_IRQChannelCmd                = ENABLE;
	//初始化 NVIC 中断寄存器
	NVIC_Init(&NVIC_InitStructure);
	
	
//使能TIM3
	TIM_Cmd(TIM3, ENABLE);
}

//定时器TIM3 中断服务程序
void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
		LED1 = !LED1;
	}
}


//----------定时器TIM3 PWM初始化--------------------------------------------------------------------------------------------------------------------------------
//PWM 初始化
//arr: 自动重装值
//pas: 时钟预分频数
void TIM3_PWM_Init(u16 arr, u16 psc)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;

	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);

	GPIO_PinRemapConfig(GPIO_PartialRemap_TIM3, ENABLE); //Timer3部分重映射  TIM3_CH2->PB5    
 
   //设置该引脚为复用输出功能,输出TIM3 CH2的PWM脉冲波形	GPIOB.5
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_5; //TIM_CH2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;  //复用推挽输出
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);//初始化GPIO
 
   //初始化TIM3
	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
	TIM_TimeBaseStructure.TIM_ClockDivision = 0; //设置时钟分割:TDTS = Tck_tim
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位
	
	//初始化TIM3 Channel2 PWM模式	 
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM2; //选择定时器模式:TIM脉冲宽度调制模式2
 	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable; //比较输出使能
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; //输出极性:TIM输出比较极性高
	TIM_OC2Init(TIM3, &TIM_OCInitStructure);  //根据T指定的参数初始化外设TIM3 OC2

	TIM_OC2PreloadConfig(TIM3, TIM_OCPreload_Enable);  //使能TIM3在CCR2上的预装载寄存器
 
	TIM_Cmd(TIM3, ENABLE);  //使能TIM3
}

//定时器TIM5 通道1输入捕获配置
void TIM5_Cap_Init(u16 arr, u16 psc)
{
//	GPIO_InitTypeDef GPIO_InitStructure;
//	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
//	TIM_ICInitTypeDef TIM5_ICInitStructure;
//	NVIC_InitTypeDef NVIC_InitStructure;

//	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM5, ENABLE);
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);

//	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0; 
//	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPD;  
//	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
//	GPIO_Init(GPIOA, &GPIO_InitStructure);//初始化GPIO
//	GPIO_ResetBit(GPIOA, GPIO_Pin_0);//PA0下拉

//	//初始化TIM5
//	TIM_TimeBaseStructure.TIM_Period = arr; //设置在下一个更新事件装入活动的自动重装载寄存器周期的值
//	TIM_TimeBaseStructure.TIM_Prescaler =psc; //设置用来作为TIMx时钟频率除数的预分频值 
//	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CR1_CKD_1; 
//	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;  //TIM向上计数模式
//	TIM_TimeBaseInit(TIM5, &TIM_TimeBaseStructure); //根据TIM_TimeBaseInitStruct中指定的参数初始化TIMx的时间基数单位

//	TIM5_ICInitStructure.TIM_Channel = TIM_Channel_1;
//	TIM5_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
//	TIM5_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
//	TIM5_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
//	TIM5_ICInitStructure.TIM_ICFilter = 0x00;
//	TIM_ICInit(TIM5, TIM5_ICInitStructure);

//	NVIC_InitStructure.NVIC_IRQChannel = TIM5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_Init(&NVIC_InitStructure);

//	TIM_ITCofig(TIM5, TIM_IT_Update | TIM_IT_CC1, ENABLE);
//	TIM_Cmd(TIM5, ENABLE);  //使能TIM3
}

u8 TIM5_Cap_State = 0;//TIM5输入捕获状态
u16 TIM5_Cap_Value; //TIM5输入捕获值
void TIM5_IRQHandler(void)
{
//	//还未捕获成功
//	if(TIM5_Cap_State & 0x80 == 0)
//	{
//		if(TIM_GetITStatus(TIM5, TIM_IT_Update) != RESET)
//		{
//			if(TIM5_Cap_State & 0x40) //0100 0000 已经捕获到高电平了
//			{

//			}
//		}
//	}

//	//捕获1发生捕获事件
//	if(TIM_GetITStatus(TIM5, TIM_IT_CC1) != RESET)
//	{
//		
//	}
}




