#include "board.h"
#include "RCC_STM32_LIB.h"
#include "GPIO.h"
#include "SPI_LIB.h"
#include "DMA_LIB.h"
#include "ADC_LIB.h"
#include "greenhouse.h"
#include "adc_mgr.h"
#include "fire_logic.h"
#include "actuators.h"

/*============================================================
 *  main.c � Entry Point
 *  -----------------------------------------------------------
 *  Project : Smart Greenhouse + Automatic Fire Alarm System
 *  MCU     : STM32F411VET6 (Cortex-M4F, 16 MHz HSI)
 *  IDE     : Keil �Vision 5 (CMSIS bare-metal)
 *
 *  -- Ki?n tr�c 3 l?p --
 *
 *  +-----------------------------------------------------+
 *  �  APP LAYER (main.c)                                 �
 *  �    Init all ? sleep (WFI)                           �
 *  +-----------------------------------------------------�
 *  �  SERVICE LAYER                                      �
 *  �    adc_mgr.c    : ADC filtering (moving average)    �
 *  �    fire_logic.c : State machine (hysteresis)        �
 *  �    actuators.c  : Buzzer pattern + motor control    �
 *  �    greenhouse.c : Central logic + SPI packet build  �
 *  +-----------------------------------------------------�
 *  �  BSP LAYER (bare-metal register-level)              �
 *  �    RCC_STM32_LIB.c : Clock enable                   �
 *  �    GPIO.c          : Pin configuration               �
 *  �    ADC_DMA_LIB.c   : ADC1 scan + DMA2 circular      �
 *  �    SPI_LIB.c       : SPI1 slave + RXNE IRQ          �
 *  +-----------------------------------------------------+
 *
 *  -- Interrupt Map --
 *
 *  ISR                    Priority   Ch?c nang
 *  ---------------------  --------   ----------------------
 *  SPI1_IRQn              1 (cao)   Tr? byte cho Raspberry Pi
 *  DMA2_Stream0_IRQn      2 (gi?a)  Set flag (processing deferred)
 *  SysTick_IRQn           3 (th?p)  Buzzer beep pattern 1ms
 *
 *  -- Lu?ng d? li?u --
 *
 *  Sensors ? ADC1 ? DMA2 ? [flag] ? main loop ? adc_mgr
 *     ? fire_logic ? actuators ? greenhouse ? SPI1 ? RPi
 *============================================================*/

/*------------------------------------------------------------
 *  SysTick_Handler � 1 ms periodic interrupt
 *
 *  Ch?c nang: c?p nh?t buzzer beep pattern.
 *  Priority 3 (th?p nh?t) ? kh�ng block DMA hay SPI.
 *------------------------------------------------------------*/
void SysTick_Handler(void)
{
    Actuator_Tick1ms();
}

/*------------------------------------------------------------
 *  SysTick_Init � C?u h�nh SysTick cho 1 ms interrupt
 *
 *  Clock source: processor clock (HSI 16 MHz)
 *  LOAD = 16000000 / 1000 - 1 = 15999
 *  ? SysTick fires m?i 1 ms
 *------------------------------------------------------------*/
static void SysTick_Init(void)
{
    /* N?p gi� tr? d?m: 16 MHz / 1 kHz - 1 = 15999 */
    SysTick->LOAD = (SYS_CLOCK_HZ / SYSTICK_FREQ_HZ) - 1U;

    /* Reset counter hi?n t?i */
    SysTick->VAL  = 0U;

    /* B?t SysTick:
     * Bit CLKSOURCE = 1 : d�ng processor clock (16 MHz)
     * Bit TICKINT   = 1 : cho ph�p interrupt khi d?m v? 0
     * Bit ENABLE    = 1 : b?t counter */
    SysTick->CTRL = SysTick_CTRL_CLKSOURCE_Msk
                  | SysTick_CTRL_TICKINT_Msk
                  | SysTick_CTRL_ENABLE_Msk;

    /* Set priority cho SysTick (th?p nh?t trong 3 ISR) */
    NVIC_SetPriority(SysTick_IRQn, IRQ_PRIO_SYSTICK);
}

/*------------------------------------------------------------
 *  main � Kh?i t?o h? th?ng & v�o sleep loop
 *
 *  Th? t? kh?i t?o R?T QUAN TR?NG:
 *    1. Clock ph?i b?t tru?c khi config peripheral
 *    2. GPIO ph?i config tru?c khi d�ng ADC/SPI
 *    3. Module SW init tru?c khi c� data
 *    4. SPI init + packet tru?c khi Pi b?t d?u poll
 *    5. ADC+DMA b?t cu?i c�ng (b?t d?u t?o IRQ)
 *    6. SysTick b?t sau c�ng
 *------------------------------------------------------------*/
int main(void)
{
    /* -- 1. B?t clock cho t?t c? peripheral c?n d�ng -- */
    RCC_Enable_For_GPIO_ADC_SPI_DMA();
    /*   AHB1: GPIOA, GPIOB, DMA2
     *   APB2: ADC1, SPI1                                */

    /* -- 2. C?u h�nh GPIO pins -- */
    GPIO_Config_ADC_PA0_PA3_Analog();   /* PA0-PA3: analog input    */
    GPIO_Config_SPI1_PA4_PA7_AF5();     /* PA4-PA7: SPI1 AF5       */
    GPIO_Config_Buzzer_PB0_Output();    /* PB0: push-pull output    */
    GPIO_Config_Motor_PB1_Output();     /* PB1: push-pull output    */

    /* -- 3. Kh?i t?o module ph?n m?m (Service Layer) -- */
    ADC_Mgr_Init();                     /* Reset b? l?c ADC         */
    FireLogic_Init();                   /* State ? NORMAL           */
    Actuator_Init();                    /* Buzzer OFF, Motor OFF    */

    /* -- 4. SPI1 slave + frame kh?i t?o --
     *  IMPORTANT: InitPacket BEFORE SPI Init!
     *  InitPacket calls SetTxBuffer to set g_tx/g_len.
     *  SPI Init reads g_tx[0] to pre-fill DR.
     *  If SPI Init runs first, g_tx is NULL ? DR gets 0x00. */
    Greenhouse_InitPacket();            /* Build frame zero ? set TX buf */
    SPI1_Slave_Init();                  /* SPI1 slave, TXE IRQ, pre-fill DR */

    /* -- 5. ADC1 scan + DMA2 circular (b?t d?u convert) -- */
		
    ADC1_DMA2_Stream0_InitStart();      /* B?t d?u convert 4 k�nh  */
		I2C1_DMA1_Stream0_Init();
    /*   T? d�y DMA TC IRQ s? fire li�n t?c,
     *   g?i Greenhouse_OnAdcReady() m?i l?n                     */

    /* -- 6. SysTick 1 ms (cho buzzer beep pattern) -- */
    SysTick_Init();

    /* -- 7. Main loop: sleep ? wake ? process ADC data --
     *
     *  BUG FIX v4: Processing is DEFERRED from DMA ISR to here.
     *
     *  Previously, Greenhouse_OnAdcReady() ran inside DMA ISR
     *  (prio 1).  ADC continuous + DMA circular fires TC every
     *  ~48 �s.  At -O0, the processing chain took >48 �s ?
     *  DMA ISR re-entered immediately ? SPI IRQ (prio 2) was
     *  STARVED ? SPI1_IRQHandler never fired ? DR never
     *  updated ? Pi read 0xAA (stale pre-fill) forever.
     *
     *  Now: DMA ISR sets flag (< 1 �s).  __WFI() wakes on
     *  any IRQ, then we check the flag and process here at
     *  thread level.  SPI IRQ (prio 1) preempts freely.
     */
    while (1)
    {
        __WFI();
        if (ADC_DMA_CheckReady())
        {
            Greenhouse_OnAdcReady();
        }
    }
}
