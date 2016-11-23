	
#include "DFURequest.h"
#include "stdbool.h"
#include "common/types.h"
#include "DFURequestCOM.h"
 

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
        Result result = RPCConnect(hintDFU);
        if (!result) return result;

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
  /* Ӳ���Ͽ����� */
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
	if (result) result = RPCDnload(blockNum, (uint8 *)buffer, bufferLength);
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
    //ע�����status�Ѿ���ָ�룬���º�������ʱ��������&status����������
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
	if (result) result = RPCReset();
	if (result) result = Disconnect();

	// Allow the device to complete its reset
	//if (result && wait) result = CheckAbort(delayResetMilliseconds);

	// Check the result
	return PostRPC(result);
}

Result GetInterfaceDFU(struct InterfaceDescriptor *descriptor)
{
  /*
	if(connected)  //��ʱ�ó���ֱ�Ӵ�оƬ��ȡ��Ϣ
	{
		descriptor = &dfuInterfaceDescriptor;
		return success;
	}
	else */
	{
		Result result = PreRPC();
		if (result) result = RPCGetInterfaceDFU(descriptor);
		return PostRPC(result);
	}
}

Result GetDevice(struct DeviceDescriptor *descriptor)
{
	Result result = PreRPC();
	if (result) result = RPCGetDevice(descriptor);
	return PostRPC(result);
}

Result GetFunct(struct DFUFunctionalDescriptor *descriptor)
{
	Result result = PreRPC();
	if (result) result = RPCGetFunct(descriptor);
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
  //Result result ;
  /*
	// Special case for vendor-specific failures
	DFUEngine::Result result(DFUEngine::fail_dfu_unknown);
	if (status == err_vendor)
	{
		// Attempt to obtain a description of the error
		CStringX description;
		if (RPCGetString(stringIndex, description) && !description.IsEmpty())
		{
			result = DFUEngine::Result(DFUEngine::fail_dfu_vendor_str, description);
		}
		else
		{
			description.Format(_T("%d"), stringIndex);
			result = DFUEngine::Result(DFUEngine::fail_dfu_vendor_num, description);
		}
	}
	else
	{
		// Attempt to find the appropriate result code
		for (int index = 0;
			 index < (sizeof(statusCodeMap) / sizeof(statusCodeMapEntry));
			 ++index)
		{
			if (statusCodeMap[index].status == status)
			{
				result = statusCodeMap[index].result;
				break;
			}
		}
	}
*/
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
	if (result) result = RPCGetStatus(status);
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
	if (result) result = RPCDetach(timeout);
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