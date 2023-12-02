#include "../include/BLE.h"

BLE BLE_Servidor("Renato");


extern "C"{void app_main(void);}

void connectedTask (void * parameter){
    for(;;) {
        if (BLE_Servidor.getDeviceConnected()) {
            ;//BLE_Servidor.send("Hello World!\n");
          }
        // disconnecting
        if (!BLE_Servidor.getDeviceConnected() && BLE_Servidor.getOldDeviceConnected()) {
            BLE_Servidor.startAdvertising(); // restart advertising
            printf("start advertising\n");
            BLE_Servidor.update();
        }
        // connecting
        if (BLE_Servidor.getDeviceConnected() && !BLE_Servidor.getOldDeviceConnected()) {
            // do stuff here on connecting
            BLE_Servidor.update();
        }

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