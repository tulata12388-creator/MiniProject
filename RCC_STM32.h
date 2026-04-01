#ifndef _RCC_STM32_H_
#define _RCC_STM32_H_

#define FLASH_BASE_ADR 0x40023C00UL
typedef struct
{
    volatile unsigned long ACR;
    volatile unsigned long KEYR;
    volatile unsigned long OPTKEYR;
    volatile unsigned long SR;
    volatile unsigned long CR;
    volatile unsigned long OPTCR;
} FLASH_TYPE;
#define FLASH_CONTROL ((FLASH_TYPE*) FLASH_BASE_ADR)
// define base address
#define RCC_BASE_ADDRESS 0x40023800UL
void RCC_Configuration(void);
// define data
typedef struct{
    volatile unsigned long CR;
    volatile unsigned long PLLCFGR;
    volatile unsigned long CFGR;
    volatile unsigned long CIR;
    volatile unsigned long AHB1RSTR;
    volatile unsigned long AHB2RSTR;
    unsigned long RESERVED0[2];
    volatile unsigned long APB1RSTR;
    volatile unsigned long APB2RSTR;
    unsigned long RESERVED1[2];
    volatile unsigned long AHB1ENR;
    volatile unsigned long AHB2ENR;
    unsigned long RESERVED2[2];
    volatile unsigned long APB1ENR;
    volatile unsigned long APB2ENR;
    unsigned long RESERVED3[2];
    volatile unsigned long AHB1LPENR;
    volatile unsigned long AHB2LPENR;
    unsigned long RESERVED4[2];
    volatile unsigned long APB1LPENR;
    volatile unsigned long APB2LPENR;
    unsigned long RESERVED5[2];
    volatile unsigned long BDCR;
    volatile unsigned long CSR;
    unsigned long RESERVED6[2];
    volatile unsigned long SSCGR;
    volatile unsigned long PLLI2SCFGR;
    volatile unsigned long DCKCFGRL;

} CLK_TYPE; 
#define CLK_CONTROL ((CLK_TYPE*) RCC_BASE_ADDRESS) 

#endif // _RCC_STM32_H