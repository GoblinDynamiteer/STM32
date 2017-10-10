
/*
2. Få koden att blinka 1 gång i sekunden:
Ändra värdena i vTaskDelay så att LED:en blinkar en
gång i sekunden.

3. Extern LED, dim:
Sladda upp en extern LED i stället och få denna att
dimma upp och ner långsamt.

4. 2 externa LED:s dimmar i otakt:
Lägg till en ytterligare extern LED och koda så att
denna externa LED dimmar 1,36 gånger snabbare än den
första LED:en. Använd valfri kodmetod.

*/

#include <MapleFreeRTOS821.h>

#define LED_PIN_RED PA1
#define LED_PIN_GREEN PA2
#define LED_PIN_BLUE PA3
#define LED_ON_BOARD PC13

#define DIM_DELAY_RED 20
#define DIM_DELAY_GREEN 13 // 20 * 1.64

/* Fade red LED */
static void vLEDDimRed(void *pvParameters)
{
    while(1)
    {
        static int pwm = 100;
        static bool rise = true;

        pwm = rise ? (pwm + 1) : (pwm - 1);

        analogWrite(LED_PIN_RED, pwm);

        if(pwm < 5 || pwm > 150)
        {
            rise = !rise;
        }

        vTaskDelay(DIM_DELAY_RED);
    }
}

/* Fade red LED */
static void vLEDDimGreen(void *pvParameters)
{
    while(1)
    {
        static int pwm = 100;
        static bool rise = true;

        pwm = rise ? (pwm + 1) : (pwm - 1);

        analogWrite(LED_PIN_GREEN, pwm);

        if(pwm < 5 || pwm > 150)
        {
            rise = !rise;
        }

        vTaskDelay(DIM_DELAY_GREEN);
    }
}

void setup()
{
    pinMode(LED_PIN_RED, OUTPUT);
    pinMode(LED_PIN_GREEN, OUTPUT);

    xTaskCreate(vLEDDimRed,
                "REDFADE",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 2,
                NULL);

    xTaskCreate(vLEDDimGreen,
                "GREENFADE",
                configMINIMAL_STACK_SIZE,
                NULL,
                tskIDLE_PRIORITY + 2,
                NULL);

    vTaskStartScheduler();
}

void loop()
{

}
