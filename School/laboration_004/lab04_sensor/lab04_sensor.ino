#include <MapleFreeRTOS900.h>

#define COUNTER_PIN_1 PA0
#define COUNTER_PIN_2 PA1
#define COUNTER_PIN_4 PA2
#define COUNTER_PIN_8 PA3

int counter_value;

struct freq {
    int LED;
    int longDelay;
    int shortDelay;
} LEDfreq[2] = {
    {PA10, 100, 10},
    {PA9,   74,  7}
};

void get_counter_value(void)
{
counter_value =
        (!digitalRead(COUNTER_PIN_1) ? 1 : 0) +
        (!digitalRead(COUNTER_PIN_2) ? 2 : 0) +
        (!digitalRead(COUNTER_PIN_4) ? 4 : 0) +
        (!digitalRead(COUNTER_PIN_8) ? 8 : 0);
}

static void vLEDFlashTask(void *pvParameters)
{
    struct freq *Lfreq = (struct freq *)pvParameters;
    pinMode(Lfreq->LED, OUTPUT);

    for (;;)
    {
        int ix;
        vTaskDelay(Lfreq->longDelay);
        get_counter_value();

        if(counter_value > 5)
        {
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
    pinMode(COUNTER_PIN_1, INPUT_PULLUP);
    pinMode(COUNTER_PIN_2, INPUT_PULLUP);
    pinMode(COUNTER_PIN_4, INPUT_PULLUP);
    pinMode(COUNTER_PIN_8, INPUT_PULLUP);

    counter_value = 0;

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

    vTaskStartScheduler();
}

void loop()
{
    // Insert background code here
}
