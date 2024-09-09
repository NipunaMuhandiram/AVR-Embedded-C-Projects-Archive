#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL
#define LED_DELAY 500  // Delay for each LED in milliseconds
#define DEBOUNCE_DELAY 50  // Debounce delay in milliseconds

volatile uint8_t mode = 0;  // 0 for blinking LEDs, 1 for other mode

// Function to blink LEDs sequentially
void blink_leds(void) {
	while (mode == 0) {  // Loop until the mode is changed
		PORTB = 0b00000001;  // Turn on LED L1 (connected to PB0)
		_delay_ms(LED_DELAY);
		PORTB = 0b00000000;  // Turn off all LEDs
		_delay_ms(LED_DELAY);
		
		PORTB = 0b00000010;  // Turn on LED L2 (connected to PB1)
		_delay_ms(LED_DELAY);
		PORTB = 0b00000000;  // Turn off all LEDs
		_delay_ms(LED_DELAY);
		
		PORTB = 0b00000100;  // Turn on LED L3 (connected to PB2)
		_delay_ms(LED_DELAY);
		PORTB = 0b00000000;  // Turn off all LEDs
		_delay_ms(LED_DELAY);
	}
}

// Interrupt Service Routine for mode toggle (INT0)
ISR(INT0_vect) {
	_delay_ms(DEBOUNCE_DELAY);  // Debounce delay
	if (!(PIND & (1 << PD2))) {  // Check if button is still pressed
		mode ^= 1;  // Toggle mode between 0 and 1
	}
}

int main(void) {
	// Set PB0-PB2 as output for controlling the LEDs
	DDRB = 0x07;  // Set PB0-PB2 as output

	// Set PD2 as input for the button with a pull-up resistor
	DDRD &= ~(1 << PD2);  // Set PD2 as input
	PORTD |= (1 << PD2);  // Enable pull-up resistor on PD2
	
	// Enable interrupt on INT0 (PD2)
	EICRA |= (1 << ISC01);  // Trigger INT0 on falling edge
	EIMSK |= (1 << INT0);   // Enable INT0 interrupt
	
	sei();  // Enable global interrupts

	while (1) {
		if (mode == 0) {
			blink_leds();  // Blink LEDs sequentially
			} else {
			// Placeholder for other modes or functions
			// This can be replaced with other functionality as needed
		}
	}

	return 0;
}
