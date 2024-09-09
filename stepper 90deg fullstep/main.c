#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL
#define STEP_DELAY 4000  // Delay between steps in milliseconds

// Function to rotate the stepper motor by one step
void step_motor(uint8_t step) {
	// Sequence of full steps
	switch(step) {
		case 0:
		PORTB = 0b00000001;  // Coil 1 ON
		break;
		case 1:
		PORTB = 0b00000010;  // Coil 2 ON
		break;
		case 2:
		PORTB = 0b00000100;  // Coil 3 ON
		break;
		case 3:
		PORTB = 0b00001000;  // Coil 4 ON
		break;
	}
}

int main(void) {
	// Set PB0-PB3 as output for controlling the stepper motor coils
	DDRB = 0x0F;  // Set PB0-PB3 as output
	
	uint8_t step = 0;
	
	while (1) {
		step_motor(step);
		
		// Increment step and wrap around after 4 steps (0, 1, 2, 3)
		step = (step + 1) % 4;
		
		// Wait before moving to the next step
		_delay_ms(STEP_DELAY);
	}
	
	return 0;
}
