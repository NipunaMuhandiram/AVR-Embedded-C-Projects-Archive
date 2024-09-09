#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL

// Example 4-digit number to display
uint16_t number = 1234;

// Define SSD digit positions
#define DIGIT1 0
#define DIGIT2 1
#define DIGIT3 2
#define DIGIT4 3

void display_number(uint16_t num) {
	// Split number into individual digits
	uint8_t digit1 = num / 1000;
	uint8_t digit2 = (num / 100) % 10;
	uint8_t digit3 = (num / 10) % 10;
	uint8_t digit4 = num % 10;
	
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
	DDRB = 0x0F;
	
	// Set PC0-PC3 as output (SSD control lines)
	DDRC = 0x0F;
	
	while (1) {
		display_number(number);
	}
	
	return 0;
}
