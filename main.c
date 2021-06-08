/*
 */
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>


static volatile int pulse =0;// integer hele programma
static volatile int i=0;    // integer hele programma

void init(void){
DDRB |= (1<<PB3); //trig pin 11
DDRD &= ~(1<<PD2);// echo pin 2 INT0 port
DDRB |= (1<<PB1); // LED pin 9

PORTB &= ~(1<<PB3);
PORTD &= ~(1<<PD2);
PORTB &= ~(1<<PB1);

EICRA |= (1<<ISC00);
EIMSK |= (1<<INT0); // turns on INT0

}
int main(void)
{
    int16_t distance = 0;
    init();
    sei();
    while(1)
    {
        PORTB |= (1<<PB3);
        _delay_us(10);
        PORTB &= ~(1<<PB3);
        distance = pulse*0.034*0.5;
        if(distance <=6)
        {
            PORTB &=~(1<<PB1);
        }
        else{
            PORTB |= (1<<PB1);
        }

    }
}
ISR (INT0_vect)
{
if (i==1)//when logic from HIGH to LOW
    {
    TCCR1B=0;//disabling counter
    pulse=TCNT1;//count memory is updated to integer
    TCNT1=0;//resetting the counter memory
    i=0;

    }

if (i==0)//when logic change from LOW to HIGH
    {
    TCCR1B|=(1<<CS10);//enabling counter
    i=1;
    }
}
