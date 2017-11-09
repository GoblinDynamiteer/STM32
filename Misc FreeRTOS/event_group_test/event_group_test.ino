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

/* Task som skriver till h�ndelsegrupp */
static void setter(void *pvParameters)
{
    bool on = false;
    while(1)
    {
        /* S�tt bitar, varannan g�ng s�tts EVENT_GROUP_LED_STATUS_BIT */
        xEventGroupSetBits(event_group,
            EVENT_GROUP_LED_CONTROL_BIT | (on ? EVENT_GROUP_LED_STATUS_BIT : 0));

        vTaskDelay(2000);

        on = !on;
    }
}

/* Task som l�ser h�ndelsegrupp */
static void reader(void *pvParameters)
{
    EventBits_t bits;
    pinMode(PC13, OUTPUT);;

    while(1)
    {
        /* V�nta (3000) p� att n�gon bit s�tts */
        bits = xEventGroupWaitBits(
            event_group,                    // H�ndelsegrupp
            EVENT_GROUP_LED_CONTROL_BIT |
                EVENT_GROUP_LED_STATUS_BIT, // Bitar att v�nta p�
            pdTRUE,                         // Rensa bitar efter l�sning
            pdFALSE,                        // V�nta INTE p� att b�da bitar s�tts
            3000);                          // Tid att v�nta

        /* T�nd onboard LED om EVENT_GROUP_LED_STATUS_BIT �r satt, annars sl�ck */
        digitalWrite(PC13, (bits & EVENT_GROUP_LED_STATUS_BIT));

    }
}

void loop()
{
    ;
}
