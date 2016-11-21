/****************************************************************************

  Copyright (C) Cambridge Silicon Radio Ltd 2000-2009

FILE
    bcspimplementation.h  -  wrapper around bcsp stack

PURPOSE
    A container for the BCSP stack, which keeps tabs on all the data
    and interfaces necessary for it to run.

****************************************************************************/


#ifndef _BCSPIMPLEMENTATION_H_
#define _BCSPIMPLEMENTATION_H_

#include "../bcsp/bcspstack.h"


extern void BCSPImplementation_Init(void);
extern void BCSPImplementation_runStack(void);
extern void BCSPImplementation (void);
extern void BCSPImplementation_Environment (void);
extern void BCSPImplementation_Test (void);


enum { NUM_PACKETS = 10 };

extern	void BCSPImplementation_createBCSPStack(void) ;
	

static void * allocMem ( void * envState , uint32 size ) ;
static void freeMem ( void * envState , void * tofree ) ;

static void enterCS ( void * envState ) ;
static void leaveCS ( void * envState ) ;
static void signal ( void * envState ) ;
static void releasePacket(void *envState, Packet *pkt);

static void (* userOnLinkFailSync)(void*) ;
static void * userOnLinkFailParamSync ;
static void (* userOnLinkFailRetries)(void*) ;
static void * userOnLinkFailParamRetries ;
static void (* userOnLinkFailUart)(void*) ;
static void * userOnLinkFailParamUart ;    
    
typedef struct BCSPImplementation
{

    BCSPStack * mStack ;
    BCSPEnvironment mBCSPenv ;  
  
    bool mXMITBusy ;
    bool mRCVBusy ;
    uint32 mXMITBytesAvailable ;
    uint32 mRCVBytesAvailable ;
    uint8 mXMITBuffer[BYTE_BUFFERSIZE] ;
    uint8 mRCVBuffer[BYTE_BUFFERSIZE] ;
    uint32 mXMITbytesWritten ;
    uint32 mLastXmitTime ;

    bool mStackMustDie ;

    Packet * Packets[NUM_PACKETS];
}_bcspinmlementation;

extern struct BCSPImplementation bcspImplementation;


extern BCSPStack * getBCSPStack(void);

extern int ThreadFunc(void);
extern uint8 test_send[];
//extern void RPCGetDevice_Func(void);
extern void Func_Test(void);
extern void load_the_stack_buffer(void);


//extern void Device_Init(void);
extern uint32 SYSTEM_TIME;

extern char set_event;
//extern void main_run(void);
#define EVENT_BCSP_DATA  0x01
#define EVENT_BCSP_OK   0x80 //(7<<1)

//void * Thread2(void *);
//void * Thread3(void *);
#endif


