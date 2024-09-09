#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL

// Delay for a second (assuming 8 MHz clock and 1-second delay needed)
#define DELAY_1_SECOND 1000  // Delay in milliseconds for one second

// Display the 4-digit counter on 4 SSDs
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

int main(void) {
	// Set PB0-PB3 as output (BCD data lines)
	DDRB = 0x0F;  // Set PB0-PB3 as output
	PORTB = 0x00; // Initialize PORTB to 0
	
	// Set PC0-PC3 as output (SSD control lines)
	DDRC = 0x0F;  // Set PC0-PC3 as output
	PORTC = 0x00; // Initialize PORTC to 0

	uint16_t counter = 0;

	while (1) {
		display_number(counter);

		// Increment counter and handle overflow
		counter++;
		if (counter > 9999) {
			counter = 0;
		}

		// Delay for 1 second
		for (uint16_t i = 0; i < DELAY_1_SECOND; i++) {
			_delay_ms(1);
		}
	}

	return 0;
}
