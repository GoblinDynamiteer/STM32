#include <MapleFreeRTOS900.h>

SemaphoreHandle_t sema_counter;

#define SEMA_COUNT 123 // Antal "nycklar" i räknesemaforen
uint8_t counter;

void setup()
{
    sema_counter = xSemaphoreCreateCounting(SEMA_COUNT, 0);

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

    xTaskCreate(
        task_three,
        "t2",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        NULL);

    vTaskStartScheduler();
}

static void task_one(void *pvParameters)
{
    gpio_set_mode(GPIOC, 13, GPIO_MODE_OUTPUT);
    bool on = true;
    counter = SEMA_COUNT;

    while(1)
    {
        /*  Om counter har nått SEMA_COUNT, så har räknesemaforen
            tagits max antal gånger */
        if(counter == SEMA_COUNT)
        {
            /* Toggla onboard LED */
            gpio_write_bit(GPIOC, 13, on);
            on = !on;

            counter = 0; // Återställ räknare

            /* Släpp alla nycklar i räknesemaforen */
            for(uint8_t i = 0; i < SEMA_COUNT; i++)
            {
                xSemaphoreGive(sema_counter);
            }

            vTaskDelay(1000);
        }
    }
}

/* Tar "nycklar" ur räknesemaforen */
static void task_two(void *pvParameters)
{
    while(1)
    {
        if(xSemaphoreTake(sema_counter, 200) == pdTRUE)
        {
            /* Ökar den globala variabeln, som borde nå samma antal
                som max antal nyckalr i räknesemaforen */
            counter++;
        }
    }
}

/* Tar också "nycklar" ur räknesemaforen! */
static void task_three(void *pvParameters)
{
    while(1)
    {
        if(xSemaphoreTake(sema_counter, 200) == pdTRUE)
        {
            /* Ökar den globala variabeln, som borde nå samma antal
                som max antal nyckalr i räknesemaforen */
            counter++;
        }
    }
}

void loop()
{
    ;
}
