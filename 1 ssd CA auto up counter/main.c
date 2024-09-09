#include <avr/io.h>

#define F_CPU 8000000UL
#include <util/delay.h>

uint8_t ssd_digits[10] = {
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
	PORTB = ssd_digits[digit];
}

int main(void) {
	DDRB = 0x7F;
	while (1) {
		for (uint8_t i = 0; i < 10; i++) {
			display_digit(i);
			_delay_ms(1000);
		}
	}
	return 0;
}
