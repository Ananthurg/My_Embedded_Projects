

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include "Stm32f407xx.h"


typedef struct
{
	uint8_t GPIO_PinNumber;
	uint8_t GPIO_PinMode;
	uint8_t GPIO_PinSpeed;
	uint8_t GPIO_PinPuPdControl;
	uint8_t GPIO_PinOpType;
	uint8_t GPIO_PinAltFunMode;

}GPIO_PinConfig_t;

typedef struct
{
	GPIO_regDef_t *pGPIOx;	// This holds the base address of the GPIO port to which the pin belongs
	GPIO_PinConfig_t GPIO_config;  //This holds GPIO Pin config settings

}GPIO_Handle_t;


/******************************************************************************************************
 * 								APIs Supported for this driver
 * 							For More information check the definition
 *
 * *****************************************************************************************************
 */


/*
* Peripheral Clock Handling
*/

void GPIO_PeriClockControl(void);

/*
* Init and De-init
*/
void GPIO_Init(void);
void GPIO_DeInit(void);

/*
* Data read and write
*/
void GPIO_ReadFromInputPin(void);
void GPIO_ReadFromInputPort(void);
void GPIO_WriteToOutputPin(void);
void GPIO_WriteToOutputPort(void);
void GPIO_ToggleOutputPin(void);

/*
* IRQ Configuration and ISR handling
*/

void GPIO_IRQConfig(void);
void GPIO_IRQHandling(void);



#endif /* INC_STM32F407XX_GPIO_DRIVER_H_ */
