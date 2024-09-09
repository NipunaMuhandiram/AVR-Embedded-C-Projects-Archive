#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 8000000UL
#define DEBOUNCE_DELAY 50  // Debounce delay in milliseconds

volatile uint8_t counter = 0;

// Interrupt Service Routine for counter increment (INT0)
ISR(INT0_vect) {
	_delay_ms(DEBOUNCE_DELAY);  // Debounce delay
	if (!(PIND & (1 << PD2))) {  // Check if button is still pressed
		counter = (counter + 1) % 8;  // Increment counter and wrap around at 8
		PORTB = (counter << 3);  // Display the counter value on PB3, PB4, PB5
	}
}

int main(void) {
	// Set PB3-PB5 as output for displaying the counter value
	DDRB = (1 << PB3) | (1 << PB4) | (1 << PB5);  // Set PB3, PB4, PB5 as output

	// Set PD2 as input for the button with a pull-up resistor
	DDRD &= ~(1 << PD2);  // Set PD2 as input
	PORTD |= (1 << PD2);  // Enable pull-up resistor on PD2

	// Enable interrupt on INT0 (PD2)
	EICRA |= (1 << ISC01);  // Trigger INT0 on falling edge
	EIMSK |= (1 << INT0);   // Enable INT0 interrupt

	sei();  // Enable global interrupts

	while (1) {
		// Main loop does nothing, just waits for interrupts
	}

	return 0;
}
