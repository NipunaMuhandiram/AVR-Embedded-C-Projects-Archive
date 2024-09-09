#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL
#define STEP_DELAY 4000  // Delay between half-steps in milliseconds

// Function to rotate the stepper motor by one half-step
void step_motor(uint8_t step) {
	// Sequence of half-steps
	switch(step) {
		case 0:
		PORTB = 0b00000001;  // Coil 1 ON
		break;
		case 1:
		PORTB = 0b00000011;  // Coil 1 and Coil 2 ON
		break;
		case 2:
		PORTB = 0b00000010;  // Coil 2 ON
		break;
		case 3:
		PORTB = 0b00000110;  // Coil 2 and Coil 3 ON
		break;
		case 4:
		PORTB = 0b00000100;  // Coil 3 ON
		break;
		case 5:
		PORTB = 0b00001100;  // Coil 3 and Coil 4 ON
		break;
		case 6:
		PORTB = 0b00001000;  // Coil 4 ON
		break;
		case 7:
		PORTB = 0b00001001;  // Coil 4 and Coil 1 ON
		break;
	}
}

int main(void) {
	// Set PB0-PB3 as output for controlling the stepper motor coils
	DDRB = 0x0F;  // Set PB0-PB3 as output
	
	uint8_t step = 0;
	
	while (1) {
		step_motor(step);
		
		// Increment step and wrap around after 8 half-steps (0, 1, 2, 3, 4, 5, 6, 7)
		step = (step + 1) % 8;
		
		// Wait before moving to the next half-step
		_delay_ms(STEP_DELAY);
	}
	
	return 0;
}
