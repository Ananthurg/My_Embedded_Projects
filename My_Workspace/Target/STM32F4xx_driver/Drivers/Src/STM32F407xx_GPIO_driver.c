

#include "STM32F407xx_GPIO_DRIVER.h"


/*>>> functionName: Peripheral Clock Handling ===========================================================
Author:      AnanthuRG
Date:        09/11/2026
Modified:    None
Desc:        configure the IO port modules.
Input:       None
Returns:     None
=========================================================================================================*/

void GPIO_PeriClockControl(GPIO_regDef_t *pGPIOx, uint8_t EnorDi)
{

}//



/*>>> functionName: Init and De-init =======================================================================
Author:      AnanthuRG
Date:        09/11/2026
Modified:    None
Desc:        configure the IO port modules.
Input:       None
Returns:     None
===========================================================================================================*/

void GPIO_Init(GPIO_Handle_t *pGPIOHandle);
void GPIO_DeInit(GPIO_regDef_t *pGPIOx);



/*>>> functionName: Data read and write =======================================================================
Author:      AnanthuRG
Date:        09/11/2026
Modified:    None
Desc:        configure the IO port modules.
Input:       None
Returns:     None
===========================================================================================================*/

uint8_t GPIO_ReadFromInputPin(GPIO_regDef_t *pGPIOx, uint8_t PinNumber);
uint16_t GPIO_ReadFromInputPort(GPIO_regDef_t *pGPIOx);
void GPIO_WriteToOutputPin(GPIO_regDef_t *pGPIOx, uint8_t PinNumber, uint8_t Value);
void GPIO_WriteToOutputPort(GPIO_regDef_t *pGPIOx, uint8_t Value);
void GPIO_ToggleOutputPin(GPIO_regDef_t *pGPIOx,uint8_t PinNumber);

/*
* IRQ Configuration and ISR handling
*/

void GPIO_IRQConfig(uint8_t IRQNumber, uint8_t IRQPriority, uint8_t EnorDi);
void GPIO_IRQHandling(uint8_t PinNumber);
