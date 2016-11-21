/****************************************************************************

  Copyright (C) Cambridge Silicon Radio April 2000-2006

FILE
        BTRACE.c  -  debug services

MODIFICATION HISTORY
	1.2  27-apr-00 npm  - added boilerplating

*/


#ifdef	RCS_STRINGS
static  char    btrace_c_id[]
  = "$Id: //depot/bc/bluesuite_2_4/devHost/engine/hcitransport/bcsp/btrace.c#1 $";
#endif


#include "bcspstack.h"
#include "stdarg.h"
#include "btrace.h"

//#ifdef _DEBUG
#if 1

#include <stdio.h>

uint32 BCSPdebugMask = 0xffffffff;
char * BCSPDebugID = null ;
FILE * BCSPDebugLog = null ;

void openDebugLog(char * filename)
{
    BCSPDebugLog = fopen(filename,"w") ;
}
/*
void plainbtrace(uint32 bit,char * fmt,...) 
{
    va_list arglist ;
    if (BCSPdebugMask & bit)
    {
        va_start(arglist,fmt) ;
        vprintf(fmt,arglist) ;
        if (BCSPDebugLog) 
        {
            vfprintf(BCSPDebugLog,fmt,arglist) ;
            fflush(BCSPDebugLog) ;
        }
    }
}


void btrace(uint32 bit,char * fmt,...) 
{
    va_list arglist ;
    if (BCSPdebugMask & bit)
    {
        va_start(arglist,fmt) ;
        if (BCSPDebugID) 
        {
            printf("%s:",BCSPDebugID) ;
            if (BCSPDebugLog) fprintf(BCSPDebugLog,"%s:",BCSPDebugID) ;
        }
        vprintf(fmt,arglist) ;
        if (BCSPDebugLog) 
        {
            vfprintf(BCSPDebugLog,fmt,arglist) ;
            fflush(BCSPDebugLog) ;
        }
    }
}
*/

void plainbtrace(uint32 bit,char * fmt,...) 
{
    va_list arglist ;
    if (BCSPdebugMask & bit)
    {
        va_start(arglist,fmt) ;
        printf(fmt,arglist) ;
        /*
        if (BCSPDebugLog) 
        {
            printf(BCSPDebugLog,fmt,arglist) ;
            fflush(BCSPDebugLog) ;
        }
        */
    }
}


#if 0
void btrace(uint32 bit,char * fmt,...) 
{
    va_list arglist ;
    //if (BCSPdebugMask & bit)
    {
        va_start(arglist,fmt) ;
        printf(fmt,arglist) ;

    }
    
}
#else
void btrace(uint32 bit,char * fmt,...) 
{
  
}
#endif

#endif



