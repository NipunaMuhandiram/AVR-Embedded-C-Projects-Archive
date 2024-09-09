#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL
#define STEP_DELAY 500  // Delay between steps in milliseconds
#define DEBOUNCE_DELAY 50  // Debounce delay in milliseconds

volatile uint8_t direction = 0;  // 0 for clockwise, 1 for counterclockwise
volatile uint8_t half_stepping = 0;  // 0 for full-stepping, 1 for half-stepping

// Function to rotate the stepper motor by one step (full or half)
void step_motor(uint8_t step) {
	uint8_t step_sequence[] = {
		0b00000001,  // Step 1
		0b00000011,  // Step 2
		0b00000010,  // Step 3
		0b00000110,  // Step 4
		0b00000100,  // Step 5
		0b00001100,  // Step 6
		0b00001000,  // Step 7
		0b00001001   // Step 8
	};

	if (half_stepping) {
		PORTB = step_sequence[step];  // Use all steps for half-stepping
		} else {
		PORTB = step_sequence[step * 2];  // Use only every other step for full-stepping
	}
}

// Interrupt Service Routine for direction toggle (INT0)
ISR(INT0_vect) {
	_delay_ms(DEBOUNCE_DELAY);  // Debounce delay
	if (!(PIND & (1 << PD2))) {  // Check if button is still pressed
		direction ^= 1;  // Toggle direction
	}
}

// Interrupt Service Routine for stepping mode toggle (INT1)
ISR(INT1_vect) {
	_delay_ms(DEBOUNCE_DELAY);  // Debounce delay
	if (!(PIND & (1 << PD3))) {  // Check if button is still pressed
		half_stepping ^= 1;  // Toggle stepping mode
	}
}

int main(void) {
	// Set PB0-PB3 as output for controlling the stepper motor coils
	DDRB = 0x0F;  // Set PB0-PB3 as output

	// Set PD2 and PD3 as inputs for the buttons with pull-up resistors
	DDRD &= ~((1 << PD2) | (1 << PD3));  // Set PD2 and PD3 as input
	PORTD |= (1 << PD2) | (1 << PD3);    // Enable pull-up resistors on PD2 and PD3
	
	// Enable interrupts on INT0 (PD2) and INT1 (PD3)
	EICRA |= (1 << ISC01) | (1 << ISC11);  // Trigger INT0 and INT1 on falling edge
	EIMSK |= (1 << INT0) | (1 << INT1);    // Enable INT0 and INT1 interrupts
	
	sei();  // Enable global interrupts

	uint8_t step = 0;
	uint8_t step_count = 0;
	uint8_t max_steps = 4;  // Full-stepping mode uses 4 steps
	
	while (1) {
		max_steps = (half_stepping) ? 8 : 4;  // Determine max steps based on stepping mode

		// Rotate the motor in the selected direction
		step_motor(step);
		_delay_ms(STEP_DELAY);

		if (direction == 0) {
			step = (step + 1) % max_steps;  // Clockwise
			} else {
			step = (step == 0) ? max_steps - 1 : step - 1;  // Counterclockwise
		}
	}
	
	return 0;
}
