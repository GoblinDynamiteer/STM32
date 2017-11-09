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
        /* Sätt register bit 8 till 1 */
        xTaskNotify(blinker_handle, (1UL << 8UL), eSetBits);
        vTaskDelay(5000);
    }
}

static void blinker(void *pvParameters)
{
    pinMode(PC13, OUTPUT);
    bool on = true;
    unsigned long bit = 0UL;

    while(1)
    {

        /* Läs register till variabel bit och skriv över med nollor */
        xTaskNotifyAndQuery(
            blinker_handle,
            0UL,
            eSetValueWithOverwrite,
            &bit);

        /* Om bit 8 är 1 */
        if(bit >> 8UL & 1)
        {
            for(uint8_t i = 0; i < 10; i++)
            {
                digitalWrite(PC13, on);
                vTaskDelay(100);
                on = !on;
            }
        }
    }
}
