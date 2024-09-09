#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL
#define STEP_DELAY 500  // Delay between steps in milliseconds
#define DEBOUNCE_DELAY 50  // Debounce delay in milliseconds

volatile uint8_t direction = 0;  // 0 for clockwise, 1 for counterclockwise
volatile uint8_t half_stepping = 0;  // 0 for full-stepping, 1 for half-stepping

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
	}
}

// Interrupt Service Routine for direction toggle (INT0)
ISR(INT0_vect) {
	_delay_ms(DEBOUNCE_DELAY);  // Debounce delay
	if (!(PIND & (1 << PD2))) {  // Check if button is still pressed
		direction ^= 1;  // Toggle direction
	}
}

int main(void) {
	// Set PB0-PB3 as output for controlling the stepper motor coils
	DDRB = 0x0F;  // Set PB0-PB3 as output

	// Set PD2 as input for the button with a pull-up resistor
	DDRD &= ~(1 << PD2);  // Set PD2 as input
	PORTD |= (1 << PD2);  // Enable pull-up resistor on PD2
	
	// Enable interrupt on INT0 (PD2)
	EICRA |= (1 << ISC01);  // Trigger INT0 on falling edge
	EIMSK |= (1 << INT0);   // Enable INT0 interrupt
	
	sei();  // Enable global interrupts

	uint8_t step = 0;
	uint8_t max_steps = 8;  // Number of steps in a full rotation
	uint8_t max_steps_half = 16;  // Number of half-steps in a full rotation

	while (1) {
		// Determine maximum steps based on stepping mode
		max_steps = (half_stepping) ? max_steps_half : 8;

		// Rotate the motor in the selected direction
		if (half_stepping) {
			step_motor_half(step);
			} else {
			step_motor_full(step);
		}
		
		_delay_ms(STEP_DELAY);

		// Update step for next step
		if (direction == 0) {
			step = (step + 1) % max_steps;  // Clockwise
			} else {
			step = (step == 0) ? max_steps - 1 : step - 1;  // Counterclockwise
		}
	}

	return 0;
}
