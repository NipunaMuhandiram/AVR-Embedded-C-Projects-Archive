#include <avr/io.h>
#include <util/delay.h>

#define F_CPU 8000000UL  // 8 MHz clock frequency
#define PWM_FREQ 500     // Set a higher PWM frequency for smoother LED control
#define PRESCALER 64     // Prescaler value

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

void adjust_brightness() {
	uint16_t duty_cycle;
	uint16_t top_value = ICR1;  // Use ICR1 for Timer/Counter1, others have TOP = 255

	// Increase brightness from 0 to maximum
	for (duty_cycle = 0; duty_cycle <= top_value; duty_cycle++) {
		OCR1A = duty_cycle;  // Set duty cycle for PB1 (OC1A)
		OCR1B = duty_cycle;  // Set duty cycle for PB2 (OC1B)
		OCR0A = duty_cycle >> 2;  // Set duty cycle for PD6 (OC0A) (scaled for 8-bit)
		OCR0B = duty_cycle >> 2;  // Set duty cycle for PD5 (OC0B) (scaled for 8-bit)
		OCR2A = duty_cycle >> 2;  // Set duty cycle for PB3 (OC2A) (scaled for 8-bit)
		OCR2B = duty_cycle >> 2;  // Set duty cycle for PD3 (OC2B) (scaled for 8-bit)
		_delay_ms(40);  // Delay for smooth transition
	}

	// Decrease brightness from maximum to 0
	for (duty_cycle = top_value; duty_cycle > 0; duty_cycle--) {
		OCR1A = duty_cycle;  // Set duty cycle for PB1 (OC1A)
		OCR1B = duty_cycle;  // Set duty cycle for PB2 (OC1B)
		OCR0A = duty_cycle >> 2;  // Set duty cycle for PD6 (OC0A) (scaled for 8-bit)
		OCR0B = duty_cycle >> 2;  // Set duty cycle for PD5 (OC0B) (scaled for 8-bit)
		OCR2A = duty_cycle >> 2;  // Set duty cycle for PB3 (OC2A) (scaled for 8-bit)
		OCR2B = duty_cycle >> 2;  // Set duty cycle for PD3 (OC2B) (scaled for 8-bit)
		_delay_ms(40);  // Delay for smooth transition
	}
}

int main(void) {
	setup_pwm();  // Set up PWM

	while (1) {
		adjust_brightness();  // Adjust LED brightness
	}

	return 0;
}
