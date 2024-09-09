#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL

uint8_t ssd_digits_ca[8] = {
	0b11000000,  // 0
	0b11111001,  // 1
	0b10100100,  // 2
	0b10110000,  // 3
	0b10011001,  // 4
	0b10010010,  // 5
	0b10000010,  // 6
	0b11111000   // 7
};

volatile uint8_t counter = 0;

void display_digit(uint8_t digit) {
	if (digit > 7) return;
	PORTB = ssd_digits_ca[digit];
	PORTC = ~digit;  // Display binary value on LEDs connected to PC0, PC1, PC2 (inverted for common-anode)
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
