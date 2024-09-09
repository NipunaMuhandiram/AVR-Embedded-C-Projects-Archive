#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL

uint8_t ssd_digits_ca[10] = {
	0b11000000,  // 0
	0b11111001,  // 1
	0b10100100,  // 2
	0b10110000,  // 3
	0b10011001,  // 4
	0b10010010,  // 5
	0b10000010,  // 6
	0b11111000,  // 7
	0b10000000,  // 8
	0b10010000   // 9
};

volatile uint8_t counter = 0;

void display_digit(uint8_t digit) {
	if (digit > 9) return;
	PORTB = ssd_digits_ca[digit];
}

ISR(INT0_vect) {
	counter = (counter + 1) % 10;
	display_digit(counter);
}

ISR(INT1_vect) {
	counter = (counter == 0) ? 9 : counter - 1;
	display_digit(counter);
}

int main(void) {
	DDRB = 0x7F;
	DDRD &= ~((1 << PD2) | (1 << PD3));
	PORTD |= (1 << PD2) | (1 << PD3);

	EICRA = (1 << ISC01) | (1 << ISC11);  // Falling edge interrupt for INT0 and INT1
	EIMSK = (1 << INT0) | (1 << INT1);    // Enable INT0 and INT1

	sei();  // Enable global interrupts

	while (1) {
		// Main loop does nothing, interrupts handle the counter changes
	}
	return 0;
}
