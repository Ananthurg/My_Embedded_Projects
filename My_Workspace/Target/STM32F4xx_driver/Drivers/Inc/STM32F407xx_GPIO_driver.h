

#ifndef INC_STM32F407XX_GPIO_DRIVER_H_
#define INC_STM32F407XX_GPIO_DRIVER_H_

#include "Stm32f407xx.h"


typedef struct
{

}GPIO_PinConfig_t;

typedef struct
{
	GPIO_regDef_t *pGPIOx;
	GPIO_PinConfig_t GPIO_config;

}GPIO_Handle_t;


#endif /* INC_STM32F407XX_GPIO_DRIVER_H_ */
