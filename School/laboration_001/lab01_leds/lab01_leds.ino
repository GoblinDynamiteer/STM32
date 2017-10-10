
/*
2. F� koden att blinka 1 g�ng i sekunden:
�ndra v�rdena i vTaskDelay s� att LED:en blinkar en
g�ng i sekunden.

3. Extern LED, dim:
Sladda upp en extern LED i st�llet och f� denna att
dimma upp och ner l�ngsamt.

4. 2 externa LED:s dimmar i otakt:
L�gg till en ytterligare extern LED och koda s� att
denna externa LED dimmar 1,36 g�nger snabbare �n den
f�rsta LED:en. Anv�nd valfri kodmetod.

5. Kodrationalisering:

Omforma nu koden s� att du bara har en funktion f�r
att blinka b�gge LED:ar, men startar tv� tasks med
denna enda funktion, en task f�r vardera LED. (Hint:
anv�nd parametern pvParameters! Unders�k hur
pvParameters kan anv�ndas genom att sl� upp
xTaskCreate p� www.freertos.org!)

*/

#include <MapleFreeRTOS821.h>

#define LED_PIN_RED PA1
#define LED_PIN_GREEN PA2
#define LED_PIN_BLUE PA3
#define LED_ON_BOARD PC13

#define DIM_DELAY_RED 20
#define DIM_DELAY_GREEN 13 // 20 * 1.64

static int settings_red = LED_PIN_RED;
static int settings_green = LED_PIN_GREEN;

/* Blink LED */
static void vLEDBlink(void *pvParameters)
{
    while(1)
    {
        vTaskDelay(1000);
        digitalWrite((int)pvParameters, HIGH);
        vTaskDelay(50);
        digitalWrite((int)pvParameters, LOW);
    }
}

void setup()
{
    pinMode(LED_PIN_RED, OUTPUT);
    pinMode(LED_PIN_GREEN, OUTPUT);

    xTaskCreate(vLEDBlink,
                "REDBINK",
                configMINIMAL_STACK_SIZE,
                (void*)settings_red,
                tskIDLE_PRIORITY + 2,
                NULL);

    xTaskCreate(vLEDBlink,
                "GREENBLINK",
                configMINIMAL_STACK_SIZE,
                (void*)settings_green,
                tskIDLE_PRIORITY + 2,
                NULL);

    vTaskStartScheduler();
}

void loop()
{

}
