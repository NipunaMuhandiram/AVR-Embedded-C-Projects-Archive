#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 8000000UL
#define DEBOUNCE_DELAY 50  // Debounce delay in milliseconds

volatile uint8_t counter = 7;  // Start from maximum value for down-counter

// Function to display binary counter value on LEDs
void display_binary_counter(void) {
	PORTB = (counter << 3);  // Display the counter value on PB3, PB4, PB5
}

// Interrupt Service Routine for counter decrement (INT0)
ISR(INT0_vect) {
	_delay_ms(DEBOUNCE_DELAY);  // Debounce delay
	if (!(PIND & (1 << PD2))) {  // Check if button is still pressed
		counter = (counter == 0) ? 7 : counter - 1;  // Decrement counter, wrap to 7 if it reaches 0
		display_binary_counter();  // Update the display
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

	// Initial display of the counter value
	display_binary_counter();

	while (1) {
		// Main loop does nothing, just waits for interrupts
	}

	return 0;
}
