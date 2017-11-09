#include <MapleFreeRTOS900.h>

SemaphoreHandle_t sema_binary;

void setup()
{
    sema_binary = xSemaphoreCreateBinary();

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

/* Task som sl�pper bin�r semafor */
static void task_one(void *pvParameters)
{
    while(1)
    {
        xSemaphoreGive(sema_binary);
        vTaskDelay(2000);
    }
}

/* Task som tar bin�r semafor */
static void task_two(void *pvParameters)
{
    //pinMode(PC13, OUTPUT);

    /* Alteranativt s�tt att s�tta "pinmode" med STM32duino */
    gpio_set_mode(GPIOC, 13, GPIO_MODE_OUTPUT);
    bool on = true;

    while(1)
    {
        /* Toggla onboard LED om bin�r semafor kan bli tagen */
        if(xSemaphoreTake(sema_binary, portMAX_DELAY) == pdTRUE)
        {
            /* Olika s�tt att skriva till pins med STM32duino
            http://wiki.stm32duino.com/index.php?title=API
            Ska vara snabbare/b�ttre �n digitalWrite */

            /* Funktion */
            // gpio_write_bit(GPIOC, 13, on);

            /* Eller direkt till register (16 f�rsta bitar P�, annars AV) */
            GPIOC->regs->BSRR = (1U << 13) << (16 * on);

            on = !on;
        }
    }

}

void loop()
{
    ;
}
