#include <avr/io.h>

#define F_CPU 8000000UL 

int main(void) {
	
	DDRB |= (1 << PB0); 
	DDRD &= ~(1 << PD0); 
	PORTD |= (1 << PD0); 

	while (1) {
		
		if (!(PIND & (1 << PD0))) {  
			PORTB |= (1 << PB0); 
			} else {
			PORTB &= ~(1 << PB0);
		}
	}
	
	return 0;
}
