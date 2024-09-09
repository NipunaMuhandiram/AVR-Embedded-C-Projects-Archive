#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL

uint8_t ssd_digits_cc[8] = {
	0b00111111,  // 0
	0b00000110,  // 1
	0b01011011,  // 2
	0b01001111,  // 3
	0b01100110,  // 4
	0b01101101,  // 5
	0b01111101,  // 6
	0b00000111   // 7
};

volatile uint8_t counter = 0;

void display_digit(uint8_t digit) {
	if (digit > 7) return;
	PORTB = ssd_digits_cc[digit];
	PORTC = digit;  // Display binary value on LEDs connected to PC0, PC1, PC2
}

ISR(INT0_vect) {
	counter = (counter + 1) % 8;
	display_digit(counter);
}

ISR(INT1_vect) {
	counter = (counter == 0) ? 7 : counter - 1;
	display_digit(counter);
}

int main(void) {
	DDRB = 0x7F;  
	DDRC = 0x07;  
	DDRD &= ~((1 << PD2) | (1 << PD3));  
	PORTD |= (1 << PD2) | (1 << PD3);   

	EICRA = (1 << ISC01) | (1 << ISC11);  
	EIMSK = (1 << INT0) | (1 << INT1);   

	sei();  
	while (1) {
		
	}
	return 0;
}
