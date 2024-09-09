#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL 
#define BLINK_DELAY_MS 1000 

int main(void) {
	
	DDRB = 0xFF; 

	while (1) {
		
		for (uint8_t i = 0; i < 8; i++) {
			PORTB |= (1 << i);  
			_delay_ms(BLINK_DELAY_MS);
		}

		
		for (int8_t i = 7; i >= 0; i--) {
			PORTB &= ~(1 << i);  
			_delay_ms(BLINK_DELAY_MS);
		}
	}

	return 0;
}
