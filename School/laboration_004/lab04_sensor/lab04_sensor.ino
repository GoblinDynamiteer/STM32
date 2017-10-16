#include <MapleFreeRTOS900.h>

#define COUNTER_CLICKER_PIN_AMOUNT 4
#define COUNTER_CLICKER_TRESHOLD 6

/* Pins for clicker counter "binary value" pins */
int counter_pins[COUNTER_CLICKER_PIN_AMOUNT] = {
    PA0, PA1, PA2, PA3
};

struct freq {
    int LED;
    int longDelay;
    int shortDelay;
} LEDfreq[2] = {
    {PA10, 10, 3},
    {PA9, 10, 3}
};

SemaphoreHandle_t semaphore = NULL;

/* Check clicker counter value */
static void pollClickerCounter(void *pvParameters)
{
    static int counter_value = 0;

    while(1)
    {
        if(xSemaphoreTake(semaphore, (TickType_t)100) == pdTRUE)
        {
            /*  Keep semaphore while counter value is under treshold,
                LEDs will stop fading. */
            do{
                counter_value = 0;
                for(int i = 0; i < COUNTER_CLICKER_PIN_AMOUNT; i++)
                {
                    counter_value |= (!digitalRead(counter_pins[i]) ?
                        (1 << i) : 0);
                }
            }while(counter_value < COUNTER_CLICKER_TRESHOLD);

            xSemaphoreGive(semaphore);
        }

        else;
    }
}

/* Fade LEDs while clicker counter value is >= 6 */
static void vLEDFlashTask(void *pvParameters)
{
    struct freq *Lfreq = (struct freq *)pvParameters;
    pinMode(Lfreq->LED, OUTPUT);

    for (;;)
    {
        /* Do one full fade cycle if semaphore is available */
        if(xSemaphoreTake(semaphore, (TickType_t)0) == pdTRUE)
        {
            xSemaphoreGive(semaphore); // Give semaphore and complete cycle

            int ix;
            vTaskDelay(Lfreq->longDelay);

            for (ix = 0; ix < 256; ix++)
            {
                analogWrite(Lfreq->LED, ix);
                vTaskDelay(Lfreq->shortDelay);
            }

            vTaskDelay(Lfreq->longDelay);

            for (ix = 255; ix >= 0; ix--)
            {
                analogWrite(Lfreq->LED, ix);
                vTaskDelay(Lfreq->shortDelay);
            }
        }
    }
}

void setup()
{
    for(int i = 0; i < COUNTER_CLICKER_PIN_AMOUNT; i++)
    {
        pinMode(counter_pins[i], INPUT_PULLUP);
    }

    semaphore = xSemaphoreCreateMutex();

    xTaskCreate(vLEDFlashTask,
        "LEDblink1",
        configMINIMAL_STACK_SIZE,
        (void *)&LEDfreq[0],
        tskIDLE_PRIORITY + 2,
        NULL);

    xTaskCreate(vLEDFlashTask,
        "LEDblink2",
        configMINIMAL_STACK_SIZE,
        (void *)&LEDfreq[1],
        tskIDLE_PRIORITY + 2,
        NULL);

    xTaskCreate(pollClickerCounter,
        "Poller",
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
