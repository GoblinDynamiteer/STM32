
/*
2. F� koden att blinka 1 g�ng i sekunden:
�ndra v�rdena i vTaskDelay s� att LED:en blinkar en
g�ng i sekunden.

3. Extern LED, dim:
Sladda upp en extern LED i st�llet och f� denna att
dimma upp och ner l�ngsamt.

*/

#include <MapleFreeRTOS821.h>

#define LED_PIN_RED PA1
#define LED_PIN_GREEN PA2
#define LED_PIN_BLUE PA3
#define LED_ON_BOARD PC13

/* Fade red LED */
static void vLEDDim(void *pvParameters)
{
    while(1)
    {
        static int pwm = 100;
        static bool rise = true;

        pwm = rise ? (pwm + 1) : (pwm - 1);

        analogWrite(LED_PIN_RED, pwm);

        if(pwm < 5 || pwm > 150)
        {
            rise = !rise;
        }

        vTaskDelay(5);
    }
}

void setup()
{
    pinMode(LED_PIN_RED, OUTPUT);

    xTaskCreate(vLEDDim,
                "REDBLINK",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 2,
                NULL);

    vTaskStartScheduler();
}

void loop()
{

}
