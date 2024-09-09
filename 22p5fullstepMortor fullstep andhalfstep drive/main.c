#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL
#define STEP_DELAY 500  // Delay between steps in milliseconds
#define FULL_STEP_DELAY 500  // Delay for full stepping mode
#define HALF_STEP_DELAY 250  // Delay for half stepping mode

// Function to rotate the stepper motor by one full step
void step_motor_full(uint8_t step) {
	// Sequence of full steps
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

// Function to rotate the stepper motor by one half-step
void step_motor_half(uint8_t step) {
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
		case 8:
		PORTB = 0b00001001;  // Coil 4 and Coil 1 ON (same as step 7 for half-stepping)
		break;
	}
}

int main(void) {
	// Set PB0-PB3 as output for controlling the stepper motor coils
	DDRB = 0x0F;  // Set PB0-PB3 as output

	uint8_t step = 0;
	uint8_t max_steps_full = 8;  // Number of full steps in a full rotation (360° / 22.5°)
	uint8_t max_steps_half = 16; // Number of half-steps in a full rotation (360° / 11.25°)

	// Full-Step Mode
	while (1) {
		for (step = 0; step < max_steps_full; step++) {
			step_motor_full(step);
			_delay_ms(FULL_STEP_DELAY);
		}
		_delay_ms(1000);  // Delay between full rotations

		// Half-Step Mode
		for (step = 0; step < max_steps_half; step++) {
			step_motor_half(step);
			_delay_ms(HALF_STEP_DELAY);
		}
		_delay_ms(1000);  // Delay between half-step rotations
	}
	
	return 0;
}
