#include <MapleFreeRTOS900.h>

SemaphoreHandle_t sema_mutex;

void setup()
{
    sema_mutex = xSemaphoreCreateMutex();
    gpio_set_mode(GPIOC, 13, GPIO_MODE_OUTPUT);

    xTaskCreate(
        task_one,
        "t1",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        NULL);

    xTaskCreate(
        task_two,
        "t2",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        NULL);

    vTaskStartScheduler();
}

/* Task tar mutex, och tänder onboard LED */
static void task_one(void *pvParameters)
{
    while(1)
    {
        if(xSemaphoreTake(sema_mutex, 10) == pdTRUE)
        {
            gpio_write_bit(GPIOC, 13, 1);
            vTaskDelay(1000);
            xSemaphoreGive(sema_mutex);

            /* Behövde delay, annars verkade det som samma task
                återtog semafor konstant */
            vTaskDelay(1);
        }
    }
}

/* Task tar mutex, och släcker onboard LED */
static void task_two(void *pvParameters)
{
    while(1)
    {
        if(xSemaphoreTake(sema_mutex, 10) == pdTRUE)
        {
            gpio_write_bit(GPIOC, 13, 0);
            vTaskDelay(1000);
            xSemaphoreGive(sema_mutex);
            vTaskDelay(1);
        }
    }
}

void loop()
{
    ;
}
