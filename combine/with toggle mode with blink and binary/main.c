#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 8000000UL
#define LED_DELAY 500  // Delay for each LED in milliseconds
#define DEBOUNCE_DELAY 50  // Debounce delay in milliseconds

volatile uint8_t mode = 0;  // 0 for blinking, 1 for binary display
volatile uint8_t counter = 0;

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

// Function to display binary counter value on LEDs
void display_binary_counter(void) {
	PORTB = (counter << 3);  // Display the counter value on PB3, PB4, PB5
}

// Interrupt Service Routine for mode toggle (INT1)
ISR(INT1_vect) {
	_delay_ms(DEBOUNCE_DELAY);  // Debounce delay
	if (!(PIND & (1 << PD3))) {  // Check if button is still pressed
		mode ^= 1;  // Toggle mode between 0 and 1
	}
}

// Interrupt Service Routine for counter increment (INT0)
ISR(INT0_vect) {
	_delay_ms(DEBOUNCE_DELAY);  // Debounce delay
	if (!(PIND & (1 << PD2))) {  // Check if button is still pressed
		counter = (counter + 1) % 8;  // Increment counter and wrap around at 8
		if (mode == 1) {
			display_binary_counter();  // Update display if in binary mode
		}
	}
}

int main(void) {
	// Set PB0-PB5 as output for controlling LEDs
	DDRB = 0x3F;  // Set PB0-PB5 as output

	// Set PD2 and PD3 as input for buttons with pull-up resistors
	DDRD &= ~((1 << PD2) | (1 << PD3));  // Set PD2 and PD3 as input
	PORTD |= (1 << PD2) | (1 << PD3);    // Enable pull-up resistors on PD2 and PD3

	// Enable interrupt on INT0 (PD2) and INT1 (PD3)
	EICRA |= (1 << ISC01) | (1 << ISC11);  // Trigger INT0 and INT1 on falling edge
	EIMSK |= (1 << INT0) | (1 << INT1);    // Enable INT0 and INT1 interrupts

	sei();  // Enable global interrupts

	while (1) {
		if (mode == 0) {
			blink_leds();  // Blink LEDs sequentially
			} else {
			// In binary mode, the LEDs update via the INT0 ISR
		}
	}

	return 0;
}
