#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#define F_CPU 8000000UL
#define DELAY_1_SECOND 1000  // Delay in milliseconds for one second

// Global variable for the counter
volatile uint16_t counter = 0;

void display_number(uint16_t num) {
	// Split number into individual digits
	uint8_t digit1 = num / 1000;          // Thousands place
	uint8_t digit2 = (num / 100) % 10;    // Hundreds place
	uint8_t digit3 = (num / 10) % 10;     // Tens place
	uint8_t digit4 = num % 10;            // Units place
	
	// Array of digits to display
	uint8_t digits[4] = {digit1, digit2, digit3, digit4};
	
	for (uint8_t i = 0; i < 4; i++) {
		// Set BCD value for the current digit
		PORTB = digits[i];
		
		// Activate the current SSD
		PORTC = (1 << i);
		
		// Short delay to ensure the display is updated
		_delay_ms(5);  // Adjust delay as needed

		// Turn off SSD after displaying
		PORTC &= ~(1 << i);
	}
}

// Interrupt Service Routine for INT0 (Increment)
ISR(INT0_vect) {
	counter++;
	if (counter > 9999) {
		counter = 0;
	}
}

// Interrupt Service Routine for INT1 (Decrement)
ISR(INT1_vect) {
	if (counter == 0) {
		counter = 9999;
		} else {
		counter--;
	}
}

int main(void) {
	// Set PB0-PB3 as output (BCD data lines)
	DDRB = 0x0F;  // Set PB0-PB3 as output
	PORTB = 0x00; // Initialize PORTB to 0
	
	// Set PC0-PC3 as output (SSD control lines)
	DDRC = 0x0F;  // Set PC0-PC3 as output
	PORTC = 0x00; // Initialize PORTC to 0
	
	// Set PD2 and PD3 as input (INT0 and INT1)
	DDRD &= ~((1 << PD2) | (1 << PD3));  // Set PD2 and PD3 as input
	PORTD |= (1 << PD2) | (1 << PD3);    // Enable pull-up resistors on PD2 and PD3
	
	// Configure interrupts
	EICRA = (1 << ISC01) | (1 << ISC11);  // INT0 and INT1 on falling edge
	EIMSK = (1 << INT0) | (1 << INT1);    // Enable INT0 and INT1
	
	sei();  // Enable global interrupts
	
	while (1) {
		display_number(counter);
		_delay_ms(10);  // Adjust delay to control the refresh rate
	}
	
	return 0;
}
