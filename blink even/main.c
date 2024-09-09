#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL  
#define BLINK_DELAY_MS 500  

int main(void) {
	
	DDRB |= (1 << PB0) | (1 << PB2) | (1 << PB4) | (1 << PB6);

	while (1) {
		
		PORTB |= (1 << PB0) | (1 << PB2) | (1 << PB4) | (1 << PB6);		
		_delay_ms(BLINK_DELAY_MS);
		PORTB &= ~((1 << PB0) | (1 << PB2) | (1 << PB4) | (1 << PB6));
		_delay_ms(BLINK_DELAY_MS);
	}
	
	return 0;
}
