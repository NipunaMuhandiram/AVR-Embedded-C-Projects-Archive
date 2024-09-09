#include <avr/io.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL  

ISR(INT0_vect) {
	
	PORTB ^= (1 << PB0);
}

int main(void) {

	DDRB |= (1 << PB0);  
	DDRD &= ~(1 << PD2); 
	PORTD |= (1 << PD2); 

	
	EICRA |= (1 << ISC01); 
	EIMSK |= (1 << INT0);  
	sei();

	while (1) {
		
	}
	
	return 0;
}



