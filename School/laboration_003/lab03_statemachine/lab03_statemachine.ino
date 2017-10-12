/*

*/

#include <MapleFreeRTOS821.h>

#define LED_PIN_BLUE PA1
#define LED_PIN_RED PA2
#define TICKER_MAX 255

enum{UP, DOWN, TOP, BOTTOM};
enum{RED, BLUE};

int state_led1, state_led2;
int pwm_led1, pwm_led2;
int ticker1, ticker2;

void change_pwm(int * pwm, bool direction)
{
    if(direction == UP)
    {
        (*pwm)++;
        *pwm = *pwm > 255 ? 255 : *pwm;
    }

    if(direction == DOWN)
    {
        (*pwm)--;
        *pwm = *pwm < 0 ? 0 : *pwm;
    }
}

void dim_led(int led)
{
    int * state = led == RED ? &state_led1 : &state_led2;
    int * pwm = led == RED ? &pwm_led1 : &pwm_led2;
    int * ticker = led == RED ? &ticker1 : &ticker2;
    int pin = led == RED ? LED_PIN_RED : LED_PIN_BLUE;

    switch(*state)
    {
        case UP:
            change_pwm(pwm, UP);
            analogWrite(pin, *pwm);
            if(*ticker == TICKER_MAX)
            {
                *ticker = 0;
                *state = TOP;
            }
            break;

        case TOP:
            if(*ticker == TICKER_MAX)
            {
                *ticker = 0;
                *state = DOWN;
            }
            break;

        case DOWN:
            change_pwm(pwm, DOWN);
            analogWrite(pin, *pwm);
            if(*ticker == TICKER_MAX)
            {
                *ticker = 0;
                *state = BOTTOM;
            }
            break;

        case BOTTOM:
            if(*ticker == TICKER_MAX)
            {
                *ticker = 0;
                *state = UP;
            }
            break;

        default:
            break;
    }

    *ticker = *ticker + 1;
}

void setup()
{
    pinMode(LED_PIN_BLUE, OUTPUT);
    pinMode(LED_PIN_RED, OUTPUT);
    state_led1 = UP;
    state_led2 = DOWN;
    pwm_led1 = 0;
    pwm_led2 = 255;
    analogWrite(LED_PIN_BLUE, pwm_led1);
    analogWrite(LED_PIN_RED, pwm_led2);
}

void loop()
{
    dim_led(RED);
    dim_led(BLUE);
    delay(20);
}
