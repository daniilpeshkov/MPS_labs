#include <mega328p.h>
#include <stdio.h>
#include <alcd.h>
#include <twi.h>
#include <ds1621_twi.h>
#include <delay.h>

char lcd_buf[50];

void sprint_temp(char *buf, int temp, int index) {
    char t_f, t_i, neg = ' '; 
	t_f = temp % 10;                        
	t_i = (temp / 10);
    if ((t_i < 0) && (t_f != 0)) {
    	t_i += 1;
        if (t_i == 0) neg = '-';
    }       
    sprintf(buf, "t%d = %c%d.%d", index, neg, t_i, t_f);
}

void main(void) {
    // Declare your local variables here
    int t0, t1;   
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
    PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);

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

    // TWI initialization
    // Mode: TWI Master
    // Bit Rate: 100 kHz
    twi_master_init(100);

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
#define ADDR_1 1
#define ADDR_2 4
	
	ds1621_init(ADDR_1, 30, 35, 0);           
	ds1621_init(ADDR_2, 30, 35, 0);           
    
    while (1) {   

    	delay_ms(100);              
               
        t0 = ds1621_temperature_10(ADDR_1);
        t1 = ds1621_temperature_10(ADDR_2);
        lcd_clear();
    	lcd_gotoxy(0, 0); 
        sprint_temp(lcd_buf, t0, 0);
    	 
        lcd_puts(lcd_buf);
        lcd_gotoxy(0, 1);
                                    
        sprint_temp(lcd_buf, t1, 1);
        lcd_puts(lcd_buf);
    }
}
