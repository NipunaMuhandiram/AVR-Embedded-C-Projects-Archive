#include <avr/io.h>
#define F_CPU 8000000UL
int main(void) {
	
	DDRB |= (1 << PB0);  
	PORTB |= (1 << PB0);
	
	while (1) {
		
	}
	
	return 0;
}
