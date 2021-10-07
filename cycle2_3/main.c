/*******************************************************
This program was created by the
CodeWizardAVR V3.12 Advanced
Automatic Program Generator
© Copyright 1998-2014 Pavel Haiduc, HP InfoTech s.r.l.
http://www.hpinfotech.com

Project : 
Version : 
Date    : 9/17/2021
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

#include <mega328p.h>

// Alphanumeric LCD functions
#include <alcd.h>
#include <delay.h>

// Declare your global variables here

// Standard Input/Output functions
#include <stdio.h>          

void init(void) {
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
PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

// Port C initialization
// Function: Bit6=In Bit5=In Bit4=In Bit3=In Bit2=Out Bit1=Out Bit0=Out 
DDRC=(0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (1<<DDC2) | (1<<DDC1) | (1<<DDC0);
// State: Bit6=T Bit5=T Bit4=T Bit3=T Bit2=0 Bit1=0 Bit0=0 
PORTC=(0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);

// Port D initialization
// Function: Bit7=In Bit6=In Bit5=In Bit4=In Bit3=In Bit2=In Bit1=In Bit0=In 
DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
// State: Bit7=T Bit6=T Bit5=T Bit4=T Bit3=T Bit2=T Bit1=T Bit0=T 
PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

// USART initialization
// Communication Parameters: 8 Data, 1 Stop, No Parity
// USART Receiver: Off
// USART Transmitter: On
// USART0 Mode: Asynchronous
// USART Baud Rate: 9600
UCSR0A=(0<<RXC0) | (0<<TXC0) | (0<<UDRE0) | (0<<FE0) | (0<<DOR0) | (0<<UPE0) | (0<<U2X0) | (0<<MPCM0);
UCSR0B=(0<<RXCIE0) | (0<<TXCIE0) | (0<<UDRIE0) | (0<<RXEN0) | (1<<TXEN0) | (0<<UCSZ02) | (0<<RXB80) | (0<<TXB80);
UCSR0C=(0<<UMSEL01) | (0<<UMSEL00) | (0<<UPM01) | (0<<UPM00) | (0<<USBS0) | (1<<UCSZ01) | (1<<UCSZ00) | (0<<UCPOL0);
UBRR0H=0x00;
UBRR0L=0x33;

}
void main(void)
{
// Declare your local variables here

    unsigned int keys = 0;
    char cur_keys = 0, tmp, t;
    char buf[100], rows[4], cur_rows[4];
    char i, j;
    init();
    // Alphanumeric LCD initialization
    // Connections are specified in the
    // Project|Configure|C Compiler|Libraries|Alphanumeric LCD menu:
    // RS - PORTC Bit 0
    // RD - PORTC Bit 1
    // EN - PORTC Bit 2
    // D4 - PORTD Bit 4
    // D5 - PORTD Bit 5
    // D6 - PORTD Bit 6
    // D7 - PORTD Bit 7
    // Characters/line: 8
    lcd_init(20);
    lcd_gotoxy(0, 0);
    //lcd_puts("Microntrollers21");

    //lcd_gotoxy(0, 1);
    //lcd_puts("Peshkov");

    DDRB = 0xF0;

    for (i = 0; i < 4; i++) { //load initial state
        PORTB = ~(1 << (i+4));
        rows[i] = PINB;
        delay_ms(10);
    }



    while (1)
    {
        for (i = 0; i < 4; i++) { //load initial state
            PORTB = ~(1 << (i+4));
            cur_rows[i] = PINB;
            delay_ms(10);
        }

        for (i = 0; i < 4; i++) {
            if (rows[i] != cur_rows[i]) {
                keys = rows[i];
                cur_keys = cur_rows[i];
                for (j = 0; j < 4; j++) {
                    t = ((cur_keys&1)<<1) | (keys&1);
                    switch (t) {
                        case 0b10: 
                            lcd_gotoxy(0, 0);
                            sprintf(buf, "BUTTON %3d UP  ", i*4 + j);  
                            lcd_puts(buf);
                            break;	
                        case 0b01:           
                            lcd_gotoxy(0, 0);
                            sprintf(buf, "BUTTON %3d DOWN", i*4 + j);  
                            lcd_puts(buf);
                            break;
                    }
                    keys>>=1;
                    cur_keys>>=1;
                } 
                rows[i] = cur_rows[i];
            }
        }
    }
}
