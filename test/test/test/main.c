/*
 * main.c
 *
 * Created: 9/4/2021 10:44:00 PM
 *  Author: danii
 */ 

#include <xc.h>
#include <avr/iom328p.h>


int main(void)
{
   
   
   UCSRA = 0x00;
   UCSRB = 0x08;
   UCSRC = 0x86;
   UBRRL = 0x33;
   UBRRH = 0x00;
   
   
}