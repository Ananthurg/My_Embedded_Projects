

#include <stdint.h>
#include<stdio.h>




int main(void)
{

	uint32_t *pClockControlReg = (uint32_t*)0x40023830;
	uint32_t *pGpioMode 	   = (uint32_t*)0x40020c00;
	uint32_t *pGpioDData 	   = (uint32_t*)0x40020c14;

	// Enable the clock for GPIOD in AHB1ENR
	*pClockControlReg |= 0x08;

	*pGpioMode &= 0xFCFFFFFF;
	*pGpioMode |= 0x01000000;

	*pGpioDData |= 0x1000;



while(1){
	;
}



}
