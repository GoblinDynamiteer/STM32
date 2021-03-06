/*
SAMMA M�L SOM IG�R MEN MED INTERRUPTS OCH TIMERS
Idag b�rjar vi p� nytt och g�r vi en tom sketch i st�llet.

STEG 1. Haka en ISR p� en knapp!

    1. Icke-FreeRTOS-dimmer-LED:
    S�tt upp en LED att dimma upp och ner p� samma s�tt som ig�r,
    l�gg dock koden Klipp och klistra �r till�tet s� l�nge som
    FreeRTOS _inte_ anv�nds.


    2. Tryckknapp som triggar avbrott:
    anv�nd denna f�r att st�nga av och s�tta p� dimmer-funktionen,
    t.ex. s� att n�r knappen trycks, s� sl�cks LED:en och f�rblir avst�ngd,
    n�r den trycks en g�ng till s� b�rjar den dimma upp och ner igen.

    4. Dimma med ISR:en hakad p� timern.
    Implementera en LED-dimmer med hj�lp av ISR:en.

    5. Unders�kning - kan man s�tta fler timers:
    Kan man? SVAR: JA
*/

#include <MapleFreeRTOS821.h>

#define LED_PIN_RED PA0
#define LED_PIN_BLUE PA1 // For testing two timers
#define INT_PIN PA7 // Button connected with pulldown
#define DIM_DELAY_RED 5
#define PWM_MAX 255
#define PWM_MIN 0
#define TICKER_MAX 255
#define BUTTON_DELAY 200

enum{UP, DOWN};
enum{NO, YES};

unsigned long button_timer;
bool button_pressed;
bool swap;

struct _led{
    int pin;
    int delay;
    bool direction;
    int pwm;
    bool run;
}led = {
    LED_PIN_RED,
    DIM_DELAY_RED,
    UP,
    0,
    YES
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

/* Toggle dimming of LED when button is pressed */
void button_isr(void)
{
    if(!button_pressed && millis() - button_timer > BUTTON_DELAY)
    {
        button_pressed = YES;
        led.run = !led.run;
    }
}

/* Timer3 controls LED */
void dim_led(void)
{
    if(led.run)
    {
        set_pwm(led.direction);
        analogWrite(led.pin, led.pwm);

        /* Swap fade direction if max/min pwm reached */
        if( (led.pwm == PWM_MAX && led.direction == UP) ||
            (led.pwm == PWM_MIN && led.direction == DOWN))
            {
                led.direction = !led.direction;
            }
    }
}

/* Timer4 blinks LED */
void blink_led(void)
{
    swap = !swap;
    digitalWrite(LED_PIN_BLUE, swap ? 0 : 255);
}

void setup()
{
    pinMode(led.pin, OUTPUT);
    pinMode(LED_PIN_BLUE, OUTPUT);

    /* Interrupt pin / button pin */
    pinMode(INT_PIN, INPUT_PULLUP);
    attachInterrupt(INT_PIN, button_isr, FALLING);

    /* Timer 3, channel 1 == pin 12 */
    Timer3.setChannel1Mode(TIMER_OUTPUTCOMPARE);
    Timer3.setPeriod(5000);
    Timer3.setCompare1(1);
    Timer3.attachCompare1Interrupt(dim_led);

    Timer4.setChannel1Mode(TIMER_OUTPUTCOMPARE);
    Timer4.setPeriod(500000); // 0,5 seconds
    Timer4.setCompare1(1);
    Timer4.attachCompare1Interrupt(blink_led);

    button_timer = millis();
    button_pressed = NO;

    swap = YES;
}

/* Dim LED up/down with main loop instead of FreeRTOS */
void loop()
{
    if(button_pressed && millis() - button_timer > BUTTON_DELAY)
    {
        button_pressed = NO;
        button_timer = millis();
    }
}
