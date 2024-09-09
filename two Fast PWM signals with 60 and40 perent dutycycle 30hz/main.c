#include <avr/io.h>

#define F_CPU 8000000UL  // 8 MHz clock frequency
#define PWM_FREQ 30      // Desired PWM frequency in Hz
#define PRESCALER 256    // Prescaler value

int main(void) {
	// Set PB1 (OC1A) and PB2 (OC1B) as outputs
	DDRB |= (1 << PB1) | (1 << PB2);

	// Calculate TOP value for 30 Hz PWM frequency
	uint16_t top_value = (F_CPU / (PRESCALER * PWM_FREQ)) - 1;

	// Set Fast PWM mode with ICR1 as TOP (WGM mode 14)
	TCCR1A = (1 << WGM11);
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS12); // Prescaler 256, WGM mode 14

	// Set the TOP value in ICR1 register
	ICR1 = top_value;

	// Set the duty cycle for OC1A (60%)
	OCR1A = (0.6 * (top_value + 1)) - 1;

	// Set the duty cycle for OC1B (40%)
	OCR1B = (0.4 * (top_value + 1)) - 1;

	// Set non-inverting mode for OC1A and OC1B (Clear on Compare Match, set on BOTTOM)
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1);

	while (1) {
		// Main loop does nothing, PWM signals are generated automatically by hardware
	}

	return 0;
}
