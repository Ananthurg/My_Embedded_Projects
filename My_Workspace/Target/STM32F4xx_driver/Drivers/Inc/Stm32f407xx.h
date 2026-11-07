/*
 * Stm32f407xx.h
 *
 *  Created on: 01-Nov-2026
 *      Author: Ananthu RG
 */

#ifndef INC_STM32F407XX_H_
#define INC_STM32F407XX_H_


#include<stdint.h>

#define __vo volatile


// Base Address of all Memory

#define FLASH_BASEADDR			0x08000000U
#define SRAM1_BASEADDR			0x20000000U
#define SRAM2_BASEADDR			0x2001C000U
#define ROM_BASEADDR			0x1FFF0000U

// Address of Bus Architecture.

#define PERIPH_BASEADDR 		0x40000000U
#define APB1PERIPH_BASEADDR		PERIPH_BASEADDR
#define APB2PERIPH_BASEADDR		0x40010000U
#define AHB1PERIPH_BASEADDR		0x40020000U
#define AHB2PERIPH_BASEADDR		0x50000000U
#define AHB3PERIPH_BASEADDR		0xA0000000U

// Base Address of all the peripheral in AHB1 Bus

#define GPIOA_BASEADDR			0x40020000U
#define GPIOB_BASEADDR			0x40020400U
#define GPIOC_BASEADDR			0x40020800U
#define GPIOD_BASEADDR			0x40020C00U
#define GPIOE_BASEADDR			0x40021000U
#define GPIOF_BASEADDR			0x40021400U
#define GPIOG_BASEADDR			0x40021800U
#define GPIOH_BASEADDR			0x40021C00U
#define GPIOI_BASEADDR			0x40022000U
#define GPIOJ_BASEADDR			0x40022400U
#define GPIOK_BASEADDR			0x40022800U

// Base Address of all the peripheral in APB1 Bus

#define I2C1_BASEADDR 		0x40005400U
#define I2C2_BASEADDR 		0x40005800U
#define I2C3_BASEADDR 		0x40005C00U
#define CAN1_BASEADDR 		0x40006400U
#define CAN2_BASEADDR 		0x40006800U
#define SPI2_BASEADDR 		0x40003800U
#define SPI3_BASEADDR 		0x40003C00U
#define USART2_BASEADDR 	0x40004400U
#define USART3_BASEADDR 	0x40004800U
#define UART4_BASEADDR 		0x40004C00U
#define UART5_BASEADDR 		0x40005000U

// Base Address of all Peripherals of APB2 Bus

#define SYSCFG_BASEADDR		0x40013800U
#define EXTI_BASEADDR		0x40013C00U
#define USART1_BASEADDR		0x40011000U
#define USART6_BASEADDR		0x40011400U
#define SPI1_BASEADDR		0x40013000U

/* <<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<< Data structure for GPIO Registers >>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>> */

typedef struct
{
	__vo uint32_t MODER;					// GPIO Mode Registers         								 Address offset : 0x00
	__vo uint32_t OTYPER;					// GPIO O/P TYPE Registers         							 Address offset : 0x04
	__vo uint32_t OSPEEDR;					// GPIO O/P SPEED Registers         						 Address offset : 0x08
	__vo uint32_t PUPDR;					// GPIO PULL up and PULL down Registers       				 Address offset : 0x0C
	__vo uint32_t IDR;						// GPIO Input Data Registers          						 Address offset : 0x10
	__vo uint32_t ODR;						// GPIO Output Data Registers          						 Address offset : 0x14
	__vo uint32_t BSRR;						// GPIO Bit set & Reset Registers         					 Address offset : 0x18
	__vo uint32_t LCKR;						// GPIO Lock Configuration Registers         				 Address offset : 0x1C
	__vo uint32_t AFR[2];					// GPIO Alternate function Registers Low[0] & high[1]     	 Address offset : 0x20

}GPIO_regDef_t;


GPIO_regDef_t *pGPIOA = ((GPIO_regDef_t*)GPIOA_BASEADDR); /// i Have to initialize for every GPIOs

typedef struct
{

	__vo uint32_t RCCR;
	__vo uint32_t PLLCFGR;
	__vo uint32_t CIR;
	__vo uint32_t AHB1RSTR;
	__vo uint32_t AHB2RSTR;
	__vo uint32_t AHB3RSTR;
		 uint32_t reserved;
	__vo uint32_t APB1RSTR;
	__vo uint32_t APB2RSTR;
		 uint32_t reserved;
		 uint32_t reserved;
	__vo uint32_t AHB1ENR;
	__vo uint32_t AHB2ENR;
	__vo uint32_t AHB3ENR;
	     uint32_t reserved;
	__vo uint32_t APB1ENR;
	__vo uint32_t APB2ENR;
	 	 uint32_t reserved;
	 	 uint32_t reserved;
	__vo uint32_t AHB1LPENR;
	__vo uint32_t AHB2LPENR;
	__vo uint32_t AHB3LPENR;
	     uint32_t reserved;
	__vo uint32_t APB1LPENR;
	__vo uint32_t APB2LPENR;
		 uint32_t reserved;
		 uint32_t reserved;
	__vo uint32_t BDCR;
	__vo uint32_t CSR;
	     uint32_t reserved;
	     uint32_t reserved;
	__vo uint32_t SSCGR;
	__vo uint32_t PLLI2SCFGR;
	__vo uint32_t PLLSAICFGR;
	__vo uint32_t DCKCFGR;

}RCC_regDef_t;




#endif /* INC_STM32F407XX_H_ */
