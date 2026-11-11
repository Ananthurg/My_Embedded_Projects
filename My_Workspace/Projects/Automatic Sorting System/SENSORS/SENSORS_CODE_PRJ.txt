/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011ARNLAB3.c  
	Author:		Ananthakrishnan R
	Date:		12/03/2025
	Modified:	None
	© Fanshawe College, 2024

	Description: Conveyor Belt control system using IR sensors and container level sensing
				usin g IR sensor.
=============================================================================*/

/* Preprocessor ===============================================================
   Hardware Configuration Bits ==============================================*/
#pragma config FOSC	= INTIO67
#pragma config PLLCFG	= OFF
#pragma config PRICLKEN = ON
#pragma config FCMEN	= OFF
#pragma config IESO	= OFF
#pragma config PWRTEN	= OFF 
#pragma config BOREN	= ON
#pragma config BORV	= 285 
#pragma config WDTEN	= OFF
#pragma config PBADEN	= OFF
#pragma config LVP	= OFF
#pragma config MCLRE	= EXTMCLR

// Libraries ==================================================================
#include <p18f45k22.h>
#include <stdio.h>
#include <stdlib.h>
#include <usart.h>
#include <delays.h>

// Constants  =================================================================
#define TRUE	1	
#define FALSE	0
#define PBMASK 0xF0	
#define NOPRESS 0XF0
#define MODE 0xE0
#define CHANSEL 0xD0
#define INC 0xB0
#define DEC 0x70

#define STEPPERPORT1 LATA
#define STEPPERMASK1 0X0F
#define STEP 1
#define PATTERNCOUNT 4

#define LEDMASK 0x0F
#define ADCRESV 0.0048828125
#define TEMPM 0.01
#define TEMPB 0.4
#define HUMDM 0.05
#define CO2M 0.000345833
#define CO2B 0 

#define LED1 LATDbits.LATD0
#define LED2 LATDbits.LATD1
#define LED3 LATDbits.LATD2
#define LED4 LATDbits.LATD3

#define IR_PORT1 PORTBbits.RB0
#define IR_PORT2 PORTAbits.RA0
#define IR_PORT3 PORTAbits.RA1
#define IR_PORT4 PORTAbits.RA2

#define RELAYPORT LATBbits.LATB1


#define T0FLAG INTCONbits.TMR0IF
#define SAMPSIZE 10

#define SENSORCOUNT 3
#define ONESEC 10
#define FOSC 4
#define STUDENTID 661

// Global Variables  ==========================================================

char patterns[PATTERNCOUNT] = {0x01, 0x02, 0x04, 0x08};


// Data Structure for Stepper Motor
typedef struct
{
	char currentPat;
	char patCounter;
	int curPos;
	int setPos;
	char movTag;
	

}
stepper_t;
stepper_t mtr1;


// Functions  =================================================================

/*>>> setOsc4MHz: ===========================================================
Author:		Ananthakrishnan R
Date:		17/05/2024
Modified:	None
Desc:		Setting Oscillator frquency to 4MHz
Input: 		None 
Returns:	None 
 ============================================================================*/
void setOsc4MHZ(void)
{
	OSCCON = 0x52;
	while (!OSCCONbits.HFIOFS);			
} // eo setOsc4MHZ::

/*>>> setPortConfig: ===========================================================
Author:		Ananthakrishnan R
Date:		25/05/2024
Modified:	None
Desc:		Configuring the Ports and setting ANSELx, LATx, TRISx, Portx registers
Input: 		None 
Returns:	None 
 ============================================================================*/
void portConfig(void)
{
	//Pin A Configuration
	ANSELA = 0x00; // set all pins on port-- A to Digital operation
	LATA = 0x00; // Latch the pins to output control off
	TRISA = 0x0F; // Set the pins to output mode

	//Pin B Configuration
	ANSELB = 0x00; // set all pins on port A to Digital operation
	LATB = 0x00; // Latch the pins to output control off
	TRISBbits.TRISB0 = 1; // IR sensor input (RB0)
	TRISBbits.TRISB1 = 0; // RELAY output (RB1)
//	TRISB = 0x01; // Set the pins to input mode

	//Pin C Configuration
	ANSELC = 0x00; // set all pins on port A to Digital operation
	LATC = 0x00; // Latch the pins to output control off
	TRISC = 0xE0; // Set the pins to input mode

	//Pin D Configuration
	ANSELD = 0x00; // set all pins on port A to Digital operation
	LATD = 0x00; // Latch the pins to output control off
	TRISD = 0xF0; // Set the pins to input mode
								
} // eo portConfig::


/*>>> USART1 config: ===========================================================
Author:		Ananthakrishnan R
Date:		21/06/2024
Modified:	None
Desc:		Configuring the USART1 
Input: 	None 
Returns:	None 
 ============================================================================*/



void sp1Config(void)
{
	SPBRG1 = 25; // Setting the baud rate to 9600
	BAUDCON1 = 0x40;
	RCSTA1 = 0x90;
	TXSTA1 = 0x26;
	

} // eo sp1Config::

/*>>> resetTimer: ===========================================================
Author:		Ananthakrishnan R
Date:		14/06/2024
Modified:	29/01/2025
Desc:		Resetting the timer to 100ms rollover and set up TMR0H and TMR0L
Input: 	None 
Returns:	None 
 ============================================================================*/

void resetTimer(void)
{
	T0FLAG = FALSE;
	TMR0H = 0x3C; //Preset False start value for 1s (Higher bits)
	TMR0L = 0xB0; //Preset Fasle start value for 1s (Lower bits)

} // eo resetTimer::

/*>>> configTimer: ===========================================================
Author:		Ananthakrishnan R
Date:		14/06/2024
Modified:	None
Desc:		Configuring the Timer0 
Input: 	None 
Returns:	None 
 ============================================================================*/

void configTimer0(void)
{

	T0CON = 0x90; // Configuring T0CON bits
	resetTimer(); // calling the reset timer function

} // eo configTimer0::

/*>>>configADC: ===========================================================
Author:	Ananthakrishnan R
Date:		07/06/2024
Modified:	None
Desc:		Setting the ADC Function
Input: 	None 
Returns:	None 
 ============================================================================*/

void configADC()
{	
	ADCON0 = 0x01;
	ADCON1 = 0x00;
	ADCON2 = 0xA9;
	
} // eo configADC::

/*>>> getADCSample: ===========================================================
Author:	Ananthakrishnan R
Date:		15/01/2025
Modified:	None
Desc:		Select an ADC channel number from the argument
		Starts an ADC conversion and waits for it to stops
		Function returns the 10bit ADC result to its call.
 
Input: 	char chTD, The selected ADC channel number. 

Returns:	int, the 10bit ADC result value. 
 ============================================================================*/

int getADCSample(char chan)
{
	ADCON0bits.CHS = chan;
	ADCON0bits.GO = TRUE;
	while(ADCON0bits.GO);
	return ADRES;

} // eo getADCSample::

/*>>>intialiseStepper: ===========================================================
Author:	Ananthakrishnan R
Date:		12/02/2025
Modified:	None
Desc:		This is a function to intialise the stepper motor struct elements to zero
Input: 	*vent, point to stepper_t Datastructure data value  
Returns:	None 
 ============================================================================*/
void intialiseStepper(stepper_t *vent)
{
	vent -> currentPat = 0x01;
	vent -> patCounter = 0;
	vent -> curPos = 0;
	vent -> setPos = 0;
	vent -> movTag = 0;

}//eo intialiseStepper::


/*>>>stepperMotor: =============================================================
Author:		Ananthakrishnan
Date:		28/02/25
Modified:	None
Desc:	 	motor function 
Input: 		char, MtrVal - To determine which type of motor to activate
Returns:	None 
 ============================================================================*/

void stepperMotor()// updates the display of the sensor data::
{
	mtr1.setPos = 760;  // Set position for 90-degree rotation
	if(mtr1.curPos != mtr1.setPos)
	{
   		if(mtr1.curPos <  mtr1.setPos)
        {
            mtr1.patCounter++;
            if(mtr1.patCounter >= PATTERNCOUNT)
            {
	            mtr1.patCounter = 0;
            } 
            mtr1.currentPat=patterns[mtr1.patCounter];
            mtr1.curPos += STEP;
            STEPPERPORT1 = STEPPERMASK1 & patterns[mtr1.patCounter];
        } 
        else if(mtr1.curPos >  mtr1.setPos)
        {
            mtr1.patCounter--;
            if(mtr1.patCounter <0)
            {
	            mtr1.patCounter =PATTERNCOUNT-1 ;
            } 
            mtr1.currentPat=patterns[mtr1.patCounter];
            mtr1.curPos -= STEP;
            STEPPERPORT1 = STEPPERMASK1 & patterns[mtr1.patCounter];
            
        }
	 }     
}//eo stepperMotor::



/*>>>systemInitialization: ===========================================================
Author:		Ananthakrishnan R
Date:		07/06/2024
Modified:	21/06/24
Desc:		Calling all the system initialization functions
Input: 		None 
Returns:	None 
 ============================================================================*/


void systemInitialization(void)
{
	setOsc4MHZ();
	portConfig();
//	sp1Config();
//	configTimer0();
//	configADC();


} // eo systemInitialization::

/*=== MAIN: FUNCTION ==========================================================
 ============================================================================*/
void main( void )
{
	int count = 0;
	char upFlag = 0;
	systemInitialization();
	//intialiseStepper(&mtr1);
	
	

	while(TRUE)
	{
		LED2 = FALSE;
		
		while(IR_PORT2 && IR_PORT3 && IR_PORT4)
		{
			RELAYPORT = 1;
			if(IR_PORT1 == 0 && count == 0)
			{
				RELAYPORT = FALSE;
				count = 1;
				LED1 = TRUE;
				LED2 = FALSE;
				Delay10KTCYx(500);
				RELAYPORT = TRUE;
				TRISBbits.TRISB0 = FALSE;
				Delay10KTCYx(100);
				TRISBbits.TRISB0 = TRUE;
				
				
			}
			if(IR_PORT1==1)
			{
				
	            count = 0;                                                                                                                                                 			count=0;
				LED1 = FALSE;
				LED2 = TRUE;
			}
		}
		
		RELAYPORT = 0;
		LED1 = TRUE;
		LED2 = FALSE;
		
					
	}
						

	
} // eo main::

