#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 8000000UL 
#define MAX_COUNT 9       

volatile uint8_t counter = 0;  


ISR(INT0_vect) {
	if (counter < MAX_COUNT) {
		counter++; 
		} else {
		counter = 0;  
	}
	PORTB = counter;  
	_delay_ms(50);  
}


ISR(INT1_vect) {
	if (counter > 0) {
		counter--; 
		} else {
		counter = MAX_COUNT;  
	}
	PORTB = counter; 
	_delay_ms(50); 
}

int main(void) {
	
	DDRB = 0xFF;  

	DDRD &= ~((1 << PD2) | (1 << PD3));  
	PORTD |= (1 << PD2) | (1 << PD3);   

	
	EICRA |= (1 << ISC01) | (1 << ISC11);  
	EIMSK |= (1 << INT0) | (1 << INT1);    
	sei();  

	while (1) {
	
	}

	return 0;
}
