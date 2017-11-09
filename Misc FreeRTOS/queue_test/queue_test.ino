#include <MapleFreeRTOS900.h>
QueueHandle_t queue;

void setup()
{
    queue = xQueueCreate(10, sizeof(uint8_t));

    xTaskCreate(
        sender,
        "s",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        NULL);

    xTaskCreate(
        reciever,
        "r",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        NULL);

    vTaskStartScheduler();
}

static void sender(void *pvParameters)
{

    uint8_t data = 2;
    TickType_t xLastWakeTime;
    const TickType_t xFrequency = 2000;

    while(1)
    {
        data = data < 10 ? data + 1 : 1;
        xQueueSend(queue, (void*)&data, 100);
        vTaskDelayUntil(&xLastWakeTime, xFrequency);
    }
}

static void reciever(void *pvParameters)
{
    pinMode(PC13, OUTPUT);
    uint8_t recieved_data;
    digitalWrite(PC13, 1);

    while(1)
    {
        if(xQueueReceive(queue, (void*)&recieved_data, 10) == pdTRUE)
        {
            for(uint8_t i = 0; i < recieved_data; i++)
            {
                vTaskDelay(100);
                digitalWrite(PC13, 0);
                vTaskDelay(100);
                digitalWrite(PC13, 1);
            }

            vTaskDelay(3000);
        }
    }
}

void loop()
{
}
