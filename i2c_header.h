#ifndef _I2C_
#define _I2C_

#define I2C1_Base_Address 0x40005400UL
typedef struct{
volatile unsigned long CR1 ; 
volatile unsigned long CR2 ; 
volatile unsigned long OAR1 ; 
volatile unsigned long OAR2 ; 
volatile unsigned long DR;
volatile unsigned long SR1;
volatile unsigned long SR2;
volatile unsigned long CCR;
volatile unsigned long TRISE ; 
volatile unsigned long FLTR ; 
} I2C_Typedef;
#define I2C1 ((I2C_Typedef*) I2C1_Base_Address)

void I2C1_Init(void);
void I2C1_Start(void);
void I2C1_Stop(void);

void I2C1_Write_Addr(unsigned char address);
void I2C1_Write_Data(unsigned char data);

unsigned char I2C1_Read_ACK(void);
unsigned char I2C1_Read_NACK(void);
#endif