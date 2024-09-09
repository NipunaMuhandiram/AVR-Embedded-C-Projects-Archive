#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL  // 8 MHz clock frequency
#define PWM_FREQ 500     // Set a higher PWM frequency for smoother LED control
#define PRESCALER 64     // Prescaler value
#define DELAY_TIME 40    // Delay between brightness steps

void setup_pwm() {
	// Set PB1 (OC1A), PB2 (OC1B), PD5 (OC0B), PD6 (OC0A), PB3 (OC2A), and PD3 (OC2B) as outputs
	DDRB |= (1 << PB1) | (1 << PB2) | (1 << PB3);
	DDRD |= (1 << PD3) | (1 << PD5) | (1 << PD6);

	// Timer/Counter1 Setup for PB1 (OC1A) and PB2 (OC1B)
	TCCR1A = (1 << WGM11);  // Fast PWM mode with ICR1 as TOP
	TCCR1B = (1 << WGM12) | (1 << WGM13) | (1 << CS11) | (1 << CS10);  // Prescaler 64
	ICR1 = (F_CPU / (PRESCALER * PWM_FREQ)) - 1;  // Set TOP value for 500Hz PWM
	TCCR1A |= (1 << COM1A1) | (1 << COM1B1);  // Non-inverting mode for OC1A and OC1B

	// Timer/Counter0 Setup for PD5 (OC0B) and PD6 (OC0A)
	TCCR0A = (1 << WGM00) | (1 << WGM01);  // Fast PWM mode with TOP = 0xFF
	TCCR0B = (1 << CS01) | (1 << CS00);  // Prescaler 64
	TCCR0A |= (1 << COM0A1) | (1 << COM0B1);  // Non-inverting mode for OC0A and OC0B

	// Timer/Counter2 Setup for PB3 (OC2A) and PD3 (OC2B)
	TCCR2A = (1 << WGM20) | (1 << WGM21);  // Fast PWM mode with TOP = 0xFF
	TCCR2B = (1 << CS22);  // Prescaler 64
	TCCR2A |= (1 << COM2A1) | (1 << COM2B1);  // Non-inverting mode for OC2A and OC2B
}

void set_brightness(uint16_t duty_cycle, uint8_t led) {
	switch (led) {
		case 0:
		OCR1A = duty_cycle;  // PB1 (OC1A)
		break;
		case 1:
		OCR1B = duty_cycle;  // PB2 (OC1B)
		break;
		case 2:
		OCR0B = duty_cycle >> 2;  // PD5 (OC0B)
		break;
		case 3:
		OCR0A = duty_cycle >> 2;  // PD6 (OC0A)
		break;
		case 4:
		OCR2A = duty_cycle >> 2;  // PB3 (OC2A)
		break;
		case 5:
		OCR2B = duty_cycle >> 2;  // PD3 (OC2B)
		break;
	}
}

void knight_rider_pattern() {
	uint16_t duty_cycle;
	uint16_t top_value = ICR1;
	uint8_t i;

	// Move the light from PB1 to PD3
	for (i = 0; i < 6; i++) {
		// Fade in
		for (duty_cycle = 0; duty_cycle <= top_value; duty_cycle++) {
			set_brightness(duty_cycle, i);
			_delay_ms(DELAY_TIME);
		}
		// Fade out
		for (duty_cycle = top_value; duty_cycle > 0; duty_cycle--) {
			set_brightness(duty_cycle, i);
			_delay_ms(DELAY_TIME);
		}
	}

	// Move the light back from PD3 to PB1
	for (i = 4; i < 255; i--) {  // uint8_t wraps around to 255
		// Fade in
		for (duty_cycle = 0; duty_cycle <= top_value; duty_cycle++) {
			set_brightness(duty_cycle, i);
			_delay_ms(DELAY_TIME);
		}
		// Fade out
		for (duty_cycle = top_value; duty_cycle > 0; duty_cycle--) {
			set_brightness(duty_cycle, i);
			_delay_ms(DELAY_TIME);
		}
	}
}

int main(void) {
	setup_pwm();  // Set up PWM

	while (1) {
		knight_rider_pattern();  // Run the Knight Rider pattern
	}

	return 0;
}
