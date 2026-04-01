#ifndef _NVIC_H
#define _NVIC_H 
/* Define base address ISERx and ICERx (set/ clear enable )
											 ISPRx and ICPRx (Set/Clear pending )  
											 IPRx 					 (Priority Register )
*/
#define NVIC_ISER_BASE   (0xE000E100UL)
#define NVIC_ICER_BASE   (0xE000E180UL)
#define NVIC_ISPR_BASE   (0xE000E200UL)
#define NVIC_ICPR_BASE   (0xE000E280UL)
#define NVIC_IPR_BASE    (0xE000E400UL)
/* Pointer */ 
#define NVIC_ISER ((volatile unsigned long*) NVIC_ISER_BASE)
#define NVIC_ICER ((volatile unsigned long*) NVIC_ICER_BASE)
#define NVIC_ISPR ((volatile unsigned long*) NVIC_ISPR_BASE)
#define NVIC_ICPR ((volatile unsigned long*) NVIC_ICPR_BASE)
#define NVIC_IPR  ((volatile unsigned long*) NVIC_IPR_BASE)
/* Function */ 
void NVIC_EnableIRQn		(unsigned char IRQn);
void NVIC_DisableIRQn		(unsigned char IRQn);
void NVIC_SetPending 		(unsigned char IRQn); 
void NVIC_ClearPending	(unsigned char IRQn);
void NVIC_SetPriority 	(unsigned char IRQn, unsigned char priority); 
#endif /* NVIC.h */