/////////////////////////WARNING - AUTOGENERATED FILE//////////////////////////
//
//  FILENAME:   gen_evt.cpp
//
//  PURPOSE :   Headers for autogenerated hci event pdus
//
///////////////////////////////////////////////////////////////////////////////

#ifndef __GEN_EVT_H__
#define __GEN_EVT_H__

class HCI_EV_USER_PASSKEY_REQUEST_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_USER_PASSKEY_REQUEST_T_PDU();
    HCI_EV_USER_PASSKEY_REQUEST_T_PDU( const PDU& );
    HCI_EV_USER_PASSKEY_REQUEST_T_PDU( const uint8 * data , uint32 len );
    BluetoothDeviceAddress get_bd_addr () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_KEYPRESS_NOTIFICATION_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_KEYPRESS_NOTIFICATION_T_PDU();
    HCI_EV_KEYPRESS_NOTIFICATION_T_PDU( const PDU& );
    HCI_EV_KEYPRESS_NOTIFICATION_T_PDU( const uint8 * data , uint32 len );
    BluetoothDeviceAddress get_bd_addr () const;
    uint8 get_notification_type () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_DATA_BUFFER_OVERFLOW_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_DATA_BUFFER_OVERFLOW_T_PDU();
    HCI_EV_DATA_BUFFER_OVERFLOW_T_PDU( const PDU& );
    HCI_EV_DATA_BUFFER_OVERFLOW_T_PDU( const uint8 * data , uint32 len );
    uint8 get_link_type () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_ACTIVE_ADDRESS_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_ACTIVE_ADDRESS_T_PDU();
    HCI_EV_ACTIVE_ADDRESS_T_PDU( const PDU& );
    HCI_EV_ACTIVE_ADDRESS_T_PDU( const uint8 * data , uint32 len );
    BluetoothDeviceAddress get_bd_addr_fixed () const;
    BluetoothDeviceAddress get_bd_addr () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_SYNC_CONN_COMPLETE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_SYNC_CONN_COMPLETE_T_PDU();
    HCI_EV_SYNC_CONN_COMPLETE_T_PDU( const PDU& );
    HCI_EV_SYNC_CONN_COMPLETE_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint16 get_handle () const;
    BluetoothDeviceAddress get_bd_addr () const;
    uint8 get_link_type () const;
    uint8 get_tx_interval () const;
    uint8 get_wesco () const;
    uint16 get_rx_packet_length () const;
    uint16 get_tx_packet_length () const;
    uint8 get_air_mode () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_ALIAS_ADDRESS_REQ_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_ALIAS_ADDRESS_REQ_T_PDU();
    HCI_EV_ALIAS_ADDRESS_REQ_T_PDU( const PDU& );
    HCI_EV_ALIAS_ADDRESS_REQ_T_PDU( const uint8 * data , uint32 len );
    BluetoothDeviceAddress get_bd_addr () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_ALLOW_PRIVATE_PAIRING_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_ALLOW_PRIVATE_PAIRING_T_PDU();
    HCI_EV_ALLOW_PRIVATE_PAIRING_T_PDU( const PDU& );
    HCI_EV_ALLOW_PRIVATE_PAIRING_T_PDU( const uint8 * data , uint32 len );
    uint16 get_handle () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_DISCONNECT_COMPLETE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_DISCONNECT_COMPLETE_T_PDU();
    HCI_EV_DISCONNECT_COMPLETE_T_PDU( const PDU& );
    HCI_EV_DISCONNECT_COMPLETE_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint16 get_handle () const;
    uint8 get_reason () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_LST_CHANGE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_LST_CHANGE_T_PDU();
    HCI_EV_LST_CHANGE_T_PDU( const PDU& );
    HCI_EV_LST_CHANGE_T_PDU( const uint8 * data , uint32 len );
    uint16 get_handle () const;
    uint16 get_timeout () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_COMMAND_STATUS_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_COMMAND_STATUS_T_PDU();
    HCI_EV_COMMAND_STATUS_T_PDU( const PDU& );
    HCI_EV_COMMAND_STATUS_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint8 get_num_hci_command_pkts () const;
    uint16 get_op_code () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_ENCRYPTION_KEY_REFRESH_COMPLETE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_ENCRYPTION_KEY_REFRESH_COMPLETE_T_PDU();
    HCI_EV_ENCRYPTION_KEY_REFRESH_COMPLETE_T_PDU( const PDU& );
    HCI_EV_ENCRYPTION_KEY_REFRESH_COMPLETE_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint16 get_handle () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_READ_CLOCK_OFFSET_COMPLETE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_READ_CLOCK_OFFSET_COMPLETE_T_PDU();
    HCI_EV_READ_CLOCK_OFFSET_COMPLETE_T_PDU( const PDU& );
    HCI_EV_READ_CLOCK_OFFSET_COMPLETE_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint16 get_handle () const;
    uint16 get_clock_offset () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_CONN_REQUEST_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_CONN_REQUEST_T_PDU();
    HCI_EV_CONN_REQUEST_T_PDU( const PDU& );
    HCI_EV_CONN_REQUEST_T_PDU( const uint8 * data , uint32 len );
    BluetoothDeviceAddress get_bd_addr () const;
    uint24 get_dev_class () const;
    uint8 get_link_type () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_MODE_CHANGE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_MODE_CHANGE_T_PDU();
    HCI_EV_MODE_CHANGE_T_PDU( const PDU& );
    HCI_EV_MODE_CHANGE_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint16 get_handle () const;
    uint8 get_curr_mode () const;
    uint16 get_interval () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_USER_PASSKEY_NOTIFICATION_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_USER_PASSKEY_NOTIFICATION_T_PDU();
    HCI_EV_USER_PASSKEY_NOTIFICATION_T_PDU( const PDU& );
    HCI_EV_USER_PASSKEY_NOTIFICATION_T_PDU( const uint8 * data , uint32 len );
    BluetoothDeviceAddress get_bd_addr () const;
    uint32 get_passkey () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_FIXED_ADDRESS_ATTEMPT_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_FIXED_ADDRESS_ATTEMPT_T_PDU();
    HCI_EV_FIXED_ADDRESS_ATTEMPT_T_PDU( const PDU& );
    HCI_EV_FIXED_ADDRESS_ATTEMPT_T_PDU( const uint8 * data , uint32 len );
    uint32 get_reserved () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_CONN_COMPLETE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_CONN_COMPLETE_T_PDU();
    HCI_EV_CONN_COMPLETE_T_PDU( const PDU& );
    HCI_EV_CONN_COMPLETE_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint16 get_handle () const;
    BluetoothDeviceAddress get_bd_addr () const;
    uint8 get_link_type () const;
    uint8 get_enc_mode () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_GENERATE_ALIAS_REQ_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_GENERATE_ALIAS_REQ_T_PDU();
    HCI_EV_GENERATE_ALIAS_REQ_T_PDU( const PDU& );
    HCI_EV_GENERATE_ALIAS_REQ_T_PDU( const uint8 * data , uint32 len );
    uint16 get_handle () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_READ_REMOTE_VER_INFO_COMPLETE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_READ_REMOTE_VER_INFO_COMPLETE_T_PDU();
    HCI_EV_READ_REMOTE_VER_INFO_COMPLETE_T_PDU( const PDU& );
    HCI_EV_READ_REMOTE_VER_INFO_COMPLETE_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint16 get_handle () const;
    uint8 get_lmp_version () const;
    uint16 get_manuf_name () const;
    uint16 get_lmp_subversion () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_IO_CAPABILITY_REQUEST_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_IO_CAPABILITY_REQUEST_T_PDU();
    HCI_EV_IO_CAPABILITY_REQUEST_T_PDU( const PDU& );
    HCI_EV_IO_CAPABILITY_REQUEST_T_PDU( const uint8 * data , uint32 len );
    BluetoothDeviceAddress get_bd_addr () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_IO_CAPABILITY_RESPONSE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_IO_CAPABILITY_RESPONSE_T_PDU();
    HCI_EV_IO_CAPABILITY_RESPONSE_T_PDU( const PDU& );
    HCI_EV_IO_CAPABILITY_RESPONSE_T_PDU( const uint8 * data , uint32 len );
    BluetoothDeviceAddress get_bd_addr () const;
    uint8 get_io_capability () const;
    uint8 get_oob_data_present () const;
    uint8 get_authentication_requirements () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_ROLE_CHANGE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_ROLE_CHANGE_T_PDU();
    HCI_EV_ROLE_CHANGE_T_PDU( const PDU& );
    HCI_EV_ROLE_CHANGE_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    BluetoothDeviceAddress get_bd_addr () const;
    uint8 get_new_role () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_LINK_KEY_REQ_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_LINK_KEY_REQ_T_PDU();
    HCI_EV_LINK_KEY_REQ_T_PDU( const PDU& );
    HCI_EV_LINK_KEY_REQ_T_PDU( const uint8 * data , uint32 len );
    BluetoothDeviceAddress get_bd_addr () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_USER_CONFIRMATION_REQUEST_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_USER_CONFIRMATION_REQUEST_T_PDU();
    HCI_EV_USER_CONFIRMATION_REQUEST_T_PDU( const PDU& );
    HCI_EV_USER_CONFIRMATION_REQUEST_T_PDU( const uint8 * data , uint32 len );
    BluetoothDeviceAddress get_bd_addr () const;
    uint32 get_numeric_value () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_FLOW_SPEC_COMPLETE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_FLOW_SPEC_COMPLETE_T_PDU();
    HCI_EV_FLOW_SPEC_COMPLETE_T_PDU( const PDU& );
    HCI_EV_FLOW_SPEC_COMPLETE_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint16 get_handle () const;
    uint8 get_flags () const;
    uint8 get_flow_direction () const;
    uint8 get_service_type () const;
    uint32 get_token_rate () const;
    uint32 get_token_bucket_size () const;
    uint32 get_peak_bandwidth () const;
    uint32 get_access_latency () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_REMOTE_OOB_DATA_REQUEST_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_REMOTE_OOB_DATA_REQUEST_T_PDU();
    HCI_EV_REMOTE_OOB_DATA_REQUEST_T_PDU( const PDU& );
    HCI_EV_REMOTE_OOB_DATA_REQUEST_T_PDU( const uint8 * data , uint32 len );
    BluetoothDeviceAddress get_bd_addr () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_MASTER_LINK_KEY_COMPLETE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_MASTER_LINK_KEY_COMPLETE_T_PDU();
    HCI_EV_MASTER_LINK_KEY_COMPLETE_T_PDU( const PDU& );
    HCI_EV_MASTER_LINK_KEY_COMPLETE_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint16 get_handle () const;
    uint8 get_key_flag () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_SIMPLE_PAIRING_COMPLETE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_SIMPLE_PAIRING_COMPLETE_T_PDU();
    HCI_EV_SIMPLE_PAIRING_COMPLETE_T_PDU( const PDU& );
    HCI_EV_SIMPLE_PAIRING_COMPLETE_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    BluetoothDeviceAddress get_bd_addr () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_FLUSH_OCCURRED_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_FLUSH_OCCURRED_T_PDU();
    HCI_EV_FLUSH_OCCURRED_T_PDU( const PDU& );
    HCI_EV_FLUSH_OCCURRED_T_PDU( const uint8 * data , uint32 len );
    uint16 get_handle () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_PAGE_SCAN_REP_MODE_CHANGE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_PAGE_SCAN_REP_MODE_CHANGE_T_PDU();
    HCI_EV_PAGE_SCAN_REP_MODE_CHANGE_T_PDU( const PDU& );
    HCI_EV_PAGE_SCAN_REP_MODE_CHANGE_T_PDU( const uint8 * data , uint32 len );
    BluetoothDeviceAddress get_bd_addr () const;
    uint8 get_page_scan_rep_mode () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_CONN_PACKET_TYPE_CHANGED_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_CONN_PACKET_TYPE_CHANGED_T_PDU();
    HCI_EV_CONN_PACKET_TYPE_CHANGED_T_PDU( const PDU& );
    HCI_EV_CONN_PACKET_TYPE_CHANGED_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint16 get_handle () const;
    uint16 get_pkt_type () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_HARDWARE_ERROR_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_HARDWARE_ERROR_T_PDU();
    HCI_EV_HARDWARE_ERROR_T_PDU( const PDU& );
    HCI_EV_HARDWARE_ERROR_T_PDU( const uint8 * data , uint32 len );
    uint8 get_hw_error () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_SYNC_CONN_CHANGED_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_SYNC_CONN_CHANGED_T_PDU();
    HCI_EV_SYNC_CONN_CHANGED_T_PDU( const PDU& );
    HCI_EV_SYNC_CONN_CHANGED_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint16 get_handle () const;
    uint8 get_tx_interval () const;
    uint8 get_wesco () const;
    uint16 get_rx_packet_length () const;
    uint16 get_tx_packet_length () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_QOS_VIOLATION_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_QOS_VIOLATION_T_PDU();
    HCI_EV_QOS_VIOLATION_T_PDU( const PDU& );
    HCI_EV_QOS_VIOLATION_T_PDU( const uint8 * data , uint32 len );
    uint16 get_handle () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_MAX_SLOTS_CHANGE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_MAX_SLOTS_CHANGE_T_PDU();
    HCI_EV_MAX_SLOTS_CHANGE_T_PDU( const PDU& );
    HCI_EV_MAX_SLOTS_CHANGE_T_PDU( const uint8 * data , uint32 len );
    uint16 get_handle () const;
    uint8 get_lmp_max_slots () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_ALIAS_ADDRESS_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_ALIAS_ADDRESS_T_PDU();
    HCI_EV_ALIAS_ADDRESS_T_PDU( const PDU& );
    HCI_EV_ALIAS_ADDRESS_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint16 get_handle () const;
    BluetoothDeviceAddress get_bd_addr () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_FIXED_ADDRESS_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_FIXED_ADDRESS_T_PDU();
    HCI_EV_FIXED_ADDRESS_T_PDU( const PDU& );
    HCI_EV_FIXED_ADDRESS_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint16 get_handle () const;
    BluetoothDeviceAddress get_bd_addr () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_CHANGE_CONN_LINK_KEY_COMPLETE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_CHANGE_CONN_LINK_KEY_COMPLETE_T_PDU();
    HCI_EV_CHANGE_CONN_LINK_KEY_COMPLETE_T_PDU( const PDU& );
    HCI_EV_CHANGE_CONN_LINK_KEY_COMPLETE_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint16 get_handle () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_SNIFF_SUB_RATE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_SNIFF_SUB_RATE_T_PDU();
    HCI_EV_SNIFF_SUB_RATE_T_PDU( const PDU& );
    HCI_EV_SNIFF_SUB_RATE_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint16 get_handle () const;
    uint16 get_transmit_latency () const;
    uint16 get_receive_latency () const;
    uint16 get_remote_timeout () const;
    uint16 get_local_timeout () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_INQUIRY_COMPLETE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_INQUIRY_COMPLETE_T_PDU();
    HCI_EV_INQUIRY_COMPLETE_T_PDU( const PDU& );
    HCI_EV_INQUIRY_COMPLETE_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_ALIAS_NOT_RECOGNISED_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_ALIAS_NOT_RECOGNISED_T_PDU();
    HCI_EV_ALIAS_NOT_RECOGNISED_T_PDU( const PDU& );
    HCI_EV_ALIAS_NOT_RECOGNISED_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    BluetoothDeviceAddress get_bd_addr () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_QOS_SETUP_COMPLETE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_QOS_SETUP_COMPLETE_T_PDU();
    HCI_EV_QOS_SETUP_COMPLETE_T_PDU( const PDU& );
    HCI_EV_QOS_SETUP_COMPLETE_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint16 get_handle () const;
    uint8 get_flags () const;
    uint8 get_service_type () const;
    uint32 get_token_rate () const;
    uint32 get_peak_bandwidth () const;
    uint32 get_latency () const;
    uint32 get_delay_variation () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_AUTH_COMPLETE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_AUTH_COMPLETE_T_PDU();
    HCI_EV_AUTH_COMPLETE_T_PDU( const PDU& );
    HCI_EV_AUTH_COMPLETE_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint16 get_handle () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_ENHANCED_FLUSH_COMPLETE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_ENHANCED_FLUSH_COMPLETE_T_PDU();
    HCI_EV_ENHANCED_FLUSH_COMPLETE_T_PDU( const PDU& );
    HCI_EV_ENHANCED_FLUSH_COMPLETE_T_PDU( const uint8 * data , uint32 len );
    uint16 get_handle () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_PAGE_SCAN_MODE_CHANGE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_PAGE_SCAN_MODE_CHANGE_T_PDU();
    HCI_EV_PAGE_SCAN_MODE_CHANGE_T_PDU( const PDU& );
    HCI_EV_PAGE_SCAN_MODE_CHANGE_T_PDU( const uint8 * data , uint32 len );
    BluetoothDeviceAddress get_bd_addr () const;
    uint8 get_page_scan_mode () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_ENCRYPTION_CHANGE_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_ENCRYPTION_CHANGE_T_PDU();
    HCI_EV_ENCRYPTION_CHANGE_T_PDU( const PDU& );
    HCI_EV_ENCRYPTION_CHANGE_T_PDU( const uint8 * data , uint32 len );
    uint8 get_status () const;
    uint16 get_handle () const;
    uint8 get_enc_enable () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

class HCI_EV_PIN_CODE_REQ_T_PDU : public HCIEventPDU
{
public:
    HCI_EV_PIN_CODE_REQ_T_PDU();
    HCI_EV_PIN_CODE_REQ_T_PDU( const PDU& );
    HCI_EV_PIN_CODE_REQ_T_PDU( const uint8 * data , uint32 len );
    BluetoothDeviceAddress get_bd_addr () const;
    virtual bool size_expected() const;// sanity check the number of bytes
};

#endif
