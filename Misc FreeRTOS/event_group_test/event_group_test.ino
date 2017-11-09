#include <MapleFreeRTOS900.h>

#define EVENT_GROUP_LED_CONTROL_BIT (1 << 4)
#define EVENT_GROUP_LED_STATUS_BIT (1 << 5)
EventGroupHandle_t event_group;

void setup()
{
    event_group = xEventGroupCreate();

    xTaskCreate(
        setter,
        "eg_setter",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        NULL);

    xTaskCreate(
        reader,
        "eg_reader",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        NULL);

    vTaskStartScheduler();
}

/* Task som skriver till händelsegrupp */
static void setter(void *pvParameters)
{
    bool on = false;
    while(1)
    {
        /* Sätt bitar, varannan gång sätts EVENT_GROUP_LED_STATUS_BIT */
        xEventGroupSetBits(event_group,
            EVENT_GROUP_LED_CONTROL_BIT | (on ? EVENT_GROUP_LED_STATUS_BIT : 0));

        vTaskDelay(2000);

        on = !on;
    }
}

/* Task som läser händelsegrupp */
static void reader(void *pvParameters)
{
    EventBits_t bits;
    pinMode(PC13, OUTPUT);;

    while(1)
    {
        /* Vänta (3000) på att någon bit sätts */
        bits = xEventGroupWaitBits(
            event_group,                    // Händelsegrupp
            EVENT_GROUP_LED_CONTROL_BIT |
                EVENT_GROUP_LED_STATUS_BIT, // Bitar att vänta på
            pdTRUE,                         // Rensa bitar efter läsning
            pdFALSE,                        // Vänta INTE på att båda bitar sätts
            3000);                          // Tid att vänta

        /* Tänd onboard LED om EVENT_GROUP_LED_STATUS_BIT är satt, annars släck */
        digitalWrite(PC13, (bits & EVENT_GROUP_LED_STATUS_BIT));

    }
}

void loop()
{
    ;
}
