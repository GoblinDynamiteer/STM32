
/*
2. F� koden att blinka 1 g�ng i sekunden:
�ndra v�rdena i vTaskDelay s� att LED:en blinkar en
g�ng i sekunden.


*/

#include <MapleFreeRTOS821.h>

#define LED_PIN_RED PA1
#define LED_PIN_GREEN PA2
#define LED_PIN_BLUE PA3
#define LED_ON_BOARD PC13

/* Blink Red LED once per seconds */
static void vLEDBlink(void *pvParameters)
{
    while(1)
    {
        vTaskDelay(1000);
        digitalWrite(LED_ON_BOARD, HIGH);
        vTaskDelay(50);
        digitalWrite(LED_ON_BOARD, LOW);
    }
}
void setup()
{
    pinMode(LED_ON_BOARD, OUTPUT);

    xTaskCreate(vLEDBlink,
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
