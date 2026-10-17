#include "GPIO.h"
#include "board.h"   /* PIN_xxx defines from Section 2 */

/*============================================================
 *  GPIO_Config_ADC_PA0_PA3_Analog
 *
 *  Configure PA0-PA3 as analog inputs for ADC1 channels.
 *  MODER = 11 (analog), PUPDR = 00 (no pull — required for ADC)
 *============================================================*/
void GPIO_Config_ADC_PA0_PA3_Analog(void)
{
    /* MODER bits [7:0] ? set pairs to 0b11 (analog) for pins 0-3 */
    GPIOA->MODER |= (3U << (PIN_ADC_LM35 * 2))
                  | (3U << (PIN_ADC_GAS  * 2))
                  | (3U << (PIN_ADC_S3   * 2))
                  | (3U << (PIN_ADC_S4   * 2));

    /* No pull-up / pull-down (PUPDR = 00 for each pin) */
    GPIOA->PUPDR &= ~( (3U << (PIN_ADC_LM35 * 2))
                     | (3U << (PIN_ADC_GAS  * 2))
                     | (3U << (PIN_ADC_S3   * 2))
                     | (3U << (PIN_ADC_S4   * 2)) );
}

/*============================================================
 *  GPIO_Config_SPI1_PA4_PA7_AF5
 *
 *  Configure PA4-PA7 as SPI1 alternate function (AF5).
 *
 *  MODER  = 10 (alternate function)
 *  AFRL   = 5  (AF5 = SPI1 on STM32F411)
 *  OSPEEDR= 11 (very high speed — needed for 1 MHz SPI clock)
 *  PUPDR  = 10 (pull-down on NSS & SCK — idle low for Mode 0)
 *
 *  These pins connect directly to Raspberry Pi SPI0:
 *    PA4 (NSS)  ? Pi GPIO8  (CE0)
 *    PA5 (SCK)  ? Pi GPIO11 (SCLK)
 *    PA6 (MISO) ? Pi GPIO9  (MISO)   — STM32 drives this line
 *    PA7 (MOSI) ? Pi GPIO10 (MOSI)   — Pi drives this line
 *============================================================*/
void GPIO_Config_SPI1_PA4_PA7_AF5(void)
{
    /* Step 1: Set MODER to 10 (AF) for pins 4-7 */
    GPIOA->MODER &= ~( (3U << (PIN_SPI_NSS  * 2))
                     | (3U << (PIN_SPI_SCK  * 2))
                     | (3U << (PIN_SPI_MISO * 2))
                     | (3U << (PIN_SPI_MOSI * 2)) );
    GPIOA->MODER |=  ( (2U << (PIN_SPI_NSS  * 2))
                     | (2U << (PIN_SPI_SCK  * 2))
                     | (2U << (PIN_SPI_MISO * 2))
                     | (2U << (PIN_SPI_MOSI * 2)) );

    /* Step 2: Select AF5 in AFRL (4 bits per pin, pins 0-7) */
    GPIOA->AFRL &= ~( (0xFUL << (PIN_SPI_NSS  * 4))
                    | (0xFUL << (PIN_SPI_SCK  * 4))
                    | (0xFUL << (PIN_SPI_MISO * 4))
                    | (0xFUL << (PIN_SPI_MOSI * 4)) );
    GPIOA->AFRL |=  ( ((uint32_t)SPI1_AF << (PIN_SPI_NSS  * 4))
                    | ((uint32_t)SPI1_AF << (PIN_SPI_SCK  * 4))
                    | ((uint32_t)SPI1_AF << (PIN_SPI_MISO * 4))
                    | ((uint32_t)SPI1_AF << (PIN_SPI_MOSI * 4)) );

    /* Step 3: Very high speed (OSPEEDR = 11) */
    GPIOA->OSPEEDR |= (3U << (PIN_SPI_NSS  * 2))
                    | (3U << (PIN_SPI_SCK  * 2))
                    | (3U << (PIN_SPI_MISO * 2))
                    | (3U << (PIN_SPI_MOSI * 2));

    /* Step 4: Pull-down on NSS + SCK (idle low for SPI Mode 0) */
    GPIOA->PUPDR &= ~( (3U << (PIN_SPI_NSS  * 2))
                     | (3U << (PIN_SPI_SCK  * 2))
                     | (3U << (PIN_SPI_MISO * 2))
                     | (3U << (PIN_SPI_MOSI * 2)) );
    GPIOA->PUPDR |= (2U << (PIN_SPI_NSS * 2))
                  | (2U << (PIN_SPI_SCK * 2));
}
/*============================================================
 *  GPIOB_I2C1_Init
 *
 *  Configure PB8 and PB9 as I2C1 alternate function (AF4).
 *
 *  MODER   = 10 (Alternate function)
 *  OTYPER  = 1  (Open-drain — required for I2C bus)
 *  PUPDR   = 01 (Pull-up — provides the necessary logic high)
 *  OSPEEDR = 11 (Very high speed)
 *  AFRH    = 4  (AF4 = I2C1 on STM32F411)
 *
 *  Pin Mapping:
 *    PB8 -> I2C1_SCL (Serial Clock)
 *    PB9 -> I2C1_SDA (Serial Data)
 *============================================================*/
void GPIOB_I2C1_Init(void)
{
    /* 1. MODER: Set PB8 and PB9 to Alternate Function (10) */
    GPIOB->MODER &= ~(3U << 16); 
    GPIOB->MODER |=  (2U << 16); 

    GPIOB->MODER &= ~(3U << 18); 
    GPIOB->MODER |=  (2U << 18); 

    /* 2. OTYPER: Set to Open Drain (1) */
    GPIOB->OTYPER |= (1U << 8);  // PB8
    GPIOB->OTYPER |= (1U << 9);  // PB9

    /* 3. PUPDR: Set to Pull-up (01) */
    GPIOB->PUPDR &= ~(3U << 16);
    GPIOB->PUPDR |=  (1U << 16); // PB8

    GPIOB->PUPDR &= ~(3U << 18);
    GPIOB->PUPDR |=  (1U << 18); // PB9

    /* 4. OSPEEDR: Set to Very High Speed (11) */
    /* Added clear mask for safety */
    GPIOB->OSPEEDR &= ~(3U << 16); 
    GPIOB->OSPEEDR |=  (3U << 16); // PB8

    GPIOB->OSPEEDR &= ~(3U << 18);
    GPIOB->OSPEEDR |=  (3U << 18); // PB9

    /* 5. AFRH: Map Pins to Alternate Function 4 (I2C1) */
    GPIOB->AFRH &= ~(0xFU << 0);  // Clear AF bits for pin 8
    GPIOB->AFRH |=  (4U << 0);    // Set PB8 to AF4

    GPIOB->AFRH &= ~(0xFU << 4);  // Clear AF bits for pin 9
    GPIOB->AFRH |=  (4U << 4);    // Set PB9 to AF4
}

/*============================================================
 *  GPIO_Config_Buzzer_PB0_Output
 *
 *  PB0 ? General-purpose output, push-pull, no pull, default LOW.
 *  Drives active buzzer directly (< 20 mA).
 *============================================================*/
void GPIO_Config_Buzzer_PB0_Output(void)
{
    GPIOB->MODER  &= ~(3U << (PIN_BUZZER * 2));
    GPIOB->MODER  |=  (1U << (PIN_BUZZER * 2));  /* 01 = output   */
    GPIOB->OTYPER &= ~(1U << PIN_BUZZER);         /* 0  = push-pull*/
    GPIOB->PUPDR  &= ~(3U << (PIN_BUZZER * 2));   /* 00 = no pull  */
    GPIOB->ODR    &= ~(1U << PIN_BUZZER);          /* start OFF     */
}

/*============================================================
 *  GPIO_Config_Motor_PB1_Output
 *
 *  PB1 ? General-purpose output, push-pull, no pull, default LOW.
 *  Drives motor via MOSFET / relay (do NOT drive > 25 mA directly).
 *============================================================*/
void GPIO_Config_Motor_PB1_Output(void)
{
    GPIOB->MODER  &= ~(3U << (PIN_MOTOR * 2));
    GPIOB->MODER  |=  (1U << (PIN_MOTOR * 2));   /* 01 = output   */
    GPIOB->OTYPER &= ~(1U << PIN_MOTOR);          /* 0  = push-pull*/
    GPIOB->PUPDR  &= ~(3U << (PIN_MOTOR * 2));    /* 00 = no pull  */
    GPIOB->ODR    &= ~(1U << PIN_MOTOR);           /* start OFF     */
}