#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL  
#define DEBOUNCE_DELAY_MS 50  
#define LED_COUNT 8

int main(void) {
	
	DDRB = 0xFF;  
	
	
	DDRD &= ~(1 << PD0); 
	PORTD |= (1 << PD0);  

	uint8_t counter = 0;  

	while (1) {
		
		if (!(PIND & (1 << PD0))) {  
			_delay_ms(DEBOUNCE_DELAY_MS);  
			
			if (!(PIND & (1 << PD0))) { 
				PORTB = (1 << counter);
				counter = (counter + 1) % LED_COUNT;
				  
				
				
				while (!(PIND & (1 << PD0))); 
				_delay_ms(DEBOUNCE_DELAY_MS); 
			}
		}
	}

	return 0;
}
