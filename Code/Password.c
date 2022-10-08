/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : Password
Version : 1.0
Date    : 10/7/2022
Author  : Aidin Esmkhani
Company : 
Comments: 


Chip type               : ATmega16
Program type            : Application
AVR Core Clock frequency: 1.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 256
*******************************************************/

#include <mega16.h>
#include "Define.h"
#include <stdio.h>
#include <stdlib.h>
#include <delay.h>
#include <stdbool.h>
#include <string.h>
#include <eeprom.h>
// Alphanumeric LCD functions
#include <alcd.h>

// Declare your global variables here
char KeyCode = -1;
char sys_code[5] = "1234";
char new_code[5] = "";
char user_code[sizeof(sys_code)]= "";
char user_code_idx = 0;
bool go = false, new = false, ok = false;
int address = 0, state = 1;
// Declare your function prototype here
void Configurations(void);
int ReadKeyPad(void);
void EnableKeyPad(void);
void DisableKeyPad(void);
// External Interrupt 0 service routine
interrupt [EXT_INT0] void ext_int0_isr(void)
{
// Place your code here

}

void main(void)
{
// Declare your local variables here

Configurations();

while (1)
      {
      // Place your code here

      }
}
//**********************************ScanKeypad**************************************
int ReadKeyPad(void)
{
    char temp = 0;
    temp = PINB & 0x0F;
    switch(temp)
        {
        case 0: // Key 7 
            return Key_7_Val;
        case 1: // Key 4 
            return Key_4_Val;
        case 2: // Key 3 
            return Key_1_Val;
        case 3: // Key Enter   
            return Key_Star_Val;
        case 4: // Key 8
            return Key_8_Val;
        case 5: // Key 5
            return Key_5_Val;
        case 6: // Key 2
            return Key_2_Val;
        case 7: // Key Zero
            return Key_0_Val;
        case 8: // Key 9
            return Key_9_Val;
        case 9: // Key 6
            return Key_6_Val;
        case 10: // Key 3
            return Key_3_Val;
        case 11: // Delete
            return Key_Sharp_Val;
        case 12: // Key Enter
            return  Key_A_Val;
        case 13: // Key Esc
            return Key_B_Val;
        case 14: // Key C
            return Key_C_Val;     
        case 15: // MEnu
            return Key_D_Val;  
        default:
            break;
        }
}
//*****************************Ask for Confirm***************************************
int AskYesNo(char msg[20])
{
  int MenuItemCnt = 2, Pointer = 0,i = 0;
  char *MenuItems[2] = {"1 Yes", "2 No"};
  bool Refresh = true;
  delay_ms(200); 
  while(1)
  {
    if(Refresh == true)
    { 
       delay_ms(100);
       Refresh = false;
       lcd_clear();
       lcd_puts(msg);
       for(i = 0; i < MenuItemCnt; i++)
       {
          lcd_gotoxy(0,i+1);
          lcd_puts(MenuItems[i]);
       }
       if(Pointer > 1)
          Pointer = 1;
       else if(Pointer < 0)
          Pointer = 0;
       lcd_gotoxy(1,Pointer + 1);
       lcd_putchar('>');
    }
    if(KeyCode == Key_Down_Val)            //Down
    {
        KeyCode = -1;
        Pointer++;
        if(Pointer > 1)
           Pointer = 1;
        Refresh = true;
    }
    else if(KeyCode == Key_Up_Val)       //Up
    {
        KeyCode = -1;
        Pointer--;
        if(Pointer < 0)
           Pointer = 0;
        Refresh = true;
    }
    else if(KeyCode == Key_Enter_Val)  //Enter
    {
        KeyCode = -1;
        lcd_clear();
        return Pointer + 1;
    }
    delay_ms(10);
  }
}
//***********************Print numeric on LCD*********************************************
void printNumbers(unsigned char keycode)
{
    char lcd_buffer[1] = {0};   
    lcd_gotoxy(2,2);
    lcd_puts("Key   Pressred");
    if(keycode >= 0 && keycode <= 9)
       {
            sprintf(lcd_buffer, "%d", keycode);
            lcd_gotoxy(6,2);
            lcd_puts(lcd_buffer); 
       }
}
//****************************Char to Integer******************************************
int convert_to_int(char data[5])
{
    int value = 0;
    value = atoi(data);
    return value;
}
//******************************Write To EEPROM**************************************
void WriteToMemory(int address, int data)
{
 int counter = 0;
 if(data > 255)
    {    
        while(data > 255)
            {
                data = data - 255;
                counter++; 
            }
    }  
    eeprom_write_byte(address, counter);
    delay_us(2);
    eeprom_write_byte(address + 1, data); 
    delay_us(2);
}
//********************************Read From EEPROM*************************************
int ReadFromMemory(int address)
{
 int counter = 0, data = 0;
 counter = eeprom_read_byte(address);  
 delay_us(2);
 data = eeprom_read_byte(address + 1);
 delay_us(2);
 data = data + (counter * 255); 
 return data;
}
//***************************Enter Password Function*******************************************
bool EnterPassword(void)
{
int ReadMemory = 0, i = 0;
unsigned char number = 0;
unsigned char system_code[5] = {0};
ReadMemory = ReadFromMemory(address);
itoa(ReadMemory, system_code);

lcd_clear();
lcd_gotoxy(0,0);
lcd_puts("Enter Password :");   

  while(1)
  {
    if(KeyCode != -1)
       {  
          number = KeyCode;
          KeyCode = -1;
          if(number == Key_Enter_Val)
             {  
                DisableKeyPad();
                user_code[user_code_idx] = '\0';
                if(strcmp(user_code,system_code) == 0)
                    { 
                        lcd_clear();
                        delay_ms(500);
                        lcd_clear();
                        lcd_gotoxy(2,0);
                        lcd_puts("System is Active");
                        KeyCode = -1; 
                        EnableKeyPad();
                        return true; 
                    }
                 else
                    {  
                        lcd_clear();
                        lcd_gotoxy(1,1);
                        lcd_puts("Incorrect PassWord");     
                        strcpy(user_code,"");
                        user_code_idx = 0;
                        delay_ms(2000);
                        return false;
                    }  
                  strcpy(user_code,"");
                  user_code_idx = 0;               
             }
           else if(number <= 9 && number >= 0)
             {
              if(user_code_idx < (sizeof(user_code)-1))
                {
                    user_code[user_code_idx] = number + 0x30;
                    lcd_gotoxy(user_code_idx + 8,1); 
                    lcd_putchar(user_code[user_code_idx]);
                    user_code_idx++;
                }
             } 
       }
  }
}
//**********************************Configurations*********************************
void Configurations(void)
{
// Input/Output Ports initialization
// Port A initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRA=(0<<DDA7) | (0<<DDA6) | (0<<DDA5) | (0<<DDA4) | (0<<DDA3) | (0<<DDA2) | (0<<DDA1) | (0<<DDA0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTA=(0<<PORTA7) | (0<<PORTA6) | (0<<PORTA5) | (0<<PORTA4) | (0<<PORTA3) | (0<<PORTA2) | (0<<PORTA1) | (0<<PORTA0);

// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

// Port C initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=Out 
DDRC=(0<<DDC7) | (0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (1<<DDC0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=0 
PORTC=(0<<PORTC7) | (0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

// External Interrupt(s) initialization
// INT0: Off
// INT0 Mode: Falling Edge
// INT1: Off
// INT2: Off
GICR|=(0<<INT1) | (0<<INT0) | (0<<INT2);
MCUCR=(0<<ISC11) | (0<<ISC10) | (1<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);
GIFR=(0<<INTF1) | (1<<INTF0) | (0<<INTF2);

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTA Bit 0
// RD - PORTA Bit 1
// EN - PORTA Bit 2
// D4 - PORTA Bit 4
// D5 - PORTA Bit 5
// D6 - PORTA Bit 6
// D7 - PORTA Bit 7
// Characters/line: 20
lcd_init(20);

// Global enable interrupts
#asm("sei")
}
//************************Enabling the Encoder to scan the keypad********************************************
void EnableKeyPad(void)
{
// External Interrupt(s) initialization
// INT0: On
// INT0 Mode: Falling Edge
// INT1: Off
// INT2: Off
GICR|=(0<<INT1) | (1<<INT0) | (0<<INT2);
MCUCR=(0<<ISC11) | (0<<ISC10) | (1<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);
GIFR=(0<<INTF1) | (1<<INTF0) | (0<<INTF2);
}
//******************************Disabling the keypad**************************************
void DisableKeyPad(void)
{
// External Interrupt(s) initialization
// INT0: Off
// INT0 Mode: Falling Edge
// INT1: Off
// INT2: Off
GICR|=(0<<INT1) | (0<<INT0) | (0<<INT2);
MCUCR=(0<<ISC11) | (0<<ISC10) | (1<<ISC01) | (0<<ISC00);
MCUCSR=(0<<ISC2);
GIFR=(0<<INTF1) | (1<<INTF0) | (0<<INTF2);
}