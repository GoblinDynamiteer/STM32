//#include <wirish/wirish.h>
//#include "libraries/FreeRTOS/MapleFreeRTOS.h"
#include <MapleFreeRTOS821.h>

#define LED_PIN_RED PA1
#define LED_PIN_GREEN PA2
#define LED_PIN_BLUE PA3

/* Blink Red LED */
static void vLEDRedBlink(void *pvParameters)
{
    while(1)
    {
        vTaskDelay(1000);
        digitalWrite(LED_PIN_RED, HIGH);
        vTaskDelay(50);
        digitalWrite(LED_PIN_RED, LOW);
    }
}

/* Fade blue LED */
static void vLEDBlueFade(void *pvParameters)
{
    while(1)
    {
        static int pwm = 100;
        static bool rise = true;

        pwm = rise ? (pwm + 1) : (pwm - 1);

        analogWrite(LED_PIN_BLUE, pwm);

        if(pwm < 5 || pwm > 150)
        {
            rise = !rise;
        }

        vTaskDelay(5);
    }
}

/* Blink Green LED */
static void vLEDGreenBlink(void *pvParameters)
{
    while(1)
    {
        vTaskDelay(200);
        digitalWrite(LED_PIN_GREEN, HIGH);
        vTaskDelay(200);
        digitalWrite(LED_PIN_GREEN, LOW);
    }
}

void setup()
{
    pinMode(LED_PIN_RED, OUTPUT);
    pinMode(LED_PIN_BLUE, OUTPUT);
    pinMode(LED_PIN_GREEN, OUTPUT);

    xTaskCreate(vLEDRedBlink,
                "REDBLINK",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 2,
                NULL);

    xTaskCreate(vLEDGreenBlink,
                "GREENBLINK",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 2,
                NULL);

    xTaskCreate(vLEDBlueFade,
                "BLUEFADE",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 2,
                NULL);

    vTaskStartScheduler();
}

void loop()
{

}
