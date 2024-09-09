#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

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
volatile uint8_t mode = 0;  // 0: Mode 1, 1: Mode 2

void display_digit(uint8_t digit) {
	if (digit > 7) return;
	if (mode == 0) {
		// Mode 1: Turn on only SSD
		PORTB = ssd_digits_cc[digit];
		PORTC &= ~0x07;  // Turn off LEDs
		} else {
		// Mode 2: Turn on LEDs
		PORTB &= ~0x7F;  // Turn off SSD
		PORTC = digit & 0x07;  // Display binary value on LEDs
	}
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
	DDRB = 0x7F;  // Set PB0-PB6 as output (SSD data lines)
	DDRC = 0x07;  // Set PC0-PC2 as output (LEDs)
	DDRD &= ~((1 << PD2) | (1 << PD3));  // Set PD2 and PD3 as input (interrupts)
	PORTD |= (1 << PD2) | (1 << PD3);  // Enable pull-up resistors on PD2 and PD3
	
	// Configure PD0 as input with pull-up resistor
	DDRD &= ~(1 << PD0);
	PORTD |= (1 << PD0);

	// Configure interrupts
	EICRA = (1 << ISC01) | (1 << ISC11);  // INT0 and INT1 on falling edge
	EIMSK = (1 << INT0) | (1 << INT1);    // Enable INT0 and INT1

	sei();  // Enable global interrupts

	while (1) {
		// Poll PD0 to toggle mode
		if (!(PIND & (1 << PD0))) {  // Check if button is pressed (active low)
			_delay_ms(50);  // Debounce delay
			if (!(PIND & (1 << PD0))) {
				mode ^= 1;  // Toggle mode between 0 and 1
				display_digit(counter);  // Update display according to new mode
				while (!(PIND & (1 << PD0))) {
					// Wait for button release
				}
				_delay_ms(50);  // Debounce delay
			}
		}
	}
	return 0;
}
