/*
 * pop1.c
 *
 * Created: 17.09.2026 10:05:08
 * Author : student
 */ 

#include <avr/io.h>
#define F_CPU 11059200UL
#include <util/delay.h>
#define CLEAR_SEG 10
#define D 49
#define ZEROALG -900

const uint8_t symbols[12] = {0x3F, 0x06, 0x5B, 0x4F, 0x66, 0x6D, 0x7D, 0x07, 0x7F, 0x6F, 0x00, 0x40};

void sevensegment();
void shownumber();

uint8_t digit(uint16_t d, uint8_t m) {
	uint8_t i = 5, a;
	while(i) {				// цикл по разр€дам числа
		a = d % 10;			// выдел€ем очередной разр€д
		if(i-- == m) break; // выделен ли желаемый разр€д
		d /= 10;			// уменьшаем число в 10 раз
	}
	return(a);
}


void sevensegment(void){
	// adjustment all box for input
	DDRA |= (1 << 1 ) | (1 << 2)| (1 << 3) | (1 << 4) | (1 << 5);
	
	// adjustment all segments for input
	DDRC |= 0xFF;
	
}

void shownumber(int32_t value){
	//check zero
	if (value == 0) {
		PORTA = (1 << 5);
		PORTC = 0x3F;
		PORTA &= ~(1 << 5);
	}
	
	else {
		// check negative values
		uint8_t negative = 0;
		if (value < 0) {
			negative = 1;
			value *= (-1);
			} else {
			negative = 0;
		}
		
		// understand how digits in value
		uint8_t j = 0;
		while(digit(value, ++j) == 0);
		
		// clear the empty boxes
		for(uint8_t i = 1; i < j; i++) {
			PORTA |= (1 << i);
			_delay_us(2);
			PORTC = symbols[CLEAR_SEG];
			PORTA &= ~(1 << i);
		}
		
		// show digits after insignificant zeros
		for(uint8_t i = j; i <= 5; i++) {
			PORTA |= (1 << i);
			_delay_us(2);
			PORTC = symbols[digit(value, i)];
			PORTA &= ~(1 << i);
		}
		
		//negative values
		if (negative == 1) {
			PORTA |= (1 << 1);
			PORTC = symbols[11];
			PORTA &= ~(1 << 1);
			} else {
			PORTA |= (1 << 1);
			PORTC = 0;
			PORTA &= ~(1 << 1);
		}
	}
}


	
int main(void)
{
    sevensegment();
	
    while (1) 
    {
		int32_t fn = ZEROALG;
		//shownumber(fn);
		//_delay_ms(1000);
		for(uint8_t i = 0; i < 40; i++) {
			shownumber(fn);
			_delay_ms(1000);
			
			fn += D;
		}
    }
}