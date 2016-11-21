
#include "stdbool.h"
#include "DFUEngineBase.h"
#include "DFURequestCOM.h"

 
//SmartPtr<DFURequests> transport;

// Maximum detach timeout and retries
static const uint16 detachTimeout = 15000;
static const uint8 detachRetries = 5;

/*
// Delay before start of recovery action
static const uint16 recoverDelay = 1000;

// Download timing to trigger additional description
static const uint32 downloadProgressMilliseconds = 4000;

// Assumed maximum upload size for progress indication
static const uint32 maximumUploadSize = 256 * 2 * 1024;

// Number of milliseconds to use accurate sleep timing for
static const uint32 accurateMilliseconds = 10;
*/


Result Progress(int num)
{
  return success;
}
Result ProgressCheck(int num)
{
  return success;
}

// Implementation of operations
Result DoReconfigure(bool checkUpload ,bool checkDownload ,DFUFile *checkFile)
{ 
   Result result;
   /*
   struct DeviceDescriptor device;
   struct InterfaceDescriptor iface;
   struct DFUFunctionalDescriptor functional;
   struct DFUStatus status;
 */
	// Attempt to start the transport (could be slow for UART)
	//Progress(DFUEngine::reconfigure_connect);
   /* 与芯片建立连接 */
	result = DoConnect(false,false);
	if (!result) return result;

	// Read the device descriptor to determine mode
    Do_After_Connect();

   // Successful if this point reached
	return success;

}

 
Result DoDownload(DFUFile file) //烧录过程
{

	Result result;
	struct DFUFunctionalDescriptor functional;
	struct DFUStatus status;

	struct DFUStatus newStatus;

	// Ensure that the transport has been started
	Progress(download_prepare);
	result = DoConnect(true,false);
	if (!result) return result;

	// Verify the starting state
	result = ProgressCheck(1);
	if (result) result = DoDFUIdle(true, false);
	if (!result) return result;

	// Read maximum transfer size and check download supported
	result = ProgressCheck(2);
	if (result) result = GetFunct(&functional);
	if (!result) return result;
	if (!(functional.bmAttributes
	      & attribute_can_download))
	{
		return fail_no_download;
	}
/* //文件处理
	// Perform the download
	Progress(download_progress, 0, file->GetLength());
	result = transport->PreDnload(file);
	if (!result) return result;
	file->SeekToBegin();
	uint16 blockNum = 0;
	uint16 bufferLength = functional.wTransferSize;
	SmartPtr<uint8, true> buffer(new uint8[bufferLength]);
  
	while (file->GetPosition() != file->GetLength())
	{
		// Update the progress indicator
		int percent = 3 + (92 * file->GetPosition())
		                   / file->GetLength();
		result = ProgressCheck(percent);
		if (!result) return result;

		// Extract the next block from the DFU file
		uint16 next = file->Read(buffer, bufferLength);

		// Check the status
		result = DoDownloadStatus(status);
		if (!result) return result;

		// Download the next block
		result = transport->Dnload(blockNum++, buffer, next);
		if (!result) return result;

		// Update the progress indicator again
		Progress(DFUEngine::download_progress,
		         file->GetPosition(), file->GetLength());

		AccurateSleep(shortTimeout);

		// Check the stutus is OK (use newStatus to ensure we don't overwrite old time)
			
		// Use different status so we don't get next write time!!
		result = DoDownloadStatus(newStatus);
		if (!result) return result;

		// Use new bState!!
		if (newStatus.bState == DFURequests::dfu_dnload_busy)
		{
			// Wait the proper amout of time!!
			if (status.bwPollTimeout < downloadProgressMilliseconds) 
			{
			  // Use original status
			  AccurateSleep(status.bwPollTimeout);
			}
			else 
			{
			  // Use original status
			  AccurateSleep(status.bwPollTimeout, DFUEngine::download_processing);
			}
		}
	}
*/  
    /*  //校验
	// Check the status
	Progress(download_verify);
	result = ProgressCheck(96);
	if (result) result = DoDownloadStatus(status);
	if (!result) return result;

	// End the download
	result = ProgressCheck(97);
	if (result) result = Dnload(blockNum, 0, 0);
	if (!result) return result;

	// Wait for the operation to complete (do not check for abort)
	Progress(98);

	if (status.bwPollTimeout < downloadProgressMilliseconds) AccurateSleep(status.bwPollTimeout);
	else AccurateSleep(status.bwPollTimeout, DFUEngine::download_processing);

	// Check the status if manifestation tolerant
	if (functional.bmAttributes
	    & attribute_manifestation_tolerant)
	{
		result = ProgressCheck(99);
		if (result) result = DoDFUIdle();
		if (!result) return result;
	}
*/
	// Successful if this point reached
	return success;
}

Result DoManifest() //复位芯片，退出DFU模式
{
	Result result;
	struct DeviceDescriptor device;
	struct InterfaceDescriptor iface;
	struct DFUStatus status;

	// Ensure that the transport has been started
	Progress(manifest_start_runtime);
	result = DoConnect(true,false);
	if (!result) return result;

	// May not be manifestation tolerant so do not check status

	// Perform a reset and restart transport
	result = ProgressCheck(10);
	if (result) result = Reset(false);
	if (result) result = DoConnect(false,false);
	if (!result) return result;

	// Read the device descriptor to determine mode
	result = ProgressCheck(30);
	if (result) result = GetDevice(&device);
	if (!result) return result;
	result = GetInterfaceDFU(&iface);
	if (!result) return result;

	// Special case if still in DFU mode
	if (IsDFUMode(&device, &iface))
	{
		// Check the status
		result = ProgressCheck(50);
		if (result) result = GetStatus(&status);
		if (!result) return result;

		// Most likely reason is corrupt firmware
		if (status.bStatus == err_firmware)
		{
			return fail_dfu_firmware;
		}

		// Abort any operation, ignoring result code
		Progress(manifest_retry_runtime);
		result = ProgressCheck(60);
		if (result) result = DoDFUIdle(true, true);
		if (!result) return result;

		// Try to reset the device again and restart the transport
		result = ProgressCheck(70);
		if (result) result = Reset(false);
		if (result) result = DoConnect(false,false);

		// Read the device descriptor again to determine mode
		result = ProgressCheck(80);
		if (result) result = GetDevice(&device);
		if (!result) return result;
		result = GetInterfaceDFU(&iface);
		if (!result) return result;

	}

	// Generate an error if still in DFU mode
	if (IsDFUMode(&device, &iface))
	{
		// Check the status
		Progress(manifest_fail);
		result = ProgressCheck(90);
		if (result) result = GetStatus(&status);
		if (!result) return result;

		// Generate a suitable error
		result = MapDFUStatus(status.bStatus,status.iString);
		if (result) result =  fail_dfu_firmware;
		return result;
	}

	// End with a reset to allow subsequent connections
	result = Reset(false);
	if (!result) result = Disconnect();
	if (!result) return result;

	// Successful if this point reached
	return success;
}

Result DoRecover(const Result failure)
{
	Result result;
	struct DFUStatus status;

	// Delay a little before starting the recovery action
	//Progress(recover_clean_up, failure);
	//Sleep(recoverDelay);

	// Ensure that the transport has been started
	result = DoConnect(false, false);
	if (!result) return result;

	// Abort any DFU operation
	result = GetStatus(&status);
	if (result && CanAbort(status.bState))
	{
		result = Abort();
	}
	if (result && CanClrStatus(status.bState))
	{
		result = ClrStatus();
	}
	if (!result)
	{
		// Disconnect and reconnect the transport if any problem
		//Progress(recover_fail, failure);
		result = Disconnect();
		if (result) result = DoConnect(false, false);
		if (!result) return result;
	}

	// Perform a reset and disconnect the transport
	result = Reset(false);
	if (!result) return result;

	// Assume successful if this point reached
	return success;
}

	//Hid operations
bool EnableHidMode(bool enable)
{
  return success;
}

	// Composite operations
Result DoConnect(bool hintDFU, bool abort )
{
	Result result;

	// Ensure that there is a transport
	//if (!transport) return fail_no_transport;

	// Enable or disable abort event checking as appropriate
	//transport->SetAbort(abort ? &threadAbort : 0);

	// Set object to receive progress notifications
	//transport->SetProgress(this);

	// Ensure that the transport has been started
	result = Connect(hintDFU);
    
	if (!result) return result;

	// Successful if this point reached
	return success;
}

Result DoDFUIdle(bool clear , bool abort )
{
	Result result;
	struct DFUStatus status;

	// Read the status
	result = GetStatus(&status);
	if (!result) return result;

	// Attempt to abort any active operation if enabled
	if (abort && CanAbort(status.bState))
	{
		result = Abort();
		if (result) result = GetStatus(&status);
		if (!result) return result;
	}

	// Attempt to clear any error condition if enabled
	if (clear && CanClrStatus(status.bState))
	{
		result = ClrStatus();
		if (result) result = GetStatus(&status);
		if (!result) return result;
	}

	// Fail if still not idle and error free
	if (status.bStatus != ok)
	{
		return MapDFUStatus(status.bStatus,status.iString);
	}
	if (status.bState != dfu_idle)
	{
		return fail_mode_not_idle;
	}

	// Successful if this point reached
	return success;
}

Result DoDownloadStatus(struct DFUStatus status)
{
	Result result;

	// Read the status
	result = GetStatus(&status);
	if (!result) return result;

	// Special case if busy
	if ((status.bState == dfu_dnload_busy)
		|| (status.bState == dfu_manifest))
	{
		// Wait for the operation to complete (do not check for abort)
		//Sleep(status.bwPollTimeout);

		// Check the status again
		result = GetStatus(&status);
		if (!result) return result;
	}

	// Special case if error
	if ((status.bState == dfu_error)
		&& (status.bStatus != ok))
	{
		return MapDFUStatus(status.bStatus,
			                           status.iString);
	}

	// Ensure that the download is idle
	if ((status.bState != dfu_idle)
		&& (status.bState != dfu_dnload_idle))
	{
		return fail_mode_not_dnload_idle;
	}

	// Successful if this point reached
	return success;
}

	// Status checking
bool IsDFUMode(const struct DeviceDescriptor *device, const struct InterfaceDescriptor *iface)
{
  
	return (device->bDeviceClass == 0 && device->bDeviceSubClass == 0 && device->bDeviceProtocol == 0)
		?
        ((iface->bInterfaceClass == dfuInterfaceClass)
        && (iface->bInterfaceSubClass == dfuInterfaceSubClass)
        && ((iface->bInterfaceProtocol == dfu10InterfaceProtocol) || (iface->bInterfaceProtocol == dfu11DFUInterfaceProtocol)))
		:
        ((device->bDeviceClass == dfu10DeviceClass)
        && (device->bDeviceSubClass == dfu10DeviceSubClass)
        && (device->bDeviceProtocol == dfu10DeviceProtocol));
}

Result Do_After_Connect()
{
   Result result;
   struct DeviceDescriptor device;
   struct InterfaceDescriptor iface;
   struct DFUFunctionalDescriptor functional;
   struct DFUStatus status;  

	// Read the device descriptor to determine mode
	result = ProgressCheck(30);
	if (result) result = GetDevice(&device);
	if (!result) return result;
	result = GetInterfaceDFU(&iface);
	if (!result) return result;
    
	// Special case if still in DFU mode
	if (IsDFUMode(&device, &iface))
	{
        printf(" still in DFU mode  ! \n");
		// Check the status
		result = ProgressCheck(50);
		if (result) result = GetStatus(&status);
		if (!result) return result;

		// Most likely reason is corrupt firmware
		if (status.bStatus == err_firmware)
		{
			return fail_dfu_firmware;
		}

		// Abort any operation, ignoring result code
		Progress(manifest_retry_runtime);
		result = ProgressCheck(35);
		if (result) result = DoDFUIdle(true, true);
		if (!result) return result;

		// Try to reset the device again and restart the transport
		result = ProgressCheck(40);
		if (result) result = Reset(false);
		if (result) result = DoConnect(false,false);

		// Read the device descriptor again to determine mode
		result = ProgressCheck(50);
		if (result) result = GetDevice(&device);
		if (!result) return result;
		result = GetInterfaceDFU(&iface);
		if (!result) return result;
      
	}
    
	// Perform a detach and reset if not in DFU mode
	uint8 retries = 0;
	while (!IsDFUMode(&device, &iface) && (retries < detachRetries))
	{
		// Save the run-time mode details
          //none operation
        printf(" Try to start dfu mode ! \n");
		// Read maximum timeout from the DFU functional descriptor
		result = ProgressCheck(55);
		if (result) result = GetFunct(&functional);
		if (!result) return result;

		// Perform detach, reset, and restart transport
		Progress(reconfigure_start_dfu);
		result = ProgressCheck(60);
		if (result) result = Detach(min(functional.wDetachTimeout, detachTimeout));
		if (result) result = Reset(false);
		if (result) result = DoConnect(true,false);
		if (!result) return result;

		// Read device descriptor again to determine mode
		result = ProgressCheck(70);
		if (result) result = GetDevice(&device);
		if (!result) return result;
		result = GetInterfaceDFU(&iface);
		if (!result) return result;

		// Increase the retry count
		++retries;
	}
    
	// Should be operating in DFU mode, no more retries if not
	if (!IsDFUMode(&device, &iface)) return fail_mode_no_dfu;

	// Check the DFU status
	Progress(reconfigure_checking);
	result = ProgressCheck(80);
	if (result) result = DoDFUIdle(true, true);
	if (result) result = GetStatus(&status);
	if (!result) return result;    
    
    // Check suitability of DFU file if any supplied
      //none operation
    
    // Check support for upload or download if requested
      //none operation
    
	// Successful if this point reached
	return success;
}

