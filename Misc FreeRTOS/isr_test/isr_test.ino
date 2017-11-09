#include <MapleFreeRTOS900.h>

volatile bool pressed;

void setup()
{
    pressed = false;

    /* Intern pullup på PA2 */
    pinMode(PA2, INPUT_PULLUP);

    /*  Koppla ISR till PA2 pin, triggar när pinn går till låg,
        (exempelvis med en knapp som jordar pin) */
    attachInterrupt(PA2, button_isr, FALLING);

    xTaskCreate(
        blinker,
        "t1",
        configMINIMAL_STACK_SIZE,
        NULL,
        tskIDLE_PRIORITY + 2,
        NULL);

    vTaskStartScheduler();
}

/* Task som blinkar onboard LED om ISR har triggats */
void blinker(void *pvParameters)
{
    pinMode(PC13, OUTPUT);
    bool on = true;

    while(1)
    {
        if(pressed)
        {
            /* Blinka onboard LED */
            for(uint8_t i = 0; i < 10; i++)
            {
                digitalWrite(PC13, on);
                on = !on;
                vTaskDelay(100);
            }

            pressed = false;
        }
    }
}

void button_isr(void)
{
    pressed = true;
}

void loop()
{
    ;
}
