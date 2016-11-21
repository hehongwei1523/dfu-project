// Include project header files
#include "DFUEngine.h"
#include "common/types.h"
#include "stdbool.h"

// Device Descriptor
#define  dfu10DeviceClass   0xFE
#define  dfu10DeviceSubClass  0x01
#define  dfu10DeviceProtocol  0x00

// Interface Descriptor
#define  dfuInterfaceClass  0xFE
#define  dfuInterfaceSubClass  0x01
#define  dfu10InterfaceProtocol  0x00
#define  dfu11RuntimeInterfaceProtocol  0x01
#define  dfu11DFUInterfaceProtocol  0x02

// Functional Descriptor
#define  DFU_VERSION_1_0  0x0100

typedef enum ResultCode Result;
typedef char * DFUFile;

	// Request type values
	enum
	{
		dir_host_to_device = 0,
		dir_device_to_host = 1
	};
	enum
	{
		type_standard = 0,
		type_class = 1,
		type_vendor = 2
	};
	enum
	{
		recipient_device = 0,
		recipient_interface = 1,
		recipient_endpoint = 2,
		recipient_other = 3
	};

	// Request numbers
	enum
	{
		request_detach = 0,
		request_dnload = 1,
		request_upload = 2,
		request_get_status = 3,
		request_clr_status = 4,
		request_get_state = 5,
		request_abort = 6
	};

	// Descriptor values
	enum
	{
		descriptor_device = 0x01,
		descriptor_configuration = 0x02,
		descriptor_string = 0x03,
		descriptor_interface = 0x04,
		descriptor_endpoint = 0x05,
		descriptor_device_qualifier = 0x06,
		descriptor_other_speed_configuration = 0x07,
		descriptor_interface_power = 0x08,
		descriptor_dfu_functional = 0x21
	};

	// DFU functional attributes
	enum
	{
		attribute_can_download = 1,
		attribute_can_upload = 2,
		attribute_manifestation_tolerant = 4
	};

	// DFU status codes
	enum
	{
		ok = 0x00,				// No error condition is present
		err_target = 0x01,		// DFU file not for this device
		err_file = 0x02,		// DFU file fails verification
		err_write = 0x03,		// Device unable to write memory
		err_erase = 0x04,		// Memory erase function failed
		err_check_erased = 0x05,// Memory erase check failed
		err_prog = 0x06,		// Program memory function failed
		err_verify = 0x07,		// Memory failed verification
		err_address = 0x08,		// Address outside valid range
		err_not_done = 0x09,	// Unexpected end of data
		err_firmware = 0x0a,	// Firmware corrupt - DFU mode only
		err_vendor = 0xb,		// Vendor specific error (string)
		err_usbr = 0x0c,		// Unexpected USB reset signalling
		err_por = 0x0d,			// Unexpected power-on reset
		err_unknown = 0x0e,		// Unknown DFU failure
		err_stalledpkt = 0x0f	// Unexpected request received
	};

	// DFU device states
	enum
	{
		app_idle = 0,			// Application operating normally
		app_detach = 1,			// Waiting for USB reset for DFU mode
		dfu_idle = 2,			// Waiting for requests in DFU mode
		dfu_dnload_sync = 3,	// Data received, waiting for query
		dfu_dnload_busy = 4,	// Programming memory in progress
		dfu_dnload_idle = 5,	// Waiting for download to continue
		dfu_manifest_sync = 6,	// End of data, waiting for query
		dfu_manifest = 7,		// Processing end of data
		dfu_manifest_wait_reset = 8,// Waiting for USB reset
		dfu_upload_idle = 9,	// Waiting for upload to continue
		dfu_error = 10			// Error occurred, waiting for clear
	};

#pragma pack(push, 1)

	// USB control requests setup data
	struct Setup
	{
		uint8 bmRequestTypeRecipient : 5;// Recipient
		uint8 bmRequestTypeType : 2;// Type of request
		uint8 bmRequestTypeDirection : 1;// Transfer direction
		uint8 bRequest;			// Specific request number
		uint16 wValue;			// Request dependent data
		uint16 wIndex;			// Request dependent data
		uint16 wLength;			// Amount of data to transfer
	};

	// USB descriptor header
	struct Descriptor
	{
		uint8 bLength;
		uint8 bDescriptorType;
	};

	// USB Configuration Descriptor
	struct ConfigurationDescriptor
	{
		uint8 bLength;
		uint8 bDescriptorType;
		uint16 wTotalLength;
		uint8 bNumInterfaces;
		uint8 bConfigurationValue;
		uint8 iConfiguration;
		uint8 bmAttributes;
		uint8 bMaxPower;
	};

	// USB Interface Descriptor
	struct InterfaceDescriptor
	{
		uint8 bLength;
		uint8 bDescriptorType;
		uint8 bInterfaceNumber;
		uint8 bAlternateSetting;
		uint8 bNumEndpoints;
		uint8 bInterfaceClass;
		uint8 bInterfaceSubClass;
		uint8 bInterfaceProtocol;
		uint8 iInterface;
	};

	// USB Device Descriptor (only used fields commented)
	struct DeviceDescriptor
	{
		uint8 bLength;
		uint8 bDescriptorType;
		uint16 bcdUSB;
		uint8 bDeviceClass;		// Class code
		uint8 bDeviceSubClass;	// Subclass code
		uint8 bDeviceProtocol;	// Protocol code
		uint8 bMaxPacketSize0;
		uint16 idVendor;		// Vendor ID
		uint16 idProduct;		// Product ID
		uint16 bcdDevice;		// Device release number
		uint8 iManufacturer;
		uint8 iProduct;
		uint8 iSerialNumber;
		uint8 bNumConfigurations;
	};

	// DFU Functional Descriptor (only used fields commented)

	struct DFU10FunctionalDescriptor
	{
		uint8 bLength;
		uint8 bDescriptorType;
		uint8 bmAttributes;		// DFU attributes
		uint16 wDetachTimeout;	// Maximum detact timeout
		uint16 wTransferSize;	// Maximum transfer size
	};
	struct DFUFunctionalDescriptor
	{
		uint8 bLength;
		uint8 bDescriptorType;
		uint8 bmAttributes;		// DFU attributes
		uint16 wDetachTimeout;	// Maximum detact timeout
		uint16 wTransferSize;	// Maximum transfer size
		uint16 bcdDFUVersion;	// DFU protocol version (0x0100 = DFU1.0, 0x0101 = DFU1.1)
	};
	typedef struct DFUFunctionalDescriptor DFU11FunctionalDescriptor;   

  	// DFU Status
	struct DFUStatus
	{
		uint32 bStatus : 8;
		uint32 bwPollTimeout : 24;
		uint8 bState;
		uint8 iString;
	};  

#pragma pack(pop)

    
	// Connection and disconnection
extern Result Connect(bool hintDFU);
extern	Result Disconnect();
 
	// Non USB DFU control requests
extern	Result Reset(bool wait);
extern	Result GetInterfaceDFU(struct InterfaceDescriptor *descriptor);
extern	Result GetDevice(struct DeviceDescriptor *descriptor);
extern	Result GetFunct(struct DFUFunctionalDescriptor *descriptor);
	//Result GetString(uint8 index, CStringX &descriptor);

extern Result GetState(uint8 *state);
extern Result GetStatus(struct DFUStatus *status);
extern Result ClrStatus();
extern Result Detach(uint16 timeout);

extern Result Dnload(uint16 blockNum, const void *buffer, uint16 bufferLength);

	// Pre-download preparation step
extern	Result PreDnload(DFUFile file);

	// Conversion of DFU status codes to DFUEngine result codes
extern	Result MapDFUStatus(uint8 status, uint8 stringIndex);

	// Check whether actions are appropriate to the current state
extern	bool CanAbort(uint8 state);
extern	bool CanClrStatus(uint8 state);

extern Result Abort(void);

extern Result RPCGetState(uint8 *state);
extern Result RPCGetStatus(struct DFUStatus *status);
 
extern bool connected; //BCSP连接标志位

