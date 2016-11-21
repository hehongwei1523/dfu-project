	enum State
	{
		inactive,					// No connection initialised
		idle,						// No operation being performed
		reconfigure,				// Starting boot loader
		upload,						// Performing upload to host
		download,					// Performing download to device
		manifest					// Restarting normal operation
	};
	enum ResultCode
	{
		success =1,					// Successful completion
		aborted,					// Operation aborted
		fail_unknown,				// Failed for unknown reason
		fail_unimplemented,			// Feature * not implemented
		fail_os,					// Failed due to OS error *
		fail_exception,				// Failed due to exception *
		fail_no_transport,			// No transport configured
		fail_no_upload,				// Upload not supported
		fail_no_download,			// Download not supported
		fail_file_create,			// Failed to create new file *
		fail_file_open,				// Failed to open existing file *
		fail_file_short,			// DFU file * too short
		fail_file_suffix,			// DFU file * invalid suffix
		fail_file_read,				// Failed to read from file *
		fail_file_crc,				// DFU file * CRC incorrect
		fail_file_vendor,			// DFU file * for different vendor
		fail_file_product,			// DFU file * for different product
		fail_reply_short,			// Command reply was too short
		fail_reply_long,			// Command reply was too long
		fail_dfu_target,			// DFU file not for this device
		fail_dfu_file,				// DFU file fails verification
		fail_dfu_write,				// Device unable to write memory
		fail_dfu_erase,				// Memory erase function failed
		fail_dfu_check_erased,		// Memory erase check failed
		fail_dfu_prog,				// Program memory function failed
		fail_dfu_verify,			// Memory failed verification
		fail_dfu_address,			// Address outside valid range
		fail_dfu_not_done,			// Unexpected end of data
		fail_dfu_firmware,			// Firmware corrupt - DFU mode only
		fail_dfu_vendor_num,		// Vendor specific error code *
		fail_dfu_vendor_str,		// Vendor specific error string *
		fail_dfu_usbr,				// Unexpected USB reset signalling
		fail_dfu_por,				// Unexpected power-on reset
		fail_dfu_unknown,			// Unknown DFU failure
		fail_dfu_stalledpkt,		// Unexpected request received
		fail_usb_open,				// Failed to open USB device *
		fail_usb_id,				// Failed to identify device *
		fail_usb_desc_short,		// USB descriptor is too short
		fail_usb_desc_long,			// USB descriptor is too long
		fail_usb_desc_bad,			// USB descriptor is incorrect
		fail_usb_no_dfu,			// DFU not supported
		fail_usb_none,				// No USB transport established
		fail_usb_ps_read,			// Failed to read persistent store
		fail_usb_buffer,			// Unsuitable buffer supplied
		fail_hci_unknown_cmd,		// Unknown HCI command
		fail_hci_no_connection,		// No suitable connection exists
		fail_hci_hardware_fail,		// Could not execute due to HW fail
		fail_hci_page_timeout,		// Device did not respond to page
		fail_hci_auth_fail,			// PIN or link key was wrong
		fail_hci_err_key_missing,	// PIN or link key not supplied
		fail_hci_memory_full,		// Insufficient memory to complete
		fail_hci_connect_timeout,	// Device did not respond to connect
		fail_hci_max_connect,		// Too many connections
		fail_hci_max_sco,			// Too many SCO links
		fail_hci_acl_exists,		// An ACL link already exists
		fail_hci_cmd_disallowed,	// Command not supported now
		fail_hci_host_rej_resource,	// Insufficient resources on host
		fail_hci_host_rej_security,	// Rejected due to security
		fail_hci_host_rej_personal,	// Personal device only
		fail_hci_host_timeout,		// Host did not respond
		fail_hci_unsupported,		// Command not supported by device
		fail_hci_invalid_params,	// Command not according to spec
		fail_hci_term_user,			// Link terminated by used
		fail_hci_term_resource,		// Link terminated due to resources
		fail_hci_term_off,			// Link terminated due to power off
		fail_hci_term_local,		// Link terminated locally
		fail_hci_repeated,			// Too soon after failed auth
		fail_hci_no_pairing,		// Pairing not allowed now
		fail_hci_lmp_unknown,		// Unknown LMP operation
		fail_hci_unsupported_rem,	// Remote device does not support
		fail_hci_sco_offset,		// SCO offset rejected
		fail_hci_sco_interval,		// SCO interval rejected
		fail_hci_sco_air_mode,		// SCO air mode rejected
		fail_hci_lmp_invalid,		// Invalid LMP parameters
		fail_hci_unspecified,		// Other LMP error
		fail_hci_lmp_unsupported,	// LMP operation not supported
		fail_hci_role_change,		// Role change not available
		fail_hci_lmp_timeout,		// LMP timeout occurred
		fail_hci_lmp_error,			// LMP error transaction collision
		fail_hci_lmp_not_allowed,	// LMP PDU not allowed
		fail_hci_unknown,			// Unknown HCI failure
		fail_mode_no_dfu,			// Unable to start DFU mode
		fail_mode_not_idle,			// DFU mode not dfuIDLE
		fail_mode_not_dnload_busy,	// DFU mode not dfuDNBUSY
		fail_mode_not_dnload_idle,	// DFU mode not dfuDNLOAD-IDLE
		fail_mode_not_manifest,		// DFU mode not dfuMANIFEST
		fail_com_open,				// Failed to start transport on *
		fail_com_start,				// Failed to start transport other
		fail_com_connect,			// Failed to establish connection
		fail_com_none,				// No COM transport established
		fail_com_fail,				// COM link failed
		fail_com_timeout,			// General communications timeout
		fail_com_timeout_tx,		// Timeout during transmit
		fail_com_timeout_rx,		// Timeout during receive
		fail_com_short,				// Serial reply was too short
		fail_com_long,				// Serial reply was too long
		fail_com_buffer,			// Unsuitable buffer supplied
		fail_com_mismatched,		// Mismatched reply received
		fail_bccmd_no_such_varid,	// Variable not recognised
		fail_bccmd_too_big,			// Data exceeded capacity
		fail_bccmd_no_value,		// Variable has no value
		fail_bccmd_bad_req,			// GETREQ or SEQREQ had error
		fail_bccmd_no_access,		// Value of variable inaccessible
		fail_bccmd_read_only,		// Value of variable unwritable
		fail_bccmd_write_only,		// Value of variable unreadable
		fail_bccmd_error,			// Other error
		fail_bccmd_permission_denied,// Request not allowed
		fail_bccmd_unknown,			// Unknown BCCMD failure
		fail_btw_error,				// Unknown BTW USB failure
		fail_btw_not_suppored,		// Unsupported BTW operation
		fail_btw_busy,				// BTW device driver is busy
		fail_btw_file,				// DFU file failed BTW checks
		fail_btw_download,			// BTW download failure
		fail_btw_system,			// BTW system failure
		fail_btw_vendor,			// BTW vendor specific failure
		fail_btw_start_dfu,			// BTW driver cannot start DFU
		fail_btw_status_short,		// BTW driver status was too short
		fail_btw_status_long		// BTW driver status was too long
	};
	enum Operation
	{
		ready,						// Device is ready
		reconfigure_connect,		// Attempting initial connection
		reconfigure_start_runtime,	// Already in DFU mode
		reconfigure_start_dfu,		// Starting DFU mode
		reconfigure_checking,		// Checking suitability
		upload_prepare,				// Preparing for upload
		upload_progress,			// * bytes uploaded
		upload_verify,				// Verifying upload complete
		download_prepare,			// Preparing for download
		download_progress,			// * of * bytes downloaded
		download_verify,			// Verifying download complete
		manifest_start_runtime,		// Starting run-time mode
		manifest_retry_runtime,		// Trying again to start run-time
		manifest_fail,				// Determining cause of failure
		recover_clean_up,			// Cleaning up after failure *
		recover_fail				// Failed to clean up after *
	};
	enum SubOperation
	{
		no_sub_operation,			// No sub-operation description
		transport_bcsp_passive_le,	// Passive BCSP at * baud
		transport_bcsp_active_le,	// Active BCSP at * baud
		transport_bcsp_no_le,		// No link BCSP at * buad
		transport_h4,				// H4 at * baud
		transport_usb_wait,			// Waiting for USB device
		transport_usb_open,			// Opening USB device *
		transport_usb_check,		// Check for new USB devices
		transport_established,		// Communications established
		download_processing			// Processing data * seconds
	};
     
 