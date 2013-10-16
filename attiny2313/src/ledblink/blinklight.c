/*
 blinklight.c
 
 Blinks a LED- basic example to show you can upload to your avr
 */

#include <avr/io.h>
#include <avr/interrupt.h>

#define LED PB3

void delay_ms(uint8_t ms) {
    uint16_t delay_count = 8000000 / 17500;
    volatile uint16_t i;
    
    while (ms != 0) {
        for (i=0; i != delay_count; i++);
        ms--;
    }
}

/* value from the ADC (0..1024) */
volatile int rate;

/* internal service routine */
/* analog digital converter vector */
ISR(ADC_vect)
{
    rate = ADCH * 4;
    /* start next conversion */
    ADCSRA |= 1 << ADSC;
}

int main(void) {
    /* Enable pins PB1 and PB3 for output via PORTB */
    DDRB = _BV(3) | _BV(1);
    
    /* configure the ADC */
    /* enable interrupt function in ADC */
    ADCSRA |= 1 << ADIE;
    /* 8-bit or 10-bit results */
    ADMUX |= 1 << ADLAR;   /* left-shifted register */
    /* Set the reference voltage */
    ADMUX |= 1 << REFS0;
    /* enable prescaler - determined by clock */
    /* prescaler between 5 and 20 */
    ADCSRA |= 1 << ADPS2;   /* prescaler of 16 */
    /* turn on the ADC */
    ADCSRA |= 1 << ADEN;

    /* enable global interrupts */
    sei();

    /* start first conversion */
    ADCSRA |= 1 << ADSC;

    for(;;) {
        /* Set only pin PB3 to HIGH */
        PORTB = _BV(3);
    }
    
    return 0;
}
