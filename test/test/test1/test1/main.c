/*
 * test1.c
 *
 * Created: 9/4/2021 10:45:47 PM
 * Author : danii
 */ 

#include <avr/io.h>
#include <stdio.h>
#define F_CPU 8000000L
#include <util/delay.h>

static void uart_putc(char c)
{
	while (!(UCSR0A & (1<<UDRE0)));	
	UDR0 = c;
}

void uart_write(const char *str) {
	while (*str != '\0') {
		uart_putc(*str);
		str++;
	}
}

char _buf[100];

#define _PRINTF(fmtstr, ...)  { \
		sprintf(_buf, fmtstr, ##__VA_ARGS__); \
		uart_write(_buf);}

void init() {
	CLKPR=(1<<CLKPCE);
	CLKPR=(0<<CLKPCE) | (0<<CLKPS3) | (0<<CLKPS2) | (0<<CLKPS1) | (0<<CLKPS0);
// USART initialization2
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

void task_1_1();
void task_2_1();

int main(void)
{
	task_1_1();
}

void task_1_1() {
	init();
	
	uint16_t arr[] = {77, 34, 15, 30000};
	uint16_t pair_11_cnt = 0, pair_00_cnt = 0;
	
	for (uint8_t i = 0; i < sizeof(arr)/sizeof(*arr); i++) {
		uint16_t cur_11_cnt = 0, cur_00_cnt = 0;
		uint16_t tmp = arr[i];
		_PRINTF("Number %d \r\n", tmp)
		
		for (uint8_t j = 0; j < sizeof(*arr) * 8 - 1; j++) {
			if ((tmp & 0x3) == 0x0) cur_00_cnt += 1;
			if ((tmp & 0x3) == 0x3) cur_11_cnt += 1;
			tmp >>= 1;
		}
		_PRINTF("	pair 11: %4d    pair 00: %4d\r\n\r\n", cur_11_cnt, cur_00_cnt);
		pair_00_cnt += cur_00_cnt;
		pair_11_cnt += cur_11_cnt;
	}
	_PRINTF("TOTAL:\r\n	pair 11: %4d    pair 00: %4d\r\n", pair_11_cnt, pair_00_cnt);
	
	while (1)
	{
		
	}
}

void task_2_1() {
	init();
	DDRB = 0xF0;
	uint16_t keys = 0;
	
	while (1) {
		uint16_t cur_keys = 0;
		for (uint8_t row = 0; row < 4; row++) {
			PORTB = ~(1 << (row+4));
			cur_keys |= (PINB & 0xF) << (4*row);
			_delay_ms(100);
		}
		if (cur_keys != keys) {
			keys = cur_keys;
			
			for (uint8_t i = 0; i < 16; i++) {
				_PRINTF("%2d", cur_keys & 1);
				cur_keys >>= 1;
			}
			_PRINTF("\r\n")
		}
	}
	
}