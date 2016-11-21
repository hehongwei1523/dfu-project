///////////////////////////////////////////////////////////////////////

#ifndef __BCSPTransport_H__
#define __BCSPTransport_H__

//#include "transportimplementation.h"
//#include "uart.h"
#include "bcspimplementation.h"
#include "../bcsp/stackconfiguration.h"
#include "pdu.h"
#include "DFURequest.h"




void onPacketToChip ( BCTransferRequest * req, BCTransferStatus status, void * info );
void onPacketFromChip (BCTransferRequest *req , BCTransferStatus status, void *info);
void processDataFromChip ( BCTransferRequest * req, enum bc_channel ch );

#endif // __BCSPTransport_H__

