
/*
2. F� koden att blinka 1 g�ng i sekunden:
�ndra v�rdena i vTaskDelay s� att LED:en blinkar en
g�ng i sekunden.


*/

#include <MapleFreeRTOS821.h>

#define LED_PIN_RED PA1
#define LED_PIN_GREEN PA2
#define LED_PIN_BLUE PA3

/* Blink Red LED once per seconds */
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
void setup()
{
    pinMode(LED_PIN_RED, OUTPUT);

    xTaskCreate(vLEDRedBlink,
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
