/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
? Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 10/15/2021
Author  : 
Company : 
Comments: 


Chip type               : ATmega328P
Program type            : Application
AVR Core Clock frequency: 8.000000 MHz
Memory model            : Small
External RAM size       : 0
Data Stack size         : 512
*******************************************************/
typedef unsigned char uint8_t;

#include <mega328p.h>
#include <delay.h>

// Alphanumeric LCD functions
#include <alcd.h>
#include <stdio.h>

// Declare your global variables here

uint8_t sec = 0;
uint8_t min = 0;
uint8_t h = 0;

// Timer2 overflow interrupt service routine
interrupt [TIM2_OVF] void timer2_ovf_isr(void)
{
// Place your code here
	sec += 1;
    if (sec == 60) {
    	sec = 0;
        min += 1;
        if (min == 60) {
        	min = 0;
            h += 1;
            if (h == 12) {
            	h = 0;
            }
        }
    
    }

}

void main(void)
{
// Declare your local variables here
char buf[100];
char mode = 0;
char tmp, btn_mode = 1, btn_up = 1, btn_down = 1;
// Crystal Oscillator division factor: 1
#pragma optsize-
CLKPR=(1<<CLKPCE);
CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
#ifdef _OPTIMIZE_SIZE_
#pragma optsize+
#endif
// Input/Output Ports initialization
// Port B initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRB=(0<<DDB7) | (0<<DDB6) | (0<<DDB5) | (0<<DDB4) | (0<<DDB3) | (0<<DDB2) | (0<<DDB1) | (0<<DDB0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (1<<PORTB2) | (1<<PORTB1) | (1<<PORTB0);

// Port C initialization
// Function: Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRC=(0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
// State: Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTC=(0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

// Timer/Counter 2 initialization
// Clock source: Crystal on TOSC1 pin
// Clock value: PCK2/128
// Mode: Normal top=0xFF
// OC2A output: Disconnected
// OC2B output: Disconnected
ASSR=(0<<EXCLK) | (1<<AS2);
TCCR2A=(0<<COM2A1) | (0<<COM2A0) | (0<<COM2B1) | (0<<COM2B0) | (0<<WGM21) | (0<<WGM20);
TCCR2B=(0<<WGM22) | (1<<CS22) | (0<<CS21) | (1<<CS20);
TCNT2=0x00;
OCR2A=0x00;
OCR2B=0x00;

// Timer/Counter 2 Interrupt(s) initialization
TIMSK2=(0<<OCIE2B) | (0<<OCIE2A) | (1<<TOIE2);

// Alphanumeric LCD initialization
// Connections are specified in the
// Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
// RS - PORTD Bit 0
// RD - PORTD Bit 1
// EN - PORTD Bit 2
// D4 - PORTD Bit 4
// D5 - PORTD Bit 5
// D6 - PORTD Bit 6
// D7 - PORTD Bit 7
// Characters/line: 20
lcd_init(20);

// Global enable interrupts
#asm("sei")

while (1)
      {    
      		tmp = PINB & 1;
            
            if (tmp != btn_mode && tmp == 0) {
             	mode += 1;
                if (mode == 4) mode = 0;
            }
            btn_mode = tmp;
                           
            
      		tmp = (PINB & 2) >> 1;
            
            if (tmp != btn_up && tmp == 0) {
            	
            	switch (mode) {
            		case 1:
                    	h += 1;
                        if (h > 12) h = 0;
                        break;
                    case 2:
                    	min += 1;
                        if (min == 60) min = 0;
                        break;
                    case 3:  
                    	#asm("cli");
                    	sec += 1;
                        if (sec == 60) sec = 0;   
                        #asm("sei");           
                        break;
                }                   
            }
            btn_up = tmp;   
            
            
            tmp = (PINB & 4) >> 2;
            
            if (tmp != btn_down && tmp == 0) {
            	
            	switch (mode) {
            		case 1:
                    	if (h == 0) h = 12;
                        else h -= 1;
                        break;
                    case 2:
                    	if (min == 0) min = 59;
                        else min -= 1;
                        break;
                    case 3:      
                    	#asm("cli");
                    	if (sec == 0) sec = 59;
                        else sec -= 1;      
                        #asm("sei");
                        break;
                }                   
            }
            btn_down = tmp;
            
                        
      		lcd_gotoxy(0,0);
      		
      		sprintf(buf, "%02d:%02d:%02d", h, min, sec);
            lcd_puts(buf);
      		delay_ms(10);
      
            if (mode) {              
             	switch(mode) {
                    case 1:
                        lcd_gotoxy(1,1);
                        break;
                    case 2:   
                        lcd_gotoxy(1,1);
                        lcd_puts(" ");
                        lcd_gotoxy(4,1);
                        break;      
                    case 3:       
                    	lcd_gotoxy(4,1);
                        lcd_puts(" ");
                    	lcd_gotoxy(7,1);
                        break;
                   
            	}
                lcd_puts("^");
            } else {
             	lcd_gotoxy(7,1);
                lcd_puts(" ");
            }  
            
            delay_ms(100);                   
            
      }
}
