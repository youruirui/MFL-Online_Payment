#include "Init.h"




void Board_Init(void)
{
		sysinit();			//初始化系统
		FLASH_Init(BUS_CLK_HZ); 
		printf("Board Init Start\n");
		Gpio_Init();
		Uart_Init();
		Rtc_Init();
	  Pit_Init();
		printf("Board Init Finish\n");
}

void Gpio_Init(void)
{
	
}
void Uart_Init(void)
{

}

void Rtc_Init(void)
{
		RTC_ConfigType  sRTCConfig;					//RTC
    RTC_ConfigType  *pRTCConfig = &sRTCConfig; 
//*********************RTC 1ms定时*********************/
    pRTCConfig->u16ModuloValue = 200;//192;                                      
    pRTCConfig->bInterruptEn   = RTC_INTERRUPT_ENABLE;     
    pRTCConfig->bClockSource   = RTC_CLKSRC_BUS;          
    pRTCConfig->bClockPresaler = RTC_CLK_PRESCALER_100;    
    RTC_Init(pRTCConfig); 
    NVIC_SetPriority(RTC_IRQn,2); 
	
}
void PIT_Task(void)
{
	static uint8 Cnt =0;
	Cnt++;
	if(Cnt>=10)
	{
		Cnt=0;
		NeoWayExternalPar.LoseTime++;
		if(NeoWayExternalPar.LoseTime >= 122)
		{
			NeoWayExternalPar.HardwareRebootState = ON;
			NeoWayExternalPar.LoseTime = 0;
		}
	}

}
void Pit_Init(void)
{
	PIT_ConfigType  sPITConfig0;  
	PIT_ConfigType  *pPIT_Config0   =&sPITConfig0; 
	 uint32_t        u32LoadValue0;     
	/* PIT clock source is bus clock,20MHz */
	/* PIT channel 0 load value = (1000000-1) */
	u32LoadValue0   = 0x1E847F;                  /*!< PIT ch0 timer load value  */ 
	/* configure PIT channel 0, only enable timer */    	
	pPIT_Config0->u32LoadValue      = u32LoadValue0;
	pPIT_Config0->bFreeze           = FALSE;    
	pPIT_Config0->bModuleDis        = FALSE;    /*!< enable PIT module */     
	pPIT_Config0->bInterruptEn      = TRUE;
	pPIT_Config0->bChainMode        = TRUE; 
	pPIT_Config0->bTimerEn          = TRUE;
	PIT_Init(PIT_CHANNEL0, pPIT_Config0);  
	PIT_SetCallback(PIT_CHANNEL0, PIT_Task);
  NVIC_SetPriority(PIT_CH0_IRQn,1); 
}
