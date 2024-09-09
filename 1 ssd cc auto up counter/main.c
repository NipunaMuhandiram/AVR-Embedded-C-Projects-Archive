#include <avr/io.h>

#define F_CPU 8000000UL  
#include <util/delay.h>

uint8_t ssd_digits[10] = {
	0b00111111,  // 0: A B C D E F 
	0b00000110,  // 1: B C
	0b01011011,  // 2: A B G E D
	0b01001111,  // 3: A B G C D
	0b01100110,  // 4: F G B C
	0b01101101,  // 5: A F G C D
	0b01111101,  // 6: A F G E D C
	0b00000111,  // 7: A B C
	0b01111111,  // 8: A B C D E F G
	0b01101111   // 9: A B C D F G
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
