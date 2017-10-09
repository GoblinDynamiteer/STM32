//#include <wirish/wirish.h>
//#include "libraries/FreeRTOS/MapleFreeRTOS.h"
#include <MapleFreeRTOS821.h>

#define LED_PIN_RED PA1
#define LED_PIN_GREEN PA2
#define LED_PIN_BLUE PA3

int pwm;
bool rise;

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

static void vLEDBlueFade(void *pvParameters)
{
    while(1)
    {
        pwm = rise ? (pwm + 1) : (pwm - 1);

        analogWrite(LED_PIN_BLUE, pwm);

        if(pwm < 5 || pwm > 150)
        {
            rise = !rise;
        }

        vTaskDelay(15);
    }
}

static void vLEDGreenBlink(void *pvParameters)
{
    while(1)
    {
        vTaskDelay(100);
        digitalWrite(LED_PIN_GREEN, HIGH);
        vTaskDelay(100);
        digitalWrite(LED_PIN_GREEN, LOW);
    }
}

void setup()
{
    // initialize the digital pin as an output:
    pinMode(LED_PIN_RED, OUTPUT);
    pinMode(LED_PIN_BLUE, OUTPUT);
    pinMode(LED_PIN_GREEN, OUTPUT);

    pwm = 100;
    rise = true;

    xTaskCreate(vLEDRedBlink,
                "Task1",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 2,
                NULL);
    //vTaskStartScheduler();

    xTaskCreate(vLEDGreenBlink,
                "Task2",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 2,
                NULL);

    xTaskCreate(vLEDBlueFade,
                "Task3",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 2,
                NULL);

    vTaskStartScheduler();
}

void loop()
{
    // Insert background code here
}
