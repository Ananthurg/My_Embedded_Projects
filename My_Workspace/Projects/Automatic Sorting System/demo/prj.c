/*Use of AI / Cognitive Assistance Software is not allowed in any evaluation, assessment or exercise.*/
/*=============================================================================
	File Name:	ELNC6007SSLAB6.c  
	Author:		SHAMIN SHAJU
	Date:		21/06/24
	Modified:	None
	© Fanshawe College, 2024

	Description: 
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


// Constants  =================================================================
#define TRUE	1	
#define FALSE	0
#define PBMASK	0xF0
#define NOPRESS	0x00
#define PB1PRESS	0xE0
#define PB2PRESS	0xD0
#define PB3PRESS	0xB0
#define PB4PRESS	0x70
#define LED1	LATDbits.LATD0
#define LED2	LATDbits.LATD1
#define LED3	LATDbits.LATD2
#define LED4	LATDbits.LATD3
#define	DELAYCOUNT	50
#define LEVEL1 128
#define LEVEL2 256
#define LEVEL3 512
#define LEVEL4 768
#define LEDMASK	0x0F
#define T0FLAG	INTCONbits.TMR0IF
#define RC2FAG	PIRbits.RC2IF
#define RC2READ	RCREG2
#define MAXSEC	60
#define	MAXMIN	60
#define MAXHOUR	12

#define PWMTRIS TRISCbits.TRISC2
#define FREQPWM 1000
#define PERIODPWM 1/FREQPWM
#define PSVPWM 4
#define FOSC 4
#define DCPER   100



// LCD Data Pins (D4-D7)
#define D4 LATBbits.LATB4
#define D5 LATBbits.LATB5
#define D6 LATBbits.LATB6
#define D7 LATBbits.LATB7

#define LCDPORT LATD
#define RS LATCbits.LATC4
#define	RW	LATCbits.LATC3
#define E LATCbits.LATC5




// Global Variables  ==========================================================
	char morse_code[] = {
    ".-", "-...", "-.-.", "-..", ".", "..-.", "--.", "....", "..", ".---",
    "-.-", ".-..", "--", "-.", "---", ".--.", "--.-", ".-.", "...", "-",
    "..-", "...-", ".--", "-..-", "-.--", "--.."};
    
const char characters[] = {'A','B','C','D','E','F','G','H','I','J','K','L','M','N','O','P','Q','R','S','T','U','V','W','X','Y','Z'};
char pbs=0;



// Functions  =================================================================

/*>>> setOsc4MHz: =============================================================
Author:		SHAMIN SHAJU
Date:		21/06/24
Modified:	None
Desc:		This function setting the oscillator to 4 MHz 
Input: 		None 
Returns:	None 
 ============================================================================*/
void setOsc4MHz(void)
{
	OSCCON=0x52;
	while(!OSCCONbits.HFIOFS);	
} // eo setOsc4MHz::


/*>>> configPort: =============================================================
Author:		SHAMIN SHAJU
Date:		21/06/24
Modified:	None
Desc:	 
Input: 		None 
Returns:	None 
 ============================================================================*/
void configPort(void)
{
	ANSELA=0x01;
	LATA=0x00;
	TRISA=0xFF;	

	ANSELB=0x00;
	LATB=0x00;
	TRISB=0xFF;

	ANSELC=0x00;
	LATC=0x00;
	TRISC=0xFF;

	ANSELD=0x00;
	LATD=0x00;
	TRISD=0x00;

	ANSELE=0x00;
	LATE=0x00;
	TRISE=0xFF;
} // eo configbits::
/*>>> lcd_command: =============================================================
Author:		SHAMIN SHAJU
Date:		21/06/24
Modified:	None
Desc:	 // Function to send a command to the LCD
Input: 		None 
Returns:	None 
 ============================================================================*/
void lcd_command(unsigned char cmd)
{
	LCDPORT = cmd;      // Send command
    RS = 0;             // Select command register
    E = 1;              // Enable LCD
    Delay1KTCYx(2);     // Delay for command to be processed
    E = 0;              // Disable LCD
    Delay1KTCYx(2);     // Delay for command to be processed
}
/*>>> lcd_init: =============================================================
Author:		SHAMIN SHAJU
Date:		21/06/24
Modified:	None
Desc:	 // Function to initialize the LCD
Input: 		None 
Returns:	None 
 ============================================================================*/
 void lcd_init() {
    // Set all LCD pins as outputs
    E = 0;
	RS = 0;
	LCDPORT = 0;
	TRISD = 0;
	TRISC = 0;

	lcd_command(0x38);
	Delay1KTCYx(2);
	lcd_command(0x0F);
	Delay1KTCYx(2);
	lcd_command(0x01);
	Delay1KTCYx(2);
	lcd_command(0x81);
	Delay1KTCYx(2);
	//__delay_ms(30);
	//__delay_us(LCD_EN_Delay);

	/*lcdCommand(0x38);
	msdelay(10);
	lcdCommand(0x0C);
	msdelay(10);
	lcdCommand(0x01);
	msdelay(10);
	lcdCommand(0x06);
	msdelay(10);
	lcdCommand(0);
	msdelay(10);
	lcdCommand(1);
	Delay1KTCYx(2);
	lcdCommand(0x81);*/
}
/*>>> lcd_char: =============================================================
Author:		SHAMIN SHAJU
Date:		21/06/24
Modified:	None
Desc:	 // Function to send a character to the LCD
Input: 		None 
Returns:	None 
 ============================================================================*/
 void lcd_char(unsigned char data) 
	{
		LCDPORT = data;
	    RS = 1;
		RW=0;
		E = 1;
		Delay1KTCYx(2);
		E = 0;
		Delay1KTCYx(2);
	}
/*>>> configTMR0: =============================================================
Author:		SHAMIN SHAJU
Date:		21/06/24
Modified:	None
Desc:	 
Input: 		None 
Returns:	None 
 ============================================================================*/
void lcdString(char *stringLCD)
{
	while(*stringLCD)
	{
		lcd_char(*(stringLCD++));
	}
}	//	eo lcdString::

/*>>> configTMR0: =============================================================
Author:		SHAMIN SHAJU
Date:		21/06/24
Modified:	None
Desc:	 
Input: 		None 
Returns:	None 
 ============================================================================*/
void configTMR0(void)
{
	T0CON=0x93;
	resetTMR0();
}
/*>>>configUSART: =============================================================
Author:		SHAMIN SHAJU
Date:		07/06/24
Modified:	None
Desc:	 
Input: 		None 
Returns:	None 
 ============================================================================*/
void configUSART()
{
	SPBRG1=25;
	BAUDCON1=0x40;
	RCSTA1=0x90;
	TXSTA1=0x26;
	
}
/*>>> systemInitialization: =============================================================
Author:		SHAMIN SHAJU
Date:		07/06/24
Modified:	None
Desc:	 
Input: 		None 
Returns:	None 
 ============================================================================*/
void systemInitialization()
{
	setOsc4MHz();
	configPort();
	configTMR0();
	configUSART();
	//lcd_init();

}

/*>>>resetTMR0: =============================================================
Author:		SHAMIN SHAJU
Date:		07/06/24
Modified:	None
Desc:	 
Input: 		None 
Returns:	None 
 ============================================================================*/
int resetTMR0()
{
	T0FLAG=FALSE;
	TMR0H=0x0B;
	TMR0L=0xDC;
}
// Functions  =================================================================

/*>>> setOsc4MHz: =============================================================
Author:		SHAMIN SHAJU
Date:		21/06/24
Modified:	None
Desc:		This function setting the oscillator to 4 MHz 
Input: 		None 
Returns:	None 
 ============================================================================*/
char translate_morse_to_char(char *input)
 
{
	int i=0;
    for(i=0;i<26;i++)
	{
		if(*input==morse_code[i])
		{
			return i;
		}
	}
}

// Functions  =================================================================

/*>>> setOsc4MHz: =============================================================
Author:		SHAMIN SHAJU
Date:		21/06/24
Modified:	None
Desc:		This function setting the oscillator to 4 MHz 
Input: 		None 
Returns:	None 
 ============================================================================*/
void process_morse(char *morse_input, int *index)
{
    if (pbs==PB1PRESS)
	{  // Check if Push Button 1 (.) is pressed
        morse_input[*index] = '.';
        (*index)++;
        
  	}

    if (pbs==PB2PRESS) 
	{  // Check if Push Button 2 (-) is pressed
        morse_input[*index] = '-';
        (*index)++;
        
    }
    
	
}
/*=== MAIN: FUNCTION ==========================================================
 ============================================================================*/

void main(void)
{
    char morse_input[10];  // Buffer to store Morse code input
    char translated_char;
    int index = 0;
    char translated[] = "Translated Char:";
    
    systemInitialization();  // Initialize the system
    
    while (TRUE) 
    {
        pbs = PORTA & PBMASK;
        //process_morse(morse_input, &index);  // Capture Morse code input
	  	
		switch(pbs)
		{
			case PB1PRESS:	
				LATD=0xFF;	  
		
				morse_input[index] = '.';
				index=index+1;
		        
				//Delay10KTCYx(100);
			break;
			case PB2PRESS:
				LATD=0x00;
				morse_input[index] = '-';
				index++;
				
			//	Delay10KTCYx(100);
			break;
	        case PB3PRESS:
	 
	            translated_char = translate_morse_to_char(morse_input);  // Translate Morse code
	            index = 0;
	            lcd_command(0x01);    // Clear display
	            lcd_command(0x80);    // Move cursor to the starting of the 1st line
	            lcdString(translated); // Print "Translated Char:"
	            lcd_char(translated_char + 'A'); // Print translated character (assuming translated_char is an index)
        	break;
			default:
				LATD=0x00;
			break;
		}
    }
}
// eo main::

