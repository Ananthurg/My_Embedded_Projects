/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6011SSLAB1.c  
	Author:		SHAMIN SHAJU
	Date:		15/01/2025
	Modified:	None
	© Fanshawe College, 2025

	Description:Develop hardware sampling algorithms for automatic sampling of multiple sensors on a timed 	interval.Filter and average the samples over time for appropriate system actions. Develop a portable method 	for adding or removing future sensors / input devices.
=============================================================================*/

/* Preprocessor ===============================================================
   Hardware Configuration Bits ==============================================*/
#pragma config FOSC		= INTIO67
#pragma config PLLCFG	= OFF
#pragma config PRICLKEN = ON
#pragma config FCMEN	= OFF
#pragma config IESO		= OFF
#pragma config PWRTEN	= OFF 
#pragma config BOREN	= ON
#pragma config BORV		= 285 
#pragma config WDTEN	= OFF
#pragma config PBADEN	= OFF
#pragma config LVP		= OFF
#pragma config MCLRE	= EXTMCLR

// Libraries ==================================================================
#include <p18f45k22.h>
#include <stdio.h>
#include <stdlib.h>
#include <delays.h>
#include <usart.h>
#include <xlcd.h>
#include "I2C.h"



// Constants  =================================================================
#define TRUE	1	
#define FALSE	0
#define PBMASK	0xF0
#define NOPRESS	0xF0
#define PB1PRESS	0xE0
#define PB2PRESS	0xD0
#define PB3PRESS	0xB0
#define PB4PRESS	0x70
#define LED1	LATBbits.LATB0
#define LED2	LATBbits.LATB1
#define LED3	LATBbits.LATB2
#define LED4	LATBbits.LATB3
#define	DELAYCOUNT	50
#define LEDMASK	0x0F
#define T0FLAG	INTCONbits.TMR0IF
#define MAXSEC	60
#define	MAXMIN	60
#define MAXHOUR	12



#define _XTAL_FREQ 4000000
#define LCD_ADDR 0x3F
#define TRISD0 TRISDbits.TRISD0
#define TRISD1 TRISDbits.TRISD1
#define START SSP2CON2bits.SEN
#define STOP SSP2CON2bits.PEN



#define MODE 0xE0
#define CHANSEL 0xD0
#define INC	0xb0
#define DEC	0x70
#define SENSORCOUNT	3
#define	SAMPLESIZE	10
#define ONESEC	10
#define STUDENTID	841

#define STEPPERPORT1	LATA
#define STEPPERPORT2	LATC
#define STEPPERPORT3	LATD
#define STEPPERMASK1	0x0F
#define STEPPERMASK2	0xF0
#define STEP	1
#define PATTERNCOUNT	4

//#define TRIGPIN	LATBbits.LATB0
//#define ECHOPIN	PORTBbits.RB1
//


// Global Variables  ==========================================================
	typedef struct
	{
		int samples[SAMPLESIZE];
		int	avg;
		int	upperL;
		int	lowerL;
		int	insert;
		char avgRdy;
	}sensorCh_t;
	typedef struct
	{
		char currentPat;
		char patCounter;
		int setPos;
		int curPos;
	}stepper_t;
	stepper_t	mtr1;
	char patterns[PATTERNCOUNT]={0x01,0x02,0x04,0x08};
	char patterns2[PATTERNCOUNT]={0x10,0x20,0x40,0x80};
	 

	typedef	int	sensor_t;
	sensorCh_t 	sensors[SENSORCOUNT];//

// Functions  =================================================================

/*>>> config ADCON: =============================================================
Author:		SHAMIN SHAJU
Date:		07/06/24
Modified:	None
Desc:		This functin will set the ADCON bits.
Input: 		None 
Returns:	None 
 ============================================================================*/
void configADCON()
{
	ADCON0=0x01;//config ADCON::
	ADCON1=0x00;
	ADCON2=0xA9;
}//eo configADCON::	

/*>>> getADCSample: =============================================================
Author:		SHAMIN SHAJU
Date:		15/01/2025
Modified:	None
Desc:		select an ADC channel number from the input argument.starts an ADC 
			convertion abd waits for it to stop.Function returns the 10 bit ADC
			result to its call.   
Input: 		char chID,The selected ADC channel number
Returns:	int,the 
 ============================================================================*/
void setOsc4MHz(void)
{ 
	OSCCON=0x52;
	while(!OSCCONbits.HFIOFS);	
} // eo setOsc4MHz::


/*>>> configbits: =============================================================
Author:		SHAMIN SHAJU
Date:		09/08/24
Modified:	None
Desc:	 Configuring the Ports
Input: 		None 
Returns:	None 
 ============================================================================*/
void configbits(void)
{
	ANSELA=0x00;// set all pins on port-- A to Digital operation
	LATA=0x00;// Latch the pins to output control off
	TRISA=0x00;	 // Set the pins to output mode


	ANSELB=0x00;// set all pins on port-- A to Digital operation
	LATB=0x00;// Latch the pins to output control off
	TRISB=0x00; // Set the pins to output mode


	ANSELC=0x00;// set all pins on port-- A to Digital operation
	LATC=0x00;// Latch the pins to output control off
	TRISC = 0x00; // Set the pins to output mode


	ANSELD=0x00;// set all pins on port-- A to Digital operation
	LATD=0x00;// Latch the pins to output control off
	TRISD=0xFF; // Set the pins to output mode


	ANSELE=0x00;// set all pins on port-- A to Digital operation
	LATE=0x00;// Latch the pins to output control off
	TRISE=0xFF; // Set the pins to output mode

} // eo setOsc4MHz::


/*>>>resetTMR0: =============================================================
Author:		SHAMIN SHAJU
Date:		09/08/24
Modified:	Shamin shaju & 29/01/25
Desc:	 	This function will reset timer
Input: 		None 
Returns:	None 
 ============================================================================*/
void resetTMR0(void)
{
	T0FLAG=FALSE;
	TMR0H=0x3C;
	TMR0L=0xB0;
}//eo resetTMR0::
/*>>> configTMR0: =============================================================
Author:		SHAMIN SHAJU
Date:		09/08/24
Modified:	None
Desc:	 	Set the timer to zero.
Input: 		None 
Returns:	None 
 ============================================================================*/
void configTMR0(void)
{
	T0CON=0x93;
	resetTMR0();
}//configTMR0::

/*>>>configUSART: =============================================================
Author:		SHAMIN SHAJU
Date:		09/08/24
Modified:	None
Desc:	 	This function will configure USART.
Input: 		None 
Returns:	None 
 ============================================================================*/
//void configUSART()
//{
//	SPBRG1=25;
//	BAUDCON1=0x40;
//	RCSTA1=0x90;
//	TXSTA1=0x26;
//	
//}//configUSART::

/*>>> systemInitialization: =============================================================
Author:		SHAMIN SHAJU
Date:		09/08/24
Modified:	None
Desc:	 
Input: 		None 
Returns:	None 
 ============================================================================*/
void systemInitialization()
{
	setOsc4MHz();
	configbits();
	configTMR0();
//	configUSART();
	configADCON();
}//systemInitialization::


/*>>> getADCsample: =============================================================
Author:		SHAMIN SHAJU
Date:		07/06/24
Modified:	None
Desc:	 	This function requires the input channel number of the ADC pin to
		be sampled, writes that channel number into the ADC registry, 
		starts a conversion, waits for completion and returns the result
Input: 		chan 
Returns:	ADRES
 ============================================================================*/
int getADCsample(char chan)
{
	ADCON0bits.CHS=chan;
	ADCON0bits.GO=TRUE;
	while(ADCON0bits.GO);
	return ADRES;
}//eo getADCsample::

/*>>>iniSensorCh: =============================================================
Author:		SHAMIN SHAJU
Date:		15/01/25
Modified:	None
Desc:	 	This function use an input pointer to the data structure type above.
		Set all the values of the data members to 0
Input: 		address of sensor channel ,  *senPtr
Returns:	None 
 ============================================================================*/

void iniSensorCh(sensorCh_t	*senPtr)
{
	int index=0;
	for(index=0;index<SAMPLESIZE;index++)
	{
		senPtr-> avg=0;
		senPtr-> upperL=0;
		senPtr-> lowerL=0;
		senPtr->insert=0;
		senPtr->avgRdy=0;
	}// eo for loop::	
}//giving value to each elements in the data array

/*>>>measureDistance: =============================================================
Author:		SHAMIN SHAJU
Date:		28/02/25
Modified:	None
Desc:	 	updates the display of the sensor data. This
		function will be called only after fresh samples were taken and
		saved. 
Input: 		None
Returns:	None 
 ============================================================================*/
//unsigned int measureDistance (void)
//{
//	unsigned int time=0;
//	TRIGPIN=1;
//	Delay10KTCYx(1);
//	TRIGPIN=0;
//	while(!ECHOPIN)
//	TMR1H=0;
//	TMR1L=0;
//	T1CON=0x01;
//	while(ECHOPIN)
//	T1CON=0x00;
//	time=(TMR1H<<8)|TMR1L;
//	return time/58;
//}
	
// LCD DISPLAY FUNCTION==============================================================

void I2C2_Init()
{
    TRISD0 = 1; // SDA2 as input
    TRISD1 = 1; // SCL2 as input

    SSP2CON1 = 0x28;  // Enable I2C2 Master mode
    SSP2CON2 = 0x00;
    SSP2ADD = (_XTAL_FREQ / (4 * 100000)) - 1;  // 100kHz I2C clock
    SSP2STAT = 0x00;
}

void I2C2_Start() 
{
	START = TRUE;  // Start condition
	while(START);
}

void I2C2_Stop() 
{
    STOP = TRUE;  // Stop condition
    while(STOP);
}

void I2C2_Write(unsigned char data) 
{
    SSP2BUF = data;
    while (!PIR3bits.SSP2IF);  // Wait for transmission
    PIR3bits.SSP2IF = 0;
}

void LCD_Write(unsigned char data) 
{
    I2C2_Start();
    I2C2_Write(LCD_ADDR << 1); // Send address with write bit
    I2C2_Write(data);
    I2C2_Stop();
    Delay10KTCYx(2);
}

void LCD_Init() 
{
    Delay10KTCYx(2);
    LCD_Write(0x33);  // Initialize LCD
    LCD_Write(0x32);  // Set to 4-bit mode
    LCD_Write(0x28);  // 2-line, 5x8 font
    LCD_Write(0x0C);  // Display ON, Cursor OFF
    LCD_Write(0x06);  // Entry mode
    LCD_Write(0x01);  // Clear screen
    Delay10KTCYx(2);
}

void LCD_SendChar(char c) {
    char data = c | 0x0D;  // Enable RS bit
    LCD_Write(data);
}

void LCD_SendString(char *str) {
    while (*str) {
        LCD_SendChar(*str++);
    }
}


//===================================================================================

/*>>>initializeStepper: =============================================================
Author:		AnanthaKrishnan R
Date:		29/01/25
Modified:	None
Desc:	 	updates the display of the sensor data. This
			function will be called only after fresh samples were taken and
			saved. 
Input: 		stepper_t
Returns:	None 
 ============================================================================*/

void initializeStepper(stepper_t *mtr)// updates the display of the sensor data::
{
	mtr->patCounter=0;
	mtr->currentPat=patterns[PATTERNCOUNT];
	mtr->setPos=0;
	mtr->curPos=0;
	
}//eo dataDisplay::

/*>>>stepperMotor: =============================================================
Author:		Ananthakrishnan
Date:		28/02/25
Modified:	None
Desc:	 	motor function 
Input: 		char, MtrVal - To determine which type of motor to activate
Returns:	None 
 ============================================================================*/

void stepperMotor(char mtrVal)// updates the display of the sensor data::
{
	char startpos = TRUE;
	mtr1.curPos = 750;  // Set position for 90-degree rotation
    while(startpos) 
    {
	    // Move forward to 90 degrees
	    while(mtr1.curPos > 0)
        {
            if(mtr1.patCounter == 0)
            {
            	 mtr1.patCounter = PATTERNCOUNT - 1;
            }
               
            else
            {
            	mtr1.patCounter--;
            }
            mtr1.curPos -= STEP;
            switch (mtrVal)
            {
	            case 'r':
	            	STEPPERPORT1 = STEPPERMASK1 & patterns[mtr1.patCounter];
	           		break;
	           	case 'b':
	           		STEPPERPORT2 = STEPPERMASK1 & patterns[mtr1.patCounter];
	           		break;
	           	case'g':
	           		
	           		STEPPERPORT2 = STEPPERMASK2 & patterns2[mtr1.patCounter];
	           		break;
	            
	            default:
	            	break;   
            }
            Delay1KTCYx(2);
        }
        Delay10KTCYx(250); // Wait 10 seconds
        
        mtr1.setPos =750;

        // Move backward to the initial position
        
        while(mtr1.curPos <  mtr1.setPos)
        {
            mtr1.patCounter++;
            mtr1.curPos += STEP;
            if(mtr1.patCounter >= PATTERNCOUNT)
            {
	            mtr1.patCounter = 0;
            } 
            switch (mtrVal)
            {
	            case 'r':
	            	STEPPERPORT1 = STEPPERMASK1 & patterns[mtr1.patCounter];
	           		break;
	           	case 'b':
	           		STEPPERPORT2 = STEPPERMASK1 & patterns[mtr1.patCounter];
	           		break;
	           	case'g':
 	           		STEPPERPORT2 = STEPPERMASK2 & patterns2[mtr1.patCounter];
	           		break;
	            default:
	            	break;  
            }
            
            Delay1KTCYx(2);
        }

        Delay10KTCYx(250); // Wait 10 seconds
        
        mtr1.setPos = 0;

        // Move backward to the initial position
        startpos = FALSE;
    }
	
}//eo stepperMotor::


/*=== MAIN: FUNCTION ==========================================================
 ============================================================================*/

void main(void)
{
	char mtrVal = 'r';
    systemInitialization();
    initializeStepper(&mtr1);
    I2C2_Init();
    LCD_Init();
    I2C2_Write(0x00);
    LCD_SendString("Hello, PIC18F!");
    
    while (1)
    {
        LCD_SendString("TEST");  // Display "TEST" continuously
        Delay10KTCYx(100);
    }

   // stepperMotor(mtrVal);
}//eo main

