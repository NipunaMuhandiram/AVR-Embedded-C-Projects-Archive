#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL  // 8 MHz clock frequency
#define PWM_FREQ 500     // Set a higher PWM frequency for smoother LED control
#define PRESCALER 64     // Prescaler value

void setup_pwm() {
	// Set PB1 (OC1A) and PB2 (OC1B) as outputs
	DDRB |= (1 << PB1) | (1 << PB2);

	// Calculate TOP value for PWM frequency
	uint16_t top_value = (F_CPU / (PRESCALER * PWM_FREQ)) - 1;

	// Set Fast PWM mode with ICR1 as TOP (WGM mode 14)
	TCCR1A = (1 << WGM11);
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11) | (1 << CS10); // Prescaler 64, WGM mode 14

	// Set the TOP value in ICR1 register
	ICR1 = top_value;

	// Set non-inverting mode for OC1A and OC1B (Clear on Compare Match, set on BOTTOM)
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1);
}

void adjust_brightness() {
	uint16_t duty_cycle;
	uint16_t top_value = ICR1;

	// Increase brightness from 0 to maximum
	for (duty_cycle = 0; duty_cycle <= top_value; duty_cycle++) {
		OCR1A = duty_cycle;  // Set duty cycle for LED on PB1
		OCR1B = duty_cycle;  // Set duty cycle for LED on PB2
		_delay_ms(40);       // Delay for smooth transition
	}

	// Decrease brightness from maximum to 0
	for (duty_cycle = top_value; duty_cycle > 0; duty_cycle--) {
		OCR1A = duty_cycle;  // Set duty cycle for LED on PB1
		OCR1B = duty_cycle;  // Set duty cycle for LED on PB2
		_delay_ms(40);       // Delay for smooth transition
	}
}

int main(void) {
	setup_pwm();  // Set up PWM

	while (1) {
		adjust_brightness();  // Adjust LED brightness
	}

	return 0;
}
