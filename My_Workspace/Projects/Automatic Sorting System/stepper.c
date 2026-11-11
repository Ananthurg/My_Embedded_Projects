/*=============================================================================
    File Name:    ELNC6007Template.c  
    Author:        Sachin Denny
    Date:        07/06/2024
    Modified:    None
    © Fanshawe College, 2024

    Description: 
=============================================================================*/

/* Preprocessor ===============================================================
   Hardware Configuration Bits ==============================================*/
#pragma config FOSC        = INTIO67
#pragma config PLLCFG    = OFF
#pragma config PRICLKEN = ON
#pragma config FCMEN    = OFF
#pragma config IESO        = OFF
#pragma config PWRTEN    = OFF 
#pragma config BOREN    = ON
#pragma config BORV        = 285 
#pragma config WDTEN    = OFF
#pragma config PBADEN    = OFF
#pragma config LVP        = OFF
#pragma config MCLRE    = EXTMCLR

// Libraries ==================================================================
#include <p18f45k22.h>
#include <stdio.h>
#include <stdlib.h>
#include <delays.h>

// Constants  =================================================================
#define TRUE    1    
#define FALSE    0
#define lcdE PORTAbits.RA6
#define lcdRS PORTAbits.RA5
#define DELAYCOUNT 2


// Global Variables  ==========================================================

unsigned char strFirst[5]={'G','R','O','U','P'};
unsigned char strSecond[7]={'P','R','O','J','E','C','T'};
// Functions  =================================================================
/*>>> functionName:portConfig ===========================================================
Author:        Sachin Denny
Date:        07/06/2024
Modified:    None
Desc:        function to configure the IO port modules.
Input:         None 
Returns:    None 
 ============================================================================*/
void portConfig(void)
{
    ANSELA=0x0F;//Set all pins on port A to Digital oprtation
    LATA=0x00;//make it low
    TRISA=0X0F;//Lower port is set as input and higher as output

    ANSELB=0x00;//Set all pins on port A to Digital oprtation
    LATB=0x00;//make it low
    TRISB=0XF0;//configure as input port

    ANSELC=0x00;//Set all pins on port A to Digital oprtation
    LATC=0x00;//make it low
    TRISC=0XFF;//configure as input port

    ANSELD=0x00;//Set all pins on port A to Digital oprtation
    LATD=0x00;//make it low
    TRISD=0X00;//configure as output port    
    
} // eo portConfig::


void setOsc4Mhz(void)
{
    OSCCON=0x52;//set to 4 Mhz
    while(!OSCCONbits.HFIOFS);
}//eo setOsc4Mhz::

void sndCmd(unsigned char cmd)
{
    lcdRS=FALSE;
    Delay1KTCYx(DELAYCOUNT);
    lcdE=TRUE;
    Delay1KTCYx(DELAYCOUNT);
    PORTD = cmd;
    Delay1KTCYx(DELAYCOUNT);
    lcdE=FALSE;
    Delay1KTCYx(DELAYCOUNT);
}
void sndData(unsigned char data)
{
    lcdRS=TRUE;
    //Delay1KTCYx(DELAYCOUNT);
    lcdE=TRUE;
    //Delay1KTCYx(DELAYCOUNT);
    PORTD = data;
    //Delay1KTCYx(DELAYCOUNT);
    lcdE=FALSE;
    //Delay1KTCYx(DELAYCOUNT);
}
void initLcd ()
{
    sndCmd(0x38);
    sndCmd(0x0E);
    sndCmd(0x01);
}
 void systemInitialization(void)
 {
     portConfig();
     setOsc4Mhz();
    
 }

/*=== MAIN: FUNCTION ==========================================================
 ============================================================================*/
void main(void)
{
    char str1[] = "Sorting Counts";  // Static text
    unsigned char count1 = 1;  // Dynamic count value 1
    unsigned char count2 = 3 ;  // Dynamic count value 2
    unsigned char count3 = 8;  // Dynamic count value 3
    char str2[50] = {"C1:4  C2:6  C3:7  "};  // Buffer to hold the formatted string for counts
    unsigned char index = 0;
    
    systemInitialization();  // Initialize system and ports
    initLcd();  // Initialize the LCD

    while (TRUE)
    {
        // Prepare the formatted string with updated count values
        //sprintf(str2, "C1:%i C2:%i C3:%i  ", count1, count2, count3);
        
        sndCmd(0x38);  // Set 8-bit data length and 2-line display
        
        // Display the first line of text
        for (index = 0; str1[index] != '\0'; index++)  // Loop through the string until null-terminator
        {
            sndData(str1[index]);  // Send each character to the LCD
        }

        // Display the second line with the dynamic count values
        str2[3] =count1 +'0';
        str2[9] =count2 +'0';
        str2[15] =count3 +'0';
        
        sndCmd(0xC0);  // Move to the second line (if needed)
        for (index = 0; str2[index] != '\0'; index++)  // Loop through the string until null-terminator
        {
            sndData(str2[index]);  // Send each character to the LCD
        }

        // Optional: Add a small delay to make the display readable
        Delay1KTCYx(DELAYCOUNT);  // Wait before updating the screen again
        
        // Optionally update the count values (e.g., increment or change them)
        
        
        // You can also add logic to reset or update the counts based on some conditions
    }
}

