/*

*/

#include <MapleFreeRTOS821.h>

#define LED_PIN_BLUE PA1
#define TICKER_MAX 255

enum{UP, DOWN, TOP, BOTTOM};

int state;
int pwm;
int ticker;

void change_pwm(bool direction)
{
    if(direction == UP)
    {
        pwm++;
        pwm = pwm > 255 ? 255 : pwm;
    }

    if(direction == DOWN)
    {
        pwm--;
        pwm = pwm < 0 ? 0 : pwm;
    }
}

void setup()
{
    pinMode(LED_PIN_BLUE, OUTPUT);
    state = UP;
    pwm = 0;
    analogWrite(LED_PIN_BLUE, pwm);
}

void loop()
{
    switch(state)
    {
        case UP:
            change_pwm(UP);
            analogWrite(LED_PIN_BLUE, pwm);
            if(ticker == TICKER_MAX)
            {
                ticker = 0;
                state = TOP;
            }
            break;

        case TOP:
            if(ticker == TICKER_MAX)
            {
                ticker = 0;
                state = DOWN;
            }
            break;

        case DOWN:
            change_pwm(DOWN);
            analogWrite(LED_PIN_BLUE, pwm);
            if(ticker == TICKER_MAX)
            {
                ticker = 0;
                state = BOTTOM;
            }
            break;

        case BOTTOM:
            if(ticker == TICKER_MAX)
            {
                ticker = 0;
                state = UP;
            }
            break;

        default:
            break;
    }

    ticker++;
    delay(20);
}
