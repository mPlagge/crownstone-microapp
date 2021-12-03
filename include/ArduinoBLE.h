#include <Serial.h>
#include <microapp.h>
#include <BleUtils.h>
#include <BleDevice.h>

enum BleEventType {
	BleEventDeviceScanned,
	BleEventConnected,
	BleEventDisconnected
};

enum BleFilterType {
	BleFilterNone = 0, // default
	BleFilterAddress,
	BleFilterLocalName,
	BleFilterUuid
};

struct BleFilter {
	BleFilterType type;
	union {
		MACaddress address;
		char name[MAX_BLE_ADV_DATA_LENGTH]; // max length of name equals max advertisement length
		uuid16_t uuid;
	};
	uint16_t len; // length of the name
};


class Ble
{
private:
	Ble(){};

	BleDevice _bleDev;

	BleFilter _activeFilter;

	bool _isScanning = false;

	uintptr_t _scanned_device_callback;

	friend void handleScanEventWrapper(microapp_ble_dev_t dev); // add handleScanEvent as a friend so it can access private variables of Ble

	void handleScanEvent(microapp_ble_dev_t dev);

	bool filterScanEvent(microapp_ble_dev_t dev);

public:

	static Ble & getInstance()
	{
		// Guaranteed to be destroyed.
		static Ble instance;

		// Instantiated on first use.
		return instance;
	}

	void setEventHandler(BleEventType eventType, void (*isr)(microapp_ble_dev_t)); // registers a callback function for some event triggered within bluenet

	/*
	 * Sends command to bluenet to call registered microapp callback function upon receiving advertisements
	 */
	bool scan(bool withDuplicates = false);

	bool scanForName(const char* name, bool withDuplicates = false);

	bool scanForAddress(const char* address, bool withDuplicates = false);

	bool scanForUuid(const char* uuid, bool withDuplicates = false);

	/*
	 * Sends command to bluenet to stop calling registered microapp callback function upon receiving advertisements
	 */
	void stopScan();

	BleFilter* getFilter();

};


#define BLE Ble::getInstance()