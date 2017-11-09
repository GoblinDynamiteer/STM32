#include <MapleFreeRTOS900.h>

SemaphoreHandle_t sema_counter;

#define SEMA_COUNT 123 // Antal "nycklar" i r�knesemaforen
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
        /*  Om counter har n�tt SEMA_COUNT, s� har r�knesemaforen
            tagits max antal g�nger */
        if(counter == SEMA_COUNT)
        {
            /* Toggla onboard LED */
            gpio_write_bit(GPIOC, 13, on);
            on = !on;

            counter = 0; // �terst�ll r�knare

            /* Sl�pp alla nycklar i r�knesemaforen */
            for(uint8_t i = 0; i < SEMA_COUNT; i++)
            {
                xSemaphoreGive(sema_counter);
            }

            vTaskDelay(1000);
        }
    }
}

/* Tar "nycklar" ur r�knesemaforen */
static void task_two(void *pvParameters)
{
    while(1)
    {
        if(xSemaphoreTake(sema_counter, 200) == pdTRUE)
        {
            /* �kar den globala variabeln, som borde n� samma antal
                som max antal nyckalr i r�knesemaforen */
            counter++;
        }
    }
}

/* Tar ocks� "nycklar" ur r�knesemaforen! */
static void task_three(void *pvParameters)
{
    while(1)
    {
        if(xSemaphoreTake(sema_counter, 200) == pdTRUE)
        {
            /* �kar den globala variabeln, som borde n� samma antal
                som max antal nyckalr i r�knesemaforen */
            counter++;
        }
    }
}

void loop()
{
    ;
}
