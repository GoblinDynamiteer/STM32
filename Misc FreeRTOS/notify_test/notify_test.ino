#include <MapleFreeRTOS900.h>

static TaskHandle_t blinker_handle = NULL;

void setup()
{
    xTaskCreate(
        blinker,
        "asd",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        &blinker_handle);

    xTaskCreate(
        notifier,
        "asd",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        NULL);

    vTaskStartScheduler();
}

void loop()
{
    ;
}

static void notifier(void *pvParameters)
{
    while(1)
    {
        xTaskNotifyGive(blinker_handle);
        vTaskDelay(5000);
    }
}

static void blinker(void *pvParameters)
{
    pinMode(PC13, OUTPUT);
    bool on = true;

    while(1)
    {
        /* Blockar tills xTaskNotifyGive från notifier*/
        ulTaskNotifyTake(pdTRUE, portMAX_DELAY);

        for(uint8_t i = 0; i < 10; i++)
        {
            digitalWrite(PC13, on);
            vTaskDelay(100);
            on = !on;
        }
    }
}
