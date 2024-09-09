#include <avr/io.h>
#include <util/delay.h>

//#define F_CPU 16000000UL  
#define F_CPU 8000000UL
#define BLINK_DELAY_MS 500  

int main(void) {
	
	DDRB |= (1 << PB0);  
	
	while (1) {
		
		PORTB |= (1 << PB0); 
		_delay_ms(BLINK_DELAY_MS);
		
		
		PORTB &= ~(1 << PB0); 
		_delay_ms(BLINK_DELAY_MS);
	}
	
	return 0;
}
