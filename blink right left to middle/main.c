#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL  
#define BLINK_DELAY_MS 2000 

int main(void) {
	
	DDRB = 0xFF;  

	while (1) {
		
		for (uint8_t i = 0; i < 4; i++) {
			PORTB = (1 << i) | (1 << (7 - i)); 
			_delay_ms(BLINK_DELAY_MS);

			PORTB &= ~((1 << i) | (1 << (7 - i)));  
			_delay_ms(BLINK_DELAY_MS);
		}

		
		//PORTB = 0xFF;  
		//_delay_ms(BLINK_DELAY_MS);
		PORTB = 0x00;  
		_delay_ms(BLINK_DELAY_MS);
	}

	return 0;
}
