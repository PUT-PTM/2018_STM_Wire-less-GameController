#include "stm32f4xx.h"
#include "stm32f4_discovery.h"
#include "tm_stm32f4_lis302dl_lis3dsh.h"
int X, Y, Z;
int stan;


void TIM4_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM4, TIM_IT_Update) != RESET)
	{
		//czekaj na opró¿nienie bufora wyjœciowego
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
		// wyslanie danych

		char data = Y + '0';
		USART_SendData(USART3, data);
		// czekaj az dane zostana wyslane
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);

		TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	}
}


void TIM3_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM3, TIM_IT_Update) != RESET)
	{
		//czekaj na opró¿nienie bufora wyjœciowego
		while(USART_GetFlagStatus(USART3, USART_FLAG_TXE) == RESET);
		// wyslanie danych

		char data = 50 + '0';
		USART_SendData(USART3, data);
		// czekaj az dane zostana wyslane
		while (USART_GetFlagStatus(USART3, USART_FLAG_TC) == RESET);

		TIM_Cmd(TIM3, DISABLE);
		TIM_SetCounter(TIM3, 0);

		TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	}
}

void EXTI0_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line0) != RESET)
	{
		TIM_Cmd(TIM3, ENABLE);

		EXTI_ClearITPendingBit(EXTI_Line0);
	}
}



int main(void)
{
	TM_LIS302DL_LIS3DSH_Init(TM_LIS3DSH_Sensitivity_2G, TM_LIS3DSH_Filter_800Hz);
	TM_LIS302DL_LIS3DSH_t Axes;


	// wlaczenie taktowania wybranego portu
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOC, ENABLE);
	// wlaczenie taktowania wybranego uk³adu USART
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_USART3, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_SYSCFG, ENABLE);
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	// konfiguracja linii Rx i Tx
	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC, &GPIO_InitStructure);
	// ustawienie funkcji alternatywnej dla pinów (USART)
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource10, GPIO_AF_USART3);
	GPIO_PinAFConfig(GPIOC, GPIO_PinSource11, GPIO_AF_USART3);

	GPIO_InitTypeDef  GPIO_InitStructure3;
	GPIO_InitStructure3.GPIO_Pin = GPIO_Pin_0 ;
	GPIO_InitStructure3.GPIO_Mode = GPIO_Mode_IN;
	GPIO_InitStructure3.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure3.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure3.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure3);



	USART_InitTypeDef USART_InitStructure;
	// predkosc transmisji (mozliwe standardowe opcje: 9600, 19200, 38400, 57600, 115200, ...)
	USART_InitStructure.USART_BaudRate = 9600;
	// d³ugoœæ s³owa (USART_WordLength_8b lub USART_WordLength_9b)
	USART_InitStructure.USART_WordLength = USART_WordLength_8b;
	// liczba bitów stopu (USART_StopBits_1, USART_StopBits_0_5, USART_StopBits_2, USART_StopBits_1_5)
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	// sprawdzanie parzystoœci (USART_Parity_No, USART_Parity_Even, USART_Parity_Odd)
	USART_InitStructure.USART_Parity = USART_Parity_No;
	// sprzêtowa kontrola przep³ywu (USART_HardwareFlowControl_None, USART_HardwareFlowControl_RTS, USART_HardwareFlowControl_CTS, USART_HardwareFlowControl_RTS_CTS)
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	// tryb nadawania/odbierania (USART_Mode_Rx, USART_Mode_Rx )
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	// konfiguracja
	USART_Init(USART3, &USART_InitStructure);


	USART_Cmd(USART3, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = 9999;
	TIM_TimeBaseStructure.TIM_Prescaler = 209;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode =  TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM4, &TIM_TimeBaseStructure);


	NVIC_InitTypeDef NVIC_InitStructure;
	NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure);

	TIM_Cmd(TIM4, ENABLE);

	TIM_ClearITPendingBit(TIM4, TIM_IT_Update);
	TIM_ITConfig(TIM4, TIM_IT_Update, ENABLE);

	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure2;
	TIM_TimeBaseStructure2.TIM_Period = 9999;
	TIM_TimeBaseStructure2.TIM_Prescaler = 1679;
	TIM_TimeBaseStructure2.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure2.TIM_CounterMode =  TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure2);


	NVIC_InitTypeDef NVIC_InitStructure3;
	NVIC_InitStructure3.NVIC_IRQChannel = TIM3_IRQn;
	NVIC_InitStructure3.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure3.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure3.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure3);

	TIM_ClearITPendingBit(TIM3, TIM_IT_Update);
	TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);

	NVIC_InitTypeDef NVIC_InitStructure2;
	NVIC_InitStructure2.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure2.NVIC_IRQChannelPreemptionPriority = 0x00;
	NVIC_InitStructure2.NVIC_IRQChannelSubPriority = 0x00;
	NVIC_InitStructure2.NVIC_IRQChannelCmd = ENABLE;
	NVIC_Init(&NVIC_InitStructure2);

	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Rising;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);


	for(;;)
	{
		TM_LIS302DL_LIS3DSH_ReadAxes(&Axes);
		X = 9.80665*Axes.X/16384;
		Y = 9.80665*Axes.Y/16384;
		Z = 9.80665*Axes.Z/16384;
	}
}
