#include <mega328p.h>
#include <math.h>
#include <delay.h>

unsigned long int millis = 0;
interrupt [TIM0_COMPA] void timer0_compa_isr(void) {
	++millis;
}

unsigned int adc_data[2];
#define ADC_VREF_TYPE ((0<<REFS1) | (0<<REFS0) | (0<<ADLAR))

interrupt [ADC_INT] void adc_isr(void) {
    static unsigned char input_index=0;
    input_index += 1;
    input_index &= 1;

    adc_data[input_index]=ADCW;        

    ADMUX=(input_index | ADC_VREF_TYPE);
    ADCSRA|=(1<<ADSC);
}

void send_data (unsigned int data) {
    unsigned char data1 = (data >> 8) & 0xFF;
    unsigned char data2 = data & 0xFF;
    PORTB.2 = 0;
    SPDR = data1;
    while (!(SPSR & (1<<SPIF)));  
    SPDR = data2;
    while (!(SPSR & (1<<SPIF))); 
    PORTB.2 = 1;
}
  
#define SAW	0
#define SIN	1
#define MIN_MOD		300
#define MAX_MOD		10000.f

void main(void) {
    unsigned int tmp, sig_mod;
    float t;
        
    #pragma optsize-
    CLKPR=(1<<CLKPCE);
    CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
    #ifdef _OPTIMIZE_SIZE_
    #pragma optsize+
    #endif
 
    DDRB=(0<<DDB7) | (0<<DDB6) | (1<<DDB5) | (0<<DDB4) | (1<<DDB3) | (1<<DDB2) | (0<<DDB1) | (0<<DDB0);
    PORTB=(0<<PORTB7) | (0<<PORTB6) | (0<<PORTB5) | (0<<PORTB4) | (0<<PORTB3) | (0<<PORTB2) | (0<<PORTB1) | (0<<PORTB0);
    DDRC=(0<<DDC6) | (0<<DDC5) | (0<<DDC4) | (0<<DDC3) | (0<<DDC2) | (0<<DDC1) | (0<<DDC0);
    PORTC=(0<<PORTC6) | (0<<PORTC5) | (0<<PORTC4) | (0<<PORTC3) | (0<<PORTC2) | (0<<PORTC1) | (0<<PORTC0);
    DDRD=(0<<DDD7) | (0<<DDD6) | (0<<DDD5) | (0<<DDD4) | (0<<DDD3) | (0<<DDD2) | (0<<DDD1) | (0<<DDD0);
    PORTD=(0<<PORTD7) | (0<<PORTD6) | (0<<PORTD5) | (0<<PORTD4) | (0<<PORTD3) | (0<<PORTD2) | (0<<PORTD1) | (0<<PORTD0);

    // Timer/Counter 0 initialization
    // Clock source: System Clock
    // Clock value: 1000.000 kHz
    // Mode: CTC top=OCR0A
    // OC0A output: Disconnected
    // OC0B output: Disconnected
    // Timer Period: 0.1 ms
    TCCR0A=(0<<COM0A1) | (0<<COM0A0) | (0<<COM0B1) | (0<<COM0B0) | (1<<WGM01) | (0<<WGM00);
    TCCR0B=(0<<WGM02) | (0<<CS02) | (1<<CS01) | (0<<CS00);
    TCNT0=0x00;
    OCR0A=0x63;
    OCR0B=0x00;

    // Timer/Counter 0 Interrupt(s) initialization
    TIMSK0=(0<<OCIE0B) | (1<<OCIE0A) | (0<<TOIE0);

    // ADC initialization
    // ADC Clock frequency: 1000.000 kHz
    // ADC Voltage Reference: AREF pin
    // ADC Auto Trigger Source: Free Running
    // Digital input buffers on ADC0: On, ADC1: On, ADC2: On, ADC3: On
    // ADC4: On, ADC5: On
    DIDR0=(0<<ADC5D) | (0<<ADC4D) | (0<<ADC3D) | (0<<ADC2D) | (0<<ADC1D) | (0<<ADC0D);
    ADMUX= ADC_VREF_TYPE;
    ADCSRA=(1<<ADEN) | (1<<ADSC) | (1<<ADATE) | (0<<ADIF) | (1<<ADIE) | (1<<ADPS2) | (1<<ADPS1) | (1<<ADPS0);
    ADCSRB=(0<<ADTS2) | (0<<ADTS1) | (0<<ADTS0);

    // SPI initialization
    // SPI Type: Master
    // SPI Clock Rate: 2*2000.000 kHz
    // SPI Clock Phase: Cycle Start
    // SPI Clock Polarity: Low
    // SPI Data Order: MSB First
    SPCR=(0<<SPIE) | (1<<SPE) | (0<<DORD) | (1<<MSTR) | (0<<CPOL) | (0<<CPHA) | (0<<SPR1) | (0<<SPR0);
    SPSR=(1<<SPI2X);

	#asm("sei")
     
    while (1) {  
    	//adc_data is the amplitude of signal
        sig_mod = MAX_MOD * (adc_data[1] / 1024.0);
        t = (millis % sig_mod) / (float)sig_mod;
        switch(PIND.0) {
        case SAW:      
        	tmp = t * adc_data[0];
        	break; 
        case SIN:
            tmp = (sin(2*PI*t) + 1.0) / 2.0 * adc_data[0];
        	break;
        }
        
        tmp <<= 2;    
        tmp |= 0x3000;
        
        send_data(tmp);
    }
}
