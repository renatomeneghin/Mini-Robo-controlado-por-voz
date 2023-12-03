#include "../include/BLE.h"

BLE BLE_Servidor("Renato");


extern "C"{void app_main(void);}

void connectedTask (void * parameter){
    for(;;) {
        if (BLE_Servidor.getBLEEnvio()) {
            BLE_Servidor.setBLEEnvio(false);
            BLE_Servidor.send("Hello World!");
          }
        // disconnecting


        vTaskDelay(10/portTICK_PERIOD_MS); // Delay between loops to reset watchdog timer
    }

    vTaskDelete(NULL);
}

void app_main(void) {
  // Create the BLE Device
  BLE_Servidor.initFull();

  xTaskCreate(connectedTask, "connectedTask", 5000, NULL, 1, NULL);

  printf("Waiting a client connection to notify...\n");
}