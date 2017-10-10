
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

6. Undersökning - stackstorlek:

Ta reda på hur stor configMINIMAL_STACK_SIZE är, det
skall finnas i källkoden till biblioteket
MapleFreeRTOS900 på din dator. I stället för att
använda configMINIMAL_STACK_SIZE när du skapar
ett task, testa med ett mindre värde och ladda upp.
Minska mer och mer tills sketchen inte fungerar längre.

Ett allmänråd när det gäller configMINIMAL_STACK_SIZE
är att om en sketch slutar fungera, så försök öka
storleken på stackstorleken.

Återställ configMINIMAL_STACK_SIZE i koden!

7. Undersökning - olika prioritet:
Ändra prioriteten på en task, behåll den andra.
Notera vad som händer!

*/

#include <MapleFreeRTOS821.h>

#define LED_PIN_RED PA1
#define LED_PIN_GREEN PA2

#define DIM_DELAY_RED 500
#define DIM_DELAY_GREEN 1000

struct _settings{
    int pin;
    int delay;
}settings[] =
{
    { LED_PIN_RED, DIM_DELAY_RED },
    { LED_PIN_GREEN, DIM_DELAY_GREEN }
};

/* Blink LED */
static void vLEDBlink(void (*pvParameters))
{
    while(1)
    {
        _settings * s = (_settings *)pvParameters;
        vTaskDelay(s->delay);
        digitalWrite(s->pin, HIGH);
        vTaskDelay(50);
        digitalWrite(s->pin, LOW);
    }
}

void setup()
{
    pinMode(LED_PIN_RED, OUTPUT);
    pinMode(LED_PIN_GREEN, OUTPUT);

    xTaskCreate(vLEDBlink,
                "REDBINK",
                configMINIMAL_STACK_SIZE,
                (void*)&settings[0],
                tskIDLE_PRIORITY + 2,
                NULL);

    xTaskCreate(vLEDBlink,
                "GREENBLINK",
                configMINIMAL_STACK_SIZE,
                (void*)&settings[1],
                tskIDLE_PRIORITY + 2,
                NULL);

    vTaskStartScheduler();
}

void loop()
{

}
