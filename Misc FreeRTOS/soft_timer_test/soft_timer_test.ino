#include <MapleFreeRTOS900.h>

TimerHandle_t timer;
bool on = true;

void setup()
{
    pinMode(PC13, OUTPUT);

    timer = xTimerCreate(
        "timer",
        2000,       // Timerns nedräkningstid
        pdTRUE,     // Ladda om timer
        (void*)0,
        timer_func  // Funktion som kör när timer klar
    );

    xTimerStart(timer, 0);  // Starta timer

    
    vTaskStartScheduler();
}

void timer_func(TimerHandle_t xTimer)
{
    for(uint8_t i = 0; i < 10; i++)
    {
        digitalWrite(PC13, on);
        vTaskDelay(50);
        on = !on;
    }
}

void loop()
{
}
