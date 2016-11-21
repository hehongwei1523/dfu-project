
//#include "DFURequest.h"

//typedef enum ResultCode Result;
	// Protocols
	enum Protocol
	{
		protocol_unknown,		// Protocol not determined
		protocol_bcsp,			// BCSP with private channels
		protocol_bcsp_tunnel,	// BCSP with HCI tunnelling
		protocol_h4,			// H4 with HCI tunnelling
		protocol_failed			// Previous connection attempts failed
	};
	enum LinkEstablishment
	{
		sync_unknown,			// Link establishment not determined
		sync_enabled,			// Link establishment enabled
		sync_disabled,			// Link establishment disabled
		sync_passive			// Link establishment enabled but passive
	};

	// A request that safely tests private channels
	Result TestPrivateChannel();

	// Automatic protocol detection
	Result AutomaticPassiveBCSP();
	Result AutomaticActiveBCSP(bool link);

	
	// Connection and disconnection
	Result RPCConnect(bool hintDFU);
	Result RPCDisconnect();

	// Non USB DFU control requests (implemented here as USB control requests)
	Result RPCReset();
	Result RPCGetInterfaceDFU(struct InterfaceDescriptor *descriptor);
	Result RPCGetDevice(struct DeviceDescriptor *descriptor);
	Result RPCGetFunct(struct DFUFunctionalDescriptor *descriptor);
	//Result RPCGetString(uint8 index, CStringX &descriptor);
/*
	// Generic control requests
	virtual DFUEngine::Result ControlRequest(const Setup &setup,
	                                         void *buffer = 0,
							                 uint16 bufferLength = 0,
	                                         uint16 *replyLength = 0);

 */      
   


extern Result ControlRequest(const struct Setup setup,
                                                 void *buffer,
                                                 uint16 bufferLength,
                                                 uint16 *replyLength);


extern Result RPCGetInterfaceDFU(struct InterfaceDescriptor *descriptor);

extern Result RPCReset(void);

extern uint8 Rcv_Flag ;

extern Result TestPrivateChannel();



