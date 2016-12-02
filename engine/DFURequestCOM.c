
#include "DFURequest.h"
#include "DFURequestCOM.h"
#include "DFUTransportCOM.h"
#include "bcspimplementation.h"
// Standard baud rates
//static const int standardBaudRates[] = {9600, 14400, 19200, 38400, 57600, 115200, 230400, 460800, 921600, 1382400};

#define ONESTOPBIT          0
#define ONE5STOPBITS        1
#define TWOSTOPBITS         2

#define NOPARITY            0
#define ODDPARITY           1
#define EVENPARITY          2
#define MARKPARITY          3
#define SPACEPARITY         4

// BCSP sync message
//static const uint8 bcspSync[] = {0xDA, 0xDC, 0xED, 0xED};

// USB control request status
enum usbStatus
{
	ack = 0,				// Completed successfully
	nak = 1,				// Still processing command (no data)
	stall = 2				// Error prevented completion (no data)
};

// Vendor specific DFU control requests for UART use only
static const uint16 dfuReset = 7;
static const uint16 dfuGetDevice = 8;
static const uint16 dfuGetFunct = 9;

// Fixed DFU interface number
static const uint16 interfaceNumber = 0;

// Maximum number of connection retry attempts
static const int retriesConnect = 3;

  
enum Protocol protocol;
enum LinkEstablishment sync;
 
// Constructors
void DFURequestsCOM()
{
	protocol = protocol_unknown;
	sync = sync_unknown;
}

Result RPCGetInterfaceDFU(struct InterfaceDescriptor *descriptor)
{
	descriptor->bLength = sizeof(descriptor);
	descriptor->bDescriptorType = descriptor_interface;
	// Interface number is constant for UART transports
	descriptor->bInterfaceNumber = interfaceNumber;
	descriptor->bAlternateSetting = 0;
	descriptor->bNumEndpoints = 0;
	descriptor->bInterfaceClass = dfuInterfaceClass;
	descriptor->bInterfaceSubClass = dfuInterfaceSubClass;
	descriptor->bInterfaceProtocol = dfu10InterfaceProtocol;
	descriptor->iInterface = 0;
	return success;
}

// Non USB DFU control requests (implemented here as USB control requests)
Result RPCReset()
{
	// Prepare the setup data
	struct Setup setup;
	setup.bmRequestTypeRecipient = recipient_interface;
	setup.bmRequestTypeType = type_vendor;
	setup.bmRequestTypeDirection = dir_host_to_device;
	setup.bRequest = dfuReset;
	setup.wValue = 0;
	setup.wIndex = interfaceNumber;
	setup.wLength = 0;

	// Perform the request (failure to receive a reply is not a failure)
	Result result = ControlRequest(setup, null, 0, null);
    
	if (!result && (result != fail_com_timeout_rx))
	{
		return result;
	}
    
	// Need to disconnect the transport before the device has reset
	result = Disconnect();
	if (!result) return result;

	// Successful if this point reached
	return success;
}


Result RPCGetDevice(struct DeviceDescriptor *descriptor)
{
  Result result;
  
	// Buffer to receive the reply
	struct 
	{
		uint16 bDeviceClass;
		uint16 bDeviceSubClass;
		uint16 bDeviceProtocol;
		uint16 idVendor;
		uint16 idProduct;
		uint16 bcdDevice;
	} reply;
    
	// Prepare the setup data
	struct Setup setup;
	setup.bmRequestTypeRecipient = recipient_interface;
	setup.bmRequestTypeType = type_vendor;
	setup.bmRequestTypeDirection = dir_device_to_host;
	setup.bRequest = dfuGetDevice;
	setup.wValue = 0;
	setup.wIndex = interfaceNumber;
	setup.wLength = sizeof(reply);

	// Perform the request
	uint16 replyLength;
    
    //uint8 buff[10] = {0x0a,0x0b};
    //result = ControlRequest(setup, &buff, sizeof(reply), &replyLength);
	result = ControlRequest(setup, &reply, sizeof(reply), &replyLength);

	// Extract the data from the reply
	if (result && (replyLength < sizeof(reply))) result = fail_reply_short;
	if (result)
	{
		descriptor->bLength = sizeof(descriptor);
		descriptor->bDescriptorType = descriptor_device;
		descriptor->bDeviceClass = (uint8)reply.bDeviceClass;
		descriptor->bDeviceSubClass = (uint8)reply.bDeviceSubClass;
		descriptor->bDeviceProtocol = (uint8)reply.bDeviceProtocol;
		descriptor->idVendor = reply.idVendor;
		descriptor->idProduct = reply.idProduct;
		descriptor->bcdDevice = reply.bcdDevice;
	}

	// Return the result
   
	return result;
}

Result AutomaticPassiveBCSP()
{
  return success;
}

extern uint8 LinkEstablishment_Flag;
extern uint8 rcv_count;
Result RPCConnect(bool hintDFU)  //与芯片建立BCSP联系
{
    Result result;
//free(bcspImplementation.mStack);
	//BCSPshutdownStack(bcspImplementation.mStack);
	BCSPImplementation_deleteBCSP();
	/*每次连接都初始化BCSP参数*/
	BCSPImplementation_Environment();
	BCSPTransport();
	
	

	rcv_count = 1; 
	LinkEstablishment_Flag = 0x00;
	uint32 time_begin = ms_clock();
	while ( LinkEstablishment_Flag != 0x02)
	{
		if ((ms_clock() - time_begin) > 6000)
		{
			printf("link time out !\n");
			return fail_com_fail;//返回错误
		}
	}
	connected = 1;

	printf("RPCConnect !\n");

	// Successful if this point reached
	Progress(transport_established);
 
	return success;
}


Result AutomaticActiveBCSP(bool link)
{
	Result result = fail_com_connect;
       
	// Return the result
	return result;
}

// A request that safely tests private channels
Result TestPrivateChannel()
{
	struct DeviceDescriptor descriptor;
	//return RPCGetDevice(descriptor);

	return success;
}

Result ConnectBCSP(const void *port,int baud, bool sync, bool tunneling,void * handle)
{
	// End any existing transport
	Result result = Disconnect();
	if (!result) return result;
	// Attempt a connection
	//result = Connect(port, baud, null);
	if (!result) return result;

	// Successful if this point reached
	return success;
}

Result RPCGetFunct(struct DFUFunctionalDescriptor *descriptor)
{
	// Buffer to receive the reply
	struct
	{
		uint16 bmAttributes;
		uint16 wDetachTimeout;
		uint16 wTransferSize;
	} reply;

	// Prepare the setup data
	struct Setup setup;
	setup.bmRequestTypeRecipient = recipient_interface;
	setup.bmRequestTypeType = type_vendor;
	setup.bmRequestTypeDirection = dir_device_to_host;
	setup.bRequest = dfuGetFunct;
	setup.wValue = 0;
	setup.wIndex = interfaceNumber;
	setup.wLength = sizeof(reply);

	// Perform the request
	uint16 replyLength;
	Result result = ControlRequest(setup, &reply, sizeof(reply), &replyLength);

	// Extract the data from the reply
	if (result && (replyLength < sizeof(reply))) result = fail_reply_short;
	if (result)
	{
		descriptor->bLength = sizeof(struct DFUFunctionalDescriptor);
		descriptor->bDescriptorType = descriptor_dfu_functional;
		descriptor->bmAttributes = (uint8)(reply.bmAttributes);
		descriptor->wDetachTimeout = reply.wDetachTimeout;
		descriptor->wTransferSize = reply.wTransferSize;
		descriptor->bcdDFUVersion = DFU_VERSION_1_0;
		//transport.MaximumLength(sizeof(setup) + reply.wDetachTimeout);
	}

	// Return the result
	return result;
}



