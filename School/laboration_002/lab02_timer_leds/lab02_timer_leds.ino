/*
SAMMA MÅL SOM IGÅR MEN MED INTERRUPTS OCH TIMERS
Idag börjar vi på nytt och gör vi en tom sketch i stället.

STEG 1. Haka en ISR på en knapp!

    1. Icke-FreeRTOS-dimmer-LED:
    Sätt upp en LED att dimma upp och ner på samma sätt som igår,
    lägg dock koden Klipp och klistra är tillåtet så länge som
    FreeRTOS _inte_ används.

*/

#include <MapleFreeRTOS821.h>

#define LED_PIN_RED PA0
#define DIM_DELAY_RED 5
#define PWM_MAX 255
#define PWM_MIN 0
#define TICKER_MAX 255;

enum{UP, DOWN};

struct _led{
    int pin;
    int delay;
    bool direction;
    int pwm;
}led = {
    LED_PIN_RED,
    DIM_DELAY_RED,
    UP,
    0,
};

void set_pwm(bool direction)
{
    if(direction == UP && led.pwm <= PWM_MAX)
    {
        led.pwm++; return;
    }

    if(direction == DOWN && led.pwm >= PWM_MIN)
    {
        led.pwm--;
    }
}

void setup()
{
    pinMode(led.pin, OUTPUT);
}

/* Dim LED up/down with main loop instead of FreeRTOS */
void loop()
{
    while(1)
    {
        set_pwm(led.direction);
        analogWrite(led.pin, led.pwm);

        if( (led.pwm == PWM_MAX && led.direction == UP) ||
            (led.pwm == PWM_MIN && led.direction == DOWN))
             {
                  led.direction = !led.direction;
             }

        delay(led.delay);
    }
}
