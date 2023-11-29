/**
 * Main file for running the BLE samples.
 */

#include "BLEUtils.h"
#include "BLEServer.h"
#include <esp_log.h>
#include <string>
#include <sys/time.h>
#include <sstream>
#include "BLEDevice.h"
#include "BLE2902.h"
#include <Task.h>
#include "GeneralUtils.h"

#include "sdkconfig.h"

/**
 * Create a BLE Server that when it receive a read request from a BLE client for the value
 * of a characteristic will have the BLECharacteristicCallback invoked in its onRead() method.
 * This can be then used to set the value of the corresponding characteristic which will then
 * be returned back to the client.
 */
#define SERVICE_UUID        "4fafc201-1fb5-459e-8fcc-c5c9c331914b"
#define CHARACTERISTIC_UUID "beb5483e-36e1-4688-b7f5-ea07361b26a8"

static uint8_t  SERVICE_UUID_BIN[] = {0x4f, 0xaf, 0xc2, 0x01, 0x1f, 0xb5, 0x45, 0x9e, 0x8f, 0xcc, 0xc5, 0xc9, 0xc3, 0x31, 0x91, 0x4b};
static char LOG_TAG[] = "SampleWrite";

/**
 * Create a BLE Server such that when a client connects and requests a change to the characteristic
 * value, the callback associated with the server will be invoked such that the server can perform
 * some action based on the new value.  The action in this sample is merely to log the new value to
 * the console.
 */




class MyCallbacks: public BLECharacteristicCallbacks {
	void onWrite(BLECharacteristic *pCharacteristic) {
		std::string value = pCharacteristic->getValue();
		if (value.length() > 0) {
				ESP_LOGD(LOG_TAG, "*********");
				ESP_LOGD(LOG_TAG, "New value: %.2x", value[0]);
				ESP_LOGD(LOG_TAG, "*********");
			}
		}
};
/* class MyCallbackHandler: public BLECharacteristicCallbacks {
	void onRead(BLECharacteristic *pCharacteristic) {
		struct timeval tv;
		gettimeofday(&tv, nullptr);
		std::ostringstream os;
		os << "Time: " << tv.tv_sec;
		pCharacteristic->setValue(os.str());
	}
}; */


extern "C" {
	void app_main(void);
}


// The list of sample entry points.
v/* oid Sample_MLE_15(void);
void Sample1(void);
void SampleAsyncScan(void);
void SampleClient(void);
void SampleClient_Notify(void);
void SampleClientAndServer(void);
void SampleClientDisconnect(void);
void SampleClientWithWiFi(void);
void SampleNotify(void); */
//void SampleRead(void);
/* void SampleScan(void);
void SampleSensorTag(void); */
void SampleServer(void);
//void SampleWrite(void);

//
// Un-comment ONE of the following
//            ---
void app_main(void) {
	//Sample_MLE_15();
	//Sample1();
	//SampleAsyncScan();
	//SampleClient();
	//SampleClient_Notify();
	//SampleClientAndServer();
	//SampleClientDisconnect();
	//SampleClientWithWiFi();
	//SampleNotify();
	//SampleRead();
	//SampleSensorTag();
	//SampleScan();
	SampleServer();
	//SampleWrite();
} // app_main



/* 
static void run() {
	GeneralUtils::dumpInfo();
	BLEDevice::init("MYDEVICE");
	BLEServer *pServer = BLEDevice::createServer();

	BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID));

	BLECharacteristic *pCharacteristic = pService->createCharacteristic(
		BLEUUID(CHARACTERISTIC_UUID),
		BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
	);

	pCharacteristic->setCallbacks(new MyCallbacks());

	pCharacteristic->setValue("Hello World");

	pService->start();

	BLEAdvertising *pAdvertising = pServer->getAdvertising();
	pAdvertising->start();
}


void SampleWrite(void)
{
	run();
}  */// app_main

/**
 * Create a new BLE server.
 */

static char LOG_TAG[] = "SampleServer";

class MainBLEServer: public Task {
	void run(void *data) {
		ESP_LOGD(LOG_TAG, "Starting BLE work!");

		BLEDevice::init("ESP32");
		BLEServer* pServer = BLEDevice::createServer();

		BLEService* pService = pServer->createService("91bad492-b950-4226-aa2b-4ede9fa42f59");

		BLECharacteristic* pCharacteristic = pService->createCharacteristic(
			BLEUUID("0d563a58-196a-48ce-ace2-dfec78acc814"),
			BLECharacteristic::PROPERTY_BROADCAST | BLECharacteristic::PROPERTY_READ  |
			BLECharacteristic::PROPERTY_NOTIFY    | BLECharacteristic::PROPERTY_WRITE |
			BLECharacteristic::PROPERTY_INDICATE
		);

		pCharacteristic->setValue("Hello World!");

		BLE2902* p2902Descriptor = new BLE2902();
		p2902Descriptor->setNotifications(true);
		pCharacteristic->addDescriptor(p2902Descriptor);

		pService->start();

		BLEAdvertising* pAdvertising = pServer->getAdvertising();
		pAdvertising->addServiceUUID(BLEUUID(pService->getUUID()));
		pAdvertising->start();

		ESP_LOGD(LOG_TAG, "Advertising started!");
		delay(1000000);
	}
};


void SampleServer(void)
{
	//esp_log_level_set("*", ESP_LOG_DEBUG);
	MainBLEServer* pMainBleServer = new MainBLEServer();
	pMainBleServer->setStackSize(20000);
	pMainBleServer->start();

} // app_main





/* static void run() {
	BLEDevice::init("MYDEVICE");
	BLEServer *pServer = BLEDevice::createServer();

	BLEService *pService = pServer->createService(BLEUUID(SERVICE_UUID_BIN, 16, true));

	BLECharacteristic *pCharacteristic = pService->createCharacteristic(
		BLEUUID(CHARACTERISTIC_UUID),
		BLECharacteristic::PROPERTY_READ | BLECharacteristic::PROPERTY_WRITE
	);

	pCharacteristic->setCallbacks(new MyCallbackHandler());

	pCharacteristic->setValue("Hello World");

	pService->start();

	BLEAdvertising *pAdvertising = pServer->getAdvertising();
	pAdvertising->start();
}

void SampleRead(void)
{
	run();
} // app_main */