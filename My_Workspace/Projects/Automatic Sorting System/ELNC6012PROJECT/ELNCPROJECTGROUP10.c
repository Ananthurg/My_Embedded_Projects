/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
    File Name:   ELNC6011SSLAB1.c  
    Author:      SHAMIN SHAJU
    Date:        15/01/2025
    Modified:    None
    © Fanshawe College, 2025

    Description: This project is an automated conveyor belt sorting system that uses an ESP32-CAM and 
                 PIC18F45K22 microcontroller to identify characters like A, B, C and activate the motor according to it. 
                 It uses stepper motors, IR sensor, and a 1602A LCD to manage, monitor, and display sorting operations in real time
=============================================================================*/

/* Preprocessor ===============================================================
   Hardware Configuration Bits ==============================================*/
#pragma config FOSC      = INTIO67
#pragma config PLLCFG    = OFF
#pragma config PRICLKEN  = ON
#pragma config FCMEN     = OFF
#pragma config IESO      = OFF
#pragma config PWRTEN    = OFF 
#pragma config BOREN     = ON
#pragma config BORV      = 285 
#pragma config WDTEN     = OFF
#pragma config PBADEN    = OFF
#pragma config LVP       = OFF
#pragma config MCLRE     = EXTMCLR

// Libraries ==================================================================
#include <p18f45k22.h>
#include <stdio.h>
#include <stdlib.h>
#include <delays.h>
#include <usart.h>

// Constants  =================================================================
#define TRUE    1   
#define FALSE   0
#define PBMASK  0xF0
#define NOPRESS 0xF0
#define PB1PRESS    0xE0
#define PB2PRESS    0xD0
#define PB3PRESS    0xB0
#define PB4PRESS    0x70
#define LED1    LATBbits.LATB0
#define LED2    LATBbits.LATB1
#define LED3    LATBbits.LATB2
#define LED4    LATBbits.LATB3
#define LEDMASK 0x0F
#define T0FLAG  INTCONbits.TMR0IF
#define MAXSEC  60
#define MAXMIN  60
#define MAXHOUR 12

#define lcdE    PORTAbits.RA6
#define lcdRS   PORTAbits.RA5
#define DELAYCOUNT 2
#define LCDSIZE 18

#define MODE    0xE0
#define CHANSEL 0xD0
#define INC     0xb0
#define DEC     0x70
#define SENSORCOUNT    3
#define SAMPLESIZE 10
#define ONESEC  10
#define STUDENTID   841

#define STEPPERPORT1   LATA
#define STEPPERPORT2   LATB
#define STEPPERPORT3   LATC
#define STEPPERMASK1   0x0F
#define STEPPERMASK2   0xF0
#define STEP    1
#define PATTERNCOUNT   4

// Global Variables  ==========================================================
int count1 = 0;  // Dynamic count value 1
int count2 = 0;  // Dynamic count value 2
int count3 = 0;  // Dynamic count value 3
char str2[LCDSIZE] = {"C1:0  C2:0  C3:0  "};  // Buffer to hold the formatted string for counts
char hold = 0;
char mtrVal = 0;
char mtrCount = 0;

// Prototype definition

void isr(void);  // Function implementation is below

#pragma code interrupt_vector = 0x0008
void interrupt_vector(void)
{
    _asm
        GOTO isr
    _endasm
}
#pragma code

typedef struct
{
    char currentPat;
    char patCounter;
    int setPos;
    int curPos;
} stepper_t;
stepper_t mtr1;
char patterns[PATTERNCOUNT] = {0x01, 0x02, 0x04, 0x08};
char patterns2[PATTERNCOUNT] = {0x10, 0x20, 0x40, 0x80};

typedef int sensor_t;
//  sensorCh_t   sensors[SENSORCOUNT];//

// Functions  =================================================================

/*>>> config ADCON: =============================================================
Author:      SHAMIN SHAJU
Date:        07/06/24
Modified:    None
Desc:        This function will set the ADCON bits.
Input:       None 
Returns:     None 
============================================================================*/
void configADCON()
{
    ADCON0 = 0x01; // config ADCON:
    ADCON1 = 0x00;
    ADCON2 = 0xA9;
}//eo configADCON:

/*>>> getADCSample: =============================================================
Author:      SHAMIN SHAJU
Date:        15/01/2025
Modified:    None
Desc:        select an ADC channel number from the input argument. Starts an ADC 
             conversion and waits for it to stop. Function returns the 10-bit ADC
             result to its caller.   
Input:       char chID, The selected ADC channel number
Returns:     int, the ADC result
============================================================================*/

/*>>> setOsc4MHz: =============================================================
Author:      SHAMIN SHAJU
Date:        15/01/2025
Modified:    None
Desc:        Sets oscillator to 4MHz.
============================================================================*/
void setOsc4MHz(void)
{ 
    OSCCON = 0x52;
    while(!OSCCONbits.HFIOFS);  
} // eo setOsc4MHz:

/*>>> configbits: =============================================================
Author:      Ananthakrishnan R
Date:        09/08/24
Modified:    None
Desc:        Configuration of all Ports 
Input:       None 
Returns:     None 
============================================================================*/
void configbits(void)
{
    ANSELA = 0x00; // set all pins on Port A to Digital operation
    LATA = 0x00;   // Latch the pins to off
    TRISA = 0x00;  // Set the pins to output mode

    ANSELB = 0x00; // set all pins on Port B to Digital operation
    LATB = 0x00;   // Latch the pins to off
    TRISB = 0x00;  // Set the pins to output mode

    ANSELC = 0x00; // set all pins on Port C to Digital operation
    LATC = 0x00;   // Latch the pins to off
    TRISC = 0x80;  // Set the pins to output mode

    ANSELD = 0x00; // set all pins on Port D to Digital operation
    LATD = 0x00;   // Latch the pins to off
    TRISD = 0x00;  // Set the pins to output mode

    ANSELE = 0x00; // set all pins on Port E to Digital operation
    LATE = 0x00;   // Latch the pins to off
    TRISE = 0xFF;  // Set the pins to output mode
} // eo configbits:

/*>>> resetTMR0: =============================================================
Author:      SHAMIN SHAJU
Date:        09/08/24
Modified:    Shamin Shaju & 29/01/25
Desc:        This function will reset the timer.
Input:       None 
Returns:     None 
============================================================================*/
void resetTMR0(void)
{
    T0FLAG = FALSE;
    TMR0H = 0x3C;
    TMR0L = 0xB0;
}//eo resetTMR0:

/*>>> configTMR0: =============================================================
Author:      SHAMIN SHAJU
Date:        09/08/24
Modified:    None
Desc:        Set the timer to zero.
Input:       None 
Returns:     None 
============================================================================*/
void configTMR0(void)
{
    T0CON = 0x93;
    resetTMR0();
}//configTMR0:

/*>>> configUSART: =============================================================
Author:      SHAMIN SHAJU
Date:        09/08/24
Modified:    None
Desc:        This function will configure USART.
Input:       None 
Returns:     None 
============================================================================*/
void configUSART()
{
    SPBRG1 = 25;
    BAUDCON1 = 0x40;
    RCSTA1 = 0x90;	
    TXSTA1 = 0x26;
}//configUSART:

void configInts()
{
    INTCON = 0xC0;
    RCONbits.IPEN = FALSE;
    PIE1bits.RC1IE = TRUE;
}

void isr()
{
hold=0;
    if(PIR1bits.RC1IF)
    {
        hold = RCREG1;
        if(hold == 'A' || hold == 'B' || hold == 'C')
        {   
            mtrVal = hold;  
        }  
    }
    INTCON |= 0xC0;
}

/*>>> getADCsample: =============================================================
Author:      SHAMIN SHAJU
Date:        07/06/24
Modified:    None
Desc:        This function requires the input channel number of the ADC pin to
             be sampled, writes that channel number into the ADC registry, 
             starts a conversion, waits for completion and returns the result.
Input:       chan 
Returns:     ADRES
============================================================================*/
int getADCsample(char chan)
{
    ADCON0bits.CHS = chan;
    ADCON0bits.GO = TRUE;
    while(ADCON0bits.GO);
    return ADRES;
}//eo getADCsample:

/*>>> initializeStepper: =============================================================
Author:      AnanthaKrishnan R
Date:        29/01/25
Modified:    None
Desc:        Initializes the stepper motor structure.
Input:       Pointer to stepper_t structure
Returns:     None 
============================================================================*/
void initializeStepper(stepper_t *mtr)
{
    mtr->patCounter = 0;
    mtr->currentPat = patterns[PATTERNCOUNT];  // Though, note that array index PATTERNCOUNT is out of bounds; adjust if needed.
    mtr->setPos = 0;
    mtr->curPos = 0;
}//eo initializeStepper:

/*>>> sndCmd: =============================================================
Author:      AnanthaKrishnan R
Date:        26/03/25
Modified:    None
Desc:        Sends a command to the LCD for display.
Input:       unsigned char cmd – LCD command.
Returns:     None 
============================================================================*/
void sndCmd(unsigned char cmd)
{
    lcdRS = FALSE;
    Delay1KTCYx(DELAYCOUNT);
    lcdE = TRUE;
    Delay1KTCYx(DELAYCOUNT);
    PORTD = cmd;
    Delay1KTCYx(DELAYCOUNT);
    lcdE = FALSE;
    Delay1KTCYx(DELAYCOUNT);
}//eo sndCmd:

/*>>> sndDATA: =============================================================
Author:      AnanthaKrishnan R
Date:        26/03/25
Modified:    None
Desc:        Sends data to the LCD for display.
Input:       unsigned char data – data to display.
Returns:     None 
============================================================================*/
void sndData(unsigned char data)
{
    lcdRS = TRUE;
    lcdE = TRUE;
    PORTD = data;
    lcdE = FALSE;
}

/*>>> initLCD: =============================================================
Author:      AnanthaKrishnan R
Date:        26/03/25
Modified:    None
Desc:        Initializes the LCD display.
Input:       None
Returns:     None 
============================================================================*/
void initLcd(void)
{
    sndCmd(0x38);
    sndCmd(0x0E);
    sndCmd(0x01);
}

/*>>> stepperMotor: =============================================================
Author:      AnanthaKrishnan R
Date:        28/02/25
Modified:    None
Desc:        Drives the stepper motor forward for 90-degree rotation and then backward to the initial position.
Input:       char mtrVal – determines which motor to activate.
Returns:     None 
============================================================================*/
void stepperMotor(char mtrVal)
{
    mtr1.setPos = 660;  // Set position for 90-degree rotation
	mtr1.curPos = 0;

    while(mtr1.curPos < mtr1.setPos)
    {
        mtr1.patCounter++;
        
        if(mtr1.patCounter >= PATTERNCOUNT)
        {
            mtr1.patCounter = 0;		
        }
		mtr1.currentPat=patterns[mtr1.patCounter];
		mtr1.curPos += STEP;
        switch(mtrVal)
        {
            case 'A':
                STEPPERPORT1 = STEPPERMASK1 & patterns[mtr1.patCounter];
                break;
            case 'B':
                STEPPERPORT2 = STEPPERMASK1 & patterns[mtr1.patCounter];
                break;
            case 'C':
                STEPPERPORT3 = STEPPERMASK1 & patterns[mtr1.patCounter];
                break;
            default:
                break;  
        }
        Delay1KTCYx(2);
    }
    Delay10KTCYx(500); // Wait for 10 seconds
	Delay10KTCYx(250);
	
	mtr1.setPos=0;
    // Move backward to initial position
    while(mtr1.curPos > mtr1.setPos)
    {
		mtr1.patCounter--;
        if(mtr1.patCounter < 0)
        {
            mtr1.patCounter = PATTERNCOUNT - 1;
        }
        
           
     
		mtr1.currentPat=patterns[mtr1.patCounter];
        mtr1.curPos -= STEP;
        switch(mtrVal)
        {
            case 'A':
                STEPPERPORT1 = STEPPERMASK1 & patterns[mtr1.patCounter];
                break;
            case 'B':
                STEPPERPORT2 = STEPPERMASK1 & patterns[mtr1.patCounter];
                break;
            case 'C':
                STEPPERPORT3 = STEPPERMASK1 & patterns[mtr1.patCounter];
                break;
            default:
                break;   
        }
		
        Delay1KTCYx(2);
    }
	
    Delay10KTCYx(500);
}

/*>>> systemInitialization: =============================================================
Author:      AnanthaKrishnan R
Date:        09/08/24
Modified:    None
Desc:        Initializes all system functions.
Input:       None 
Returns:     None 
============================================================================*/
void systemInitialization()
{
    setOsc4MHz();
    configbits();
    configTMR0();
    configUSART();
    configADCON();
    configInts();
}//systemInitialization:

/*=== MAIN: FUNCTION ============================================================
============================================================================*/
void main(void)
{
    char str1[] = "Sorting Counts";  // Static text for LCD first line
    unsigned char index = 0;
	char buf = 0;

    systemInitialization();
    initializeStepper(&mtr1);
    initLcd();  // Initialize the LCD

    while(TRUE)
    {  
		//mtrVal = 'C';
        // Process the received character only once when valid (A, B, or C)
        if(mtrVal == 'A'|| mtrVal == 'B'|| mtrVal == 'C')
        {
			buf = mtrVal;
            stepperMotor(mtrVal);
			
        
            switch(mtrVal)
            {
                case 'A':
                    count1++;
					puts1USART(buf);
                    break;
                case 'B':
                    count2++;
					puts1USART(buf);
                    break;
                case 'C':
                    count3++;
					puts1USART(buf);
                    break;
                default:
                    break;
            }
            
            mtrVal = 0;  // Reset mtrVal so the motor doesn't continuously move
        }
              
        sndCmd(0x38);  // Set 8-bit data length and 2-line display for LCD
        
        // Display the first line of text
        for (index = 0; str1[index] != '\0'; index++)
        {
            sndData(str1[index]);
        }
        sprintf(str2, "C1:%d  C2:%d  C3:%d ", count1, count2, count3);
		
    
        sndCmd(0xC0);  // Move to the second line
        for (index = 0; str2[index] != '\0'; index++)
        {
            sndData(str2[index]);
        }
        Delay1KTCYx(DELAYCOUNT);  // Wait before updating the screen again
    }
}//eo main
