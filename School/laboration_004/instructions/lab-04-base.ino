//#include <wirish/wirish.h>
//#include "libraries/FreeRTOS/MapleFreeRTOS.h"
#include <MapleFreeRTOS900.h>

#define BOARD_LED_PIN PC13

struct freq {
  int LED;
  int longDelay;
  int shortDelay;
} LEDfreq[2] = {
  {PA10, 100, 10},
  {PA9,   74,  7}
};

static void vLEDFlashTask(void *pvParameters) {
  /*int freqIx = (int)pvParameters;
    struct freq *Lfreq = &LEDfreq[freqIx];*/
  struct freq *Lfreq = (struct freq *)pvParameters;
  pinMode(Lfreq->LED, OUTPUT);
  for (;;) {
    int ix;
    vTaskDelay(Lfreq->longDelay);
    for (ix = 0; ix < 256; ix++) {
      analogWrite(Lfreq->LED, ix);
      vTaskDelay(Lfreq->shortDelay);
    }
    vTaskDelay(Lfreq->longDelay);
    for (ix = 255; ix >= 0; ix--) {
      analogWrite(Lfreq->LED, ix);
      vTaskDelay(Lfreq->shortDelay);
    }
  }
}

void setup() {
  // initialize the digital pin as an output:
  Serial.begin(19200);

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

void loop() {
  // Insert background code here
}
