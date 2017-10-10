
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

5. Kodrationalisering:

Omforma nu koden så att du bara har en funktion för
att blinka bägge LED:ar, men startar två tasks med
denna enda funktion, en task för vardera LED. (Hint:
använd parametern pvParameters! Undersök hur
pvParameters kan användas genom att slå upp
xTaskCreate på www.freertos.org!)

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
