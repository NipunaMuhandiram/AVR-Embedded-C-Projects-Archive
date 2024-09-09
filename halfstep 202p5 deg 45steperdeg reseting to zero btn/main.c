#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL
#define STEP_DELAY 5000  // Delay between half-steps in milliseconds
#define DEBOUNCE_DELAY 50  // Debounce delay in milliseconds
#define STEPS_TO_ROTATE 9  // 202.5° requires 9 half-steps

volatile uint8_t rotate_motor = 0;  // Flag to start motor rotation

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

// Interrupt Service Routine for the button press (INT0)
ISR(INT0_vect) {
	_delay_ms(DEBOUNCE_DELAY);  // Debounce delay
	if (!(PIND & (1 << PD2))) {  // Check if button is still pressed
		rotate_motor = 1;  // Set flag to start motor rotation
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
	uint8_t step_count = 0;

	while (1) {
		if (rotate_motor) {
			// Rotate forward by 202.5° (9 half-steps)
			for (step_count = 0; step_count < STEPS_TO_ROTATE; step_count++) {
				step_motor(step);
				step = (step + 1) % 8;  // Move to the next half-step
				_delay_ms(STEP_DELAY);
			}

			// Rotate backward to return to 0° (9 half-steps)
			for (step_count = 0; step_count < STEPS_TO_ROTATE; step_count++) {
				step = (step == 0) ? 7 : step - 1;  // Move to the previous half-step
				step_motor(step);
				_delay_ms(STEP_DELAY);
			}
			
			rotate_motor = 0;  // Reset the flag after completing the rotation
		}
	}
	
	return 0;
}
