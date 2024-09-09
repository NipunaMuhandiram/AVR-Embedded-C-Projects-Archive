#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 8000000UL
#define STEP_DELAY 500  // Delay between half-steps in milliseconds
#define DEBOUNCE_DELAY 50  // Debounce delay in milliseconds

volatile uint8_t direction = 1;  // 1 for clockwise, 0 for counterclockwise

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
		direction = !direction;  // Toggle direction
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
	
	while (1) {
		step_motor(step);
		
		// Increment or decrement step based on direction
		if (direction) {
			step = (step + 1) % 8;  // Clockwise direction
			} else {
			step = (step == 0) ? 7 : step - 1;  // Counterclockwise direction
		}
		
		// Wait before moving to the next half-step
		_delay_ms(STEP_DELAY);
	}
	
	return 0;
}
