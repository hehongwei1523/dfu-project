	
#include "DFURequest.h"
#include "stdbool.h"
#include "common/types.h"
#include "DFURequestCOM.h"
#include  "bcspimplementation.h"

// Is a connection active
bool connected = 0;

// Number of DFU interface, set by Connect
struct InterfaceDescriptor dfuInterfaceDescriptor;

// Pre- and post-processing of RPC operations
Result PreRPC();
Result PostRPC(Result result);
 

// Length of time after reset before device responds to communications
//static const uint32 delayResetMilliseconds = 1300;

// Mapping of DFU status codes to DFUEngine result codes
struct statusCodeMapEntry
{
	uint8 status;
	Result result;
};


// Connection and disconnection
Result Connect(bool hintDFU)
{
	if (!connected)
	{
		printf("##########Connect############ \n");
		//sendpdu_flag = 1;
        Result result = RPCConnect(hintDFU);
        if (!result) return result;
		//sendpdu_flag = 0;
        connected = true;
        result = RPCGetInterfaceDFU(&dfuInterfaceDescriptor);
        if (!result) return result;
	}
  
	return success;
}

Result Disconnect()
{
	if (connected)
	{
		connected = false;
		return RPCDisconnect();
	}
	return success;
}

Result RPCDisconnect()
{
  /* 硬件断开连接 */
	printf("Disconnect !\n");
	//BCSPImplementation_deleteBCSP();
	return success;
}


Result RPCDnload(uint16 blockNum, void *buffer, uint16 bufferLength)
{
	// Prepare the setup data
	struct Setup setup;
	setup.bmRequestTypeRecipient = recipient_interface;
	setup.bmRequestTypeType = type_class;
	setup.bmRequestTypeDirection = dir_host_to_device;
	setup.bRequest = request_dnload;
	setup.wValue = blockNum;
	setup.wIndex = dfuInterfaceDescriptor.bInterfaceNumber;
	setup.wLength = bufferLength;

    uint16 replyLength;
    /*
{
  uint16 i = 0;
  for( i=0; i<13; i++)
   printf("buffer[%d] = %x \n",i,*((uint8 *)buffer+i));
}         */
	// Perform the request
	return ControlRequest(setup, buffer, bufferLength ,&replyLength);
}

Result Dnload(uint16 blockNum, const void *buffer, uint16 bufferLength)
{
	Result result = PreRPC();
	printf("##########Dnload############ \n");
	sendpdu_flag = 2;
	if (result) result = RPCDnload(blockNum, (uint8 *)buffer, bufferLength);
	sendpdu_flag = 0;
	return PostRPC(result);
}

Result RPCUpload(uint16 blockNum, void *buffer, uint16 bufferLength, uint16 replyLength)
{
	// Prepare the setup data
	struct Setup setup;
	setup.bmRequestTypeRecipient = recipient_interface;
	setup.bmRequestTypeType = type_class;
	setup.bmRequestTypeDirection = dir_device_to_host;
	setup.bRequest = request_upload;
	setup.wValue = blockNum;
	setup.wIndex = dfuInterfaceDescriptor.bInterfaceNumber;
	setup.wLength = bufferLength;

	// Perform the request
	return ControlRequest(setup, buffer, bufferLength, &replyLength);
}
typedef struct DFUStatus dfustatus;
typedef struct DFUStatus * pdfustatus;

Result RPCGetStatus(struct DFUStatus *status)
{
	// Prepare the setup data
	struct Setup setup;
	setup.bmRequestTypeRecipient = recipient_interface;
	setup.bmRequestTypeType = type_class;
	setup.bmRequestTypeDirection = dir_device_to_host;
	setup.bRequest = request_get_status;
	setup.wValue = 0;
	setup.wIndex = dfuInterfaceDescriptor.bInterfaceNumber;
	setup.wLength = sizeof(struct DFUStatus);

	// Perform the request
	uint16 replyLength;
    //printf("sizeof(status) = %d  ",sizeof(struct DFUStatus));
    //注意参数status已经是指针，以下函数调用时，不能用&status，否则会出错
	Result result = ControlRequest(setup, status, sizeof(struct DFUStatus), &replyLength);
    
	if (!result) return result;

	// Extract the data from the reply
	if (replyLength < sizeof(status)) result = fail_reply_short;
	if (sizeof(status) < replyLength) result = fail_reply_long;

	// Return the result
	return success;
}

Result RPCClrStatus()
{
	// Prepare the setup data
	struct Setup setup;
	setup.bmRequestTypeRecipient = recipient_interface;
	setup.bmRequestTypeType = type_class;
	setup.bmRequestTypeDirection = dir_host_to_device;
	setup.bRequest = request_clr_status;
	setup.wValue = 0;
	setup.wIndex = dfuInterfaceDescriptor.bInterfaceNumber;
	setup.wLength = 0;

	// Perform the request
	return ControlRequest(setup, null, 0, null);
}



Result RPCGetState(uint8 *state)
{
	// Prepare the setup data
	struct Setup setup;
	setup.bmRequestTypeRecipient = recipient_interface;
	setup.bmRequestTypeType = type_class;
	setup.bmRequestTypeDirection = dir_device_to_host;
	setup.bRequest = request_get_state;
	setup.wValue = 0;
	setup.wIndex = dfuInterfaceDescriptor.bInterfaceNumber;
	setup.wLength = sizeof(uint8);

	// Perform the request
	uint16 replyLength;
	Result result = ControlRequest(setup, &state, sizeof(uint16), &replyLength);
	if (!result) return result;

	// Extract the data from the reply
	if (replyLength < sizeof(uint8)) result = fail_reply_short;
	if (sizeof(uint8) < replyLength) result = fail_reply_long;

	// Return the result
	return success;
}

Result RPCAbort()
{
 
	// Prepare the setup data
	struct Setup setup;
	setup.bmRequestTypeRecipient = recipient_interface;
	setup.bmRequestTypeType = type_class;
	setup.bmRequestTypeDirection = dir_host_to_device;
	setup.bRequest = request_abort;
	setup.wValue = 0;
	setup.wIndex = dfuInterfaceDescriptor.bInterfaceNumber;
	setup.wLength = 0;

	// Perform the request
	return ControlRequest(setup, null, 0, null);
 
}

// Non USB DFU control requests
Result Reset(bool wait)
{
	// Perform the reset
	Result result = PreRPC(); 
	printf("##########Reset############ \n");
	sendpdu_flag = 1;
	if (result) result = RPCReset();
	if (result) result = Disconnect();
	sendpdu_flag = 0;
	// Allow the device to complete its reset
	//if (result && wait) result = CheckAbort(delayResetMilliseconds);

	// Check the result
	return PostRPC(result);
}

Result GetInterfaceDFU(struct InterfaceDescriptor *descriptor)
{
  
	if(connected)
	{
		//descriptor = &dfuInterfaceDescriptor;  //直接让地址指向结构体时会赋值不成功，所以下面直接调用函数赋值   2016-11-24
		memcpy(descriptor, &dfuInterfaceDescriptor, sizeof(struct InterfaceDescriptor));
		return success;
	}
	else 
	{
		Result result = PreRPC();
		if (result) result = RPCGetInterfaceDFU(descriptor);
		return PostRPC(result);
	}
}

Result GetDevice(struct DeviceDescriptor *descriptor)
{
	Result result = PreRPC();
	printf("##########GetDevice############ \n");
	sendpdu_flag = 3;
	if (result) result = RPCGetDevice(descriptor);
	sendpdu_flag = 0;
	return PostRPC(result);
}

Result GetFunct(struct DFUFunctionalDescriptor *descriptor)
{
	Result result = PreRPC();
	printf("##########GetFunct############ \n");
	sendpdu_flag = 3;
	if (result) result = RPCGetFunct(descriptor);
	sendpdu_flag = 0;
	return PostRPC(result);
}


// Pre-download preparation step
Result PreDnload(DFUFile file)
{
	// This function is not normally required
	return success;
}

// Conversion of DFU status codes to DFUEngine result codes
Result MapDFUStatus(uint8 status, uint8 stringIndex)
{
 
	// Return the result
	return success;
}



// Pre- and post-processing of RPC operations
Result PreRPC()
{
	if (!connected) return fail_no_transport;
	return success;
}


Result PostRPC(Result result)
{
	if ( result == fail_dfu_stalledpkt )
	{
		// Attempt to read and clear the DFU error code if stalled response
		struct DFUStatus status;
		if (RPCGetStatus(&status))
		{
			if (status.bStatus != ok)
			{
				result = MapDFUStatus(status.bStatus, status.iString);
			}
			if (status.bState == dfu_error) RPCClrStatus();
		}
	}
  
	return result;
}

Result GetStatus(struct DFUStatus *status)
{
	Result result = PreRPC();
	printf("##########GetStatus############ \n");
	sendpdu_flag = 3;
	if (result) result = RPCGetStatus(status);
	sendpdu_flag = 0;
	return PostRPC(result);
}

Result ClrStatus()
{
	Result result = PreRPC();
	if (result) result = RPCClrStatus();
	return PostRPC(result);
}

Result GetState(uint8 *state)
{
	Result result = PreRPC();
	if (result) result = RPCGetState(state);
	return PostRPC(result);
}

Result RPCDetach(uint16 timeout)
{
	// Prepare the setup data
	struct Setup setup;
	setup.bmRequestTypeRecipient = recipient_interface;
	setup.bmRequestTypeType = type_class;
	setup.bmRequestTypeDirection = dir_host_to_device;
	setup.bRequest = request_detach;
	setup.wValue = timeout;
	setup.wIndex = dfuInterfaceDescriptor.bInterfaceNumber;
	setup.wLength = 0;

	// Perform the request
	return ControlRequest(setup, null, 0, null);
}

// Individual USB DFU control requests
Result Detach(uint16 timeout)
{
	Result result = PreRPC();
	printf("##########Detach############ \n");
	sendpdu_flag = 1;
	if (result) result = RPCDetach(timeout);
	sendpdu_flag = 0;
	return PostRPC(result);
}

// Check whether actions are appropriate to the current state
bool CanAbort(uint8 state)
{
	return (state == dfu_dnload_sync)
	       || (state == dfu_manifest_sync);
}

bool CanClrStatus(uint8 state)
{
	return state == dfu_error;
}

Result Abort(void)
{
	Result result = PreRPC();
	if (result) result = RPCAbort();
	return PostRPC(result);
}
