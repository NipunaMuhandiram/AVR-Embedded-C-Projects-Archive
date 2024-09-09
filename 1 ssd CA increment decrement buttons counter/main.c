#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL
#define DEBOUNCE_DELAY 50  // Debounce time in milliseconds

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

void display_digit(uint8_t digit) {
	if (digit > 9) return;
	PORTB = ssd_digits_ca[digit];
}

uint8_t is_button_pressed(uint8_t pin) {
	return !(PIND & (1 << pin));
}

void debounce(uint8_t pin) {
	_delay_ms(DEBOUNCE_DELAY);
	while (is_button_pressed(pin)) {
		_delay_ms(DEBOUNCE_DELAY);
	}
}

int main(void) {
	uint8_t counter = 0;
	DDRB = 0x7F;
	DDRD &= ~((1 << PD0) | (1 << PD1));
	PORTD |= (1 << PD0) | (1 << PD1);
	
	while (1) {
		if (is_button_pressed(PD0)) {
			debounce(PD0);
			counter = (counter + 1) % 10;
		}
		if (is_button_pressed(PD1)) {
			debounce(PD1);
			counter = (counter == 0) ? 9 : counter - 1;
		}
		display_digit(counter);
	}
	return 0;
}
