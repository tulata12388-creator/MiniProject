#include "NVIC.h" 
void NVIC_EnableIRQn		(unsigned char IRQn)
{
	NVIC_ISER[IRQn / 32 ] |= 	(1 << (IRQn % 32)); 
}
void NVIC_DisableIRQn		(unsigned char IRQn)
{
	NVIC_ICER[IRQn / 32 ] |=  (1 << (IRQn % 32)); 
}
void NVIC_SetPending 		(unsigned char IRQn)
{
	NVIC_ISPR[IRQn / 32 ] |=  (1 << (IRQn % 32));
}
void NVIC_ClearPending	(unsigned char IRQn)
{
	NVIC_ICPR[IRQn / 32 ] |=  (1 << (IRQn % 32));

}
void NVIC_SetPriority 	(unsigned char IRQn, unsigned char priority)
{
	NVIC_IPR [IRQn] |= (priority << 4); /* Only use 4-bits MSB */ 
}
