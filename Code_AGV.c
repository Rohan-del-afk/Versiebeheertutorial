/*
 */

#include <avr/io.h>
#include <util/delay.h>
#include <stdbool.h>

bool step_00 = false;
bool step_10 = false;
bool step_20 = false;
bool step_30 = false;
bool step_40 = false;
bool step_50 = false;
bool step_60 = false;
bool step_70 = false;
bool step_80 = false;
bool step_90 = false;

int Step = 00;

int main(void)
{
    DDRD |= (1<<PD3);// pin 3 AANKNOP

    DDRD |= (1<<PD5);// pin 5 MinSchakelaar

    DDRD |= (1<<PD6);// pin 6 MaxSchakelaar

    DDRD |= (1<<PD7);// pin 7 LED blauw

    DDRB |= (1<<PB0);// pin 8 aan knop LED

    DDRB |= (1<<PB1);// pin 9 noodstop

    DDRB |= (1<<PB2);//pin 10 LED groen

    DDRB |= (1<<PB4); //pin 12 motor

    DDRB |= (1<<PB5); //pin 13 motor


    DDRD &=  ~(0b00001010); // pin 2 en 4 knopje



    while(1)
    {
    int START = 0;
    int MOTOR = 0;
    int NOODSTOP = 0;
    int MotorMetKlok = 0;
    int MotorTegenKlok = 0;
    int MinSchakelaar = 0;
    int MaxSchakelaar = 0;

        if (( PIND & (1<<PD3))) // Machine gereed START
        {
            START = true;
        }
        if (( PIND & (1<<PD2))) // Motor met klok
        {
            MotorMetKlok = true;
        }
        if (( PIND & (1<<PD4))) // Motor tegen klok
        {
            MotorTegenKlok = true;
        }
        if (( PINB & (1<<PB1))) // Noodstop
        {
            NOODSTOP = true;
        }
        if (( PIND & (1<<PD5))) // MinSchakelaar
        {
            MinSchakelaar = true;
        }
        if (( PIND & (1<<PD6))) // MaxSchakelaar
        {
            MaxSchakelaar = true;
        }
        step_00  =   (START);
        step_10  =   (START)&& (MotorMetKlok) && !(MotorTegenKlok) && !(NOODSTOP); //motor draaien met klok
        step_20  =   (START)&& !(MotorMetKlok)&& (MotorTegenKlok) && !(NOODSTOP); //motor draaien tegen de klok
        step_30  =   (START)&& (NOODSTOP); //noodstop
        step_40  =   (START)&& (MotorMetKlok) && (MotorTegenKlok) && !(NOODSTOP); //Tegelijk
        step_50  =  !(START); // machine staat uit
        step_60  =   (START)&&(MinSchakelaar) ; //Minnimum niks
        step_70  =   (START)&&(MaxSchakelaar) ; //Maximum niks
        step_80  =   (START)&&(MinSchakelaar) && (MotorTegenKlok)&& !(NOODSTOP); //Minimum omhoog
        step_90  =   (START)&&(MaxSchakelaar) && (MotorMetKlok)&& !(NOODSTOP); //Maximum omlaag

        if (step_00)
        {
            Step = 00;
        }
        if (step_10)
        {
            Step = 10;
        }
        if (step_20)
        {
            Step = 20;
        }
        if (step_30)
        {
            Step = 30;
        }
        if (step_40)
        {
            Step = 40;
        }
        if (step_50)
        {
            Step = 50;
        }
        if (step_60)
        {
            Step = 60;
        }
        if (step_70)
        {
            Step = 70;
        }
        if (step_80)
        {
            Step = 80;
        }
        if (step_90)
        {
            Step = 90;
        }
        switch (Step)
        {
        case 00:
            Start(1);
            break;  //Zonder break stapt het programma niet uit de lus en gaat alle stappen aflopen
        case 10:
            Motor(1);
            break;
        case 20:
            Motor(2);
            break;
        case 30:
            Noodstop(1);
            break;
        case 40:
            Motor(3);
            break;
        case 50:
            Start(2);
            break;
        case 60:
            Motor(3);
            break;
        case 70:
            Motor(3);
            break;
        case 80:
            Motor(1);
            break;
        case 90:
            Motor(2);
            break;
        }

   }
    }
void Reset()
{
    step_00 = false;
    step_10 = false;
    step_20 = false;
    step_30 = false;
    step_40 = false;
    step_50 = false;
    step_60 = false;
    step_70 = false;
    step_80 = false;
    step_90 = false;


}
void Motor(int M)
{

    // Serial.println(M);
    if (M == 1) // OMHOOG
    {
            _delay_ms(50);
            PORTB |= (1<<PB5);//PIN hoog
            PORTB &= ~(1<<PB4);// PIN laag
            PORTD |= (1<<PD7);//pin 7 LED blauw aan
            PORTB |= (1<<PB0);// AAN LED
    }

    if ((M == 2))  // OMLAAG
    {
            _delay_ms(50);
            PORTB |= (1<<PB4);//PIN hoog
            PORTB &= ~(1<<PB5);// PIN laag
            PORTB |= (1<<PB0);// AAN LED
            PORTB |= (1<<PB2);//pin 10 LED groen aan
    }

    if ((M == 3))  //   TWEE KNOPPEN TEGELIJK = UIT
    {
        PORTB &= ~(1<<PB5);
        PORTB &= ~(1<<PB4);
        PORTB |= (1<<PB0);// AAN LED
        PORTB &= ~(1<<PB2);//pin 10 LED groen uit
        PORTD &= ~(1<<PD7);//pin 7 LED blauw uit
    }
}
void Noodstop (int N)
{
    if ((N == 1))
    {
        PORTB &= ~(1<<PB5); //motor uit
        PORTB &= ~(1<<PB4); //motor uit
        PORTD |= (1<<PD6);//pin 11 noodstop LED rood aan DEFECT
        PORTB |= (1<<PB0);// LED ROOD knipperen AAN
        _delay_ms(500);
        PORTB &= ~(1<<PB0); // LED ROOD knipperen UIT
        _delay_ms(500);
    }
}
void Start (int S)
{
    if ((S == 1))
    {
        _delay_ms(20);
        PORTB &= ~(1<<PB5); //motor uit
        PORTB &= ~(1<<PB4); //motor uit
        PORTB &= ~(1<<PB2); //lamp uit
        PORTB &= ~(1<<PB3); //lamp uit
        PORTD &= ~(1<<PD7); //lamp uit
        PORTB |= (1<<PB0);// AAN LED ROOD

    }
        if ((S == 2))
    {
        _delay_ms(20);
        PORTB &= ~(1<<PB5); //motor uit
        PORTB &= ~(1<<PB4); //motor uit
        PORTB &= ~(1<<PB2); //lamp uit
        PORTB &= ~(1<<PB3); //lamp uit
        PORTD &= ~(1<<PD7); //lamp uit
        PORTB &= ~(1<<PB0);// AAN LED ROOD
    }
}