#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL 
int main(void) {
	
	DDRB |= (1 << PB0);  	
	DDRD &= ~(1 << PD0); 
	PORTD |= (1 << PD0); 

	uint8_t led_state = 0;  
	uint8_t button_state = 0; 
	
	while (1) {
		
		if (!(PIND & (1 << PD0))) {
			if (button_state == 0) {  
				led_state = !led_state;  
				button_state = 1;  
				
				if (led_state) {
					PORTB |= (1 << PB0);  
					} else {
					PORTB &= ~(1 << PB0); 
				}
			}
			} else {
			button_state = 0; 
		}

		_delay_ms(50);  
	}
	
	return 0;
}
