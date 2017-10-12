/*

*/

#include <MapleFreeRTOS821.h>

#define LED_PIN_BLUE PA1
#define LED_PIN_RED PA2
#define TICKER_MAX 255

enum{UP, DOWN, TOP, BOTTOM};
enum{RED, BLUE};

int state[2];
int pwm[2];
int ticker[2];
int pin[2];

void change_pwm(int led, bool direction)
{
    if(direction == UP)
    {
        pwm[led]++;
        pwm[led] = pwm[led] > 255 ? 255 : pwm[led];
    }

    if(direction == DOWN)
    {
        pwm[led]--;
        pwm[led] = pwm[led] < 0 ? 0 : pwm[led];
    }
}

void dim_led(int led)
{
    switch(state[led])
    {
        case UP:
            change_pwm(led, UP);
            analogWrite(pin[led], pwm[led]);
            if(ticker[led] == TICKER_MAX)
            {
                ticker[led] = 0;
                state[led] = TOP;
            }
            break;

        case TOP:
            if(ticker[led] == TICKER_MAX)
            {
                ticker[led] = 0;
                state[led] = DOWN;
            }
            break;

        case DOWN:
            change_pwm(led, DOWN);
            analogWrite(pin[led], pwm[led]);
            if(ticker[led] == TICKER_MAX)
            {
                ticker[led] = 0;
                state[led] = BOTTOM;
            }
            break;

        case BOTTOM:
            if(ticker[led] == TICKER_MAX)
            {
                ticker[led] = 0;
                state[led] = UP;
            }
            break;

        default:
            break;
    }

    ticker[led]++;
}

void setup()
{
    state[RED] = UP;
    state[BLUE] = DOWN;
    pwm[RED] = 0;
    pwm[BLUE] = 255;
    pin[RED] = LED_PIN_RED;
    pin[BLUE] = LED_PIN_BLUE;

    pinMode(pin[RED], OUTPUT);
    pinMode(pin[BLUE], OUTPUT);

    analogWrite(pin[RED], pwm[RED]);
    analogWrite(pin[BLUE], pwm[BLUE]);
}

void loop()
{
    dim_led(RED);
    dim_led(BLUE);
    delay(5);
}
