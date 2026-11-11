#line 1 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X\LCD.c"
#line 1 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X\LCD.c"
#line 1 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X/LCD.h"





#line 7 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X/LCD.h"

#line 9 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X/LCD.h"
#line 10 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X/LCD.h"
#line 11 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X/LCD.h"
#line 12 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X/LCD.h"
#line 13 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X/LCD.h"
#line 14 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X/LCD.h"
#line 15 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X/LCD.h"
#line 16 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X/LCD.h"
#line 17 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X/LCD.h"
#line 18 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X/LCD.h"
#line 19 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X/LCD.h"

 

void LCD_Init();
void LCD_Clear();
void LCD_SL();
void LCD_SR();
void LCD_CMD(unsigned char);
void LCD_DATA(unsigned char);
void LCD_Set_Cursor(unsigned char, unsigned char);
void LCD_Write_Char(char);
void LCD_Write_String(char*);

#line 35 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X/LCD.h"

    
    

#line 42 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X/LCD.h"

#line 44 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X/LCD.h"

#line 1 "C:\Users\AnanthuRG\Downloads\LCD_DEMO.X (1)\LCD_DEMO.X\LCD.c"


void LCD_DATA(unsigned char Data)
{
  if(Data & 1)
    PORTDbits.RD4  = 1;
  else
    PORTDbits.RD4  = 0;
  if(Data & 2)
    PORTDbits.RD5  = 1;
  else
    PORTDbits.RD5  = 0;
  if(Data & 4)
    PORTDbits.RD6  = 1;
  else
    PORTDbits.RD6  = 0;
  if(Data & 8)
    PORTDbits.RD7  = 1;
  else
    PORTDbits.RD7  = 0;
}
void LCD_CMD(unsigned char CMD)
{
  
  PORTAbits.RA7  = 0;
  
  LCD_DATA(CMD);
  
  PORTAbits.RA6  = 1;
  __delay_us(500 );
  PORTAbits.RA6  = 0;
}
void LCD_Clear()
{
  LCD_CMD(0);
  LCD_CMD(1);
}
void LCD_Set_Cursor(unsigned char r, unsigned char c)
{
  unsigned char Temp,Low4,High4;
  if(r == 1)
  {
    Temp = 0x80 + c - 1; 
    High4 = Temp >> 4;
    Low4 = Temp & 0x0F;
    LCD_CMD(High4);
    LCD_CMD(Low4);
  }
  if(r == 2)
  {
    Temp = 0xC0 + c - 1;
    High4 = Temp >> 4;
    Low4 = Temp & 0x0F;
    LCD_CMD(High4);
    LCD_CMD(Low4);
  }
}
void LCD_Init()
{
  
  TRISD  = 0x00;
  TRISA7  = 0;
  TRISA6  = 0;
  
  LCD_DATA(0x00);
  __delay_ms(30);
  __delay_us(500 );
  LCD_CMD(0x03);
  __delay_ms(5);
  LCD_CMD(0x03);
  __delay_us(150);
  LCD_CMD(0x03);
  LCD_CMD(0x02);
  LCD_CMD(0x02);
  LCD_CMD(0x08);
  LCD_CMD(0x00);
  LCD_CMD(0x0C);
  LCD_CMD(0x00);
  LCD_CMD(0x06);
}
void LCD_Write_Char(char Data)
{
  char Low4,High4;
  Low4 = Data & 0x0F;
  High4 = Data & 0xF0;
  PORTAbits.RA7  = 1;
  LCD_DATA(High4>>4);
  PORTAbits.RA6  = 1;
  __delay_us(500 );
  PORTAbits.RA6  = 0;
  __delay_us(500 );
  LCD_DATA(Low4);
  PORTAbits.RA6  = 1;
  __delay_us(500 );
  PORTAbits.RA6  = 0;
  __delay_us(500 );
}
void LCD_Write_String(char *str)
{
  int i;
  for(i=0;str[i]!='\0';i++)
    LCD_Write_Char(str[i]);
}
void LCD_SL()
{
  LCD_CMD(0x01);
  LCD_CMD(0x08);
}
void LCD_SR()
{
  LCD_CMD(0x01);
  LCD_CMD(0x0C);
}
