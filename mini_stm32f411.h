#ifndef _MINI_STM32F411_H_ 
#define _MINI_STM32F411_H_ 

/*====================BASIC QUALIFIERS=================*/ 
#define __I  volatile const
#define __O  volatile 
#define __IO volatile 
	
/*=====================IRQ NUMBERS======================*/ 

typedef enum { 
	  NonMaskableInt_IRQn       = -14,
    HardFault_IRQn           = -13,
    MemoryManagement_IRQn    = -12,
    BusFault_IRQn            = -11,
    UsageFault_IRQn          = -10,
    SVCall_IRQn              = -5,
    DebugMonitor_IRQn        = -4,
    PendSV_IRQn              = -2,
    SysTick_IRQn             = -1,
		
	  
}IRQn_type; 













#endif //_MINI_STM32F411_H_ 
