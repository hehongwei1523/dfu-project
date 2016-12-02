/*
* LamaProg v 0.1.1
* Communicanion routines v 0.1.0
* Jan Parkman   (parkmaj@users.sourceforge.net)
*/

#pragma warning(disable: 4996)

#include "com.h"
#include <fcntl.h>
#include <stdio.h>
//#include <windows.h>

#define STRICT 1 
#define VERBOSE 1
 HANDLE hCOMHnd;          // Handle serioveho portu

//WCHAR * OpenComName = "\\\\.\\COM14";
//char * OpenComName = "\\\\.\\COM10";

DWORD dwError;

// COM port settings for BCSP
static const int comBCSPDataBits = 8;
static const int comBCSPParity = EVENPARITY;
static const int comBCSPStopBits = ONESTOPBIT;
static const int comBCSPRtsControl = RTS_CONTROL_DISABLE;
static const char comBCSPErrorChar = '\xc0';

// COM port settings
static const int comDataBits = 8;
static const int comStopBits = ONESTOPBIT;
static const int comParity = EVENPARITY;


#define true 1
#define false 0

int BCSP_init()
{
	hCOMHnd = CreateFile(L"\\\\.\\COM6", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);
	// Start a BCSP transport
	if (hCOMHnd == INVALID_HANDLE_VALUE)
	{
		printf("Init failed...\n");
	}
	else
	{
		// Purge any operations in progress
		if (!PurgeComm(hCOMHnd, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR))
		{
			return 0;
		}
		// Configure the COM port for BCSP
		DCB dcb;
		if (!GetCommState(hCOMHnd, &dcb))
		{
			return 0;
		}
		dcb.BaudRate = CBR_115200;
		dcb.fBinary = true;
		dcb.fParity = comBCSPParity != NOPARITY;
		dcb.fOutxCtsFlow = false;
		dcb.fOutxDsrFlow = false;
		dcb.fDtrControl = DTR_CONTROL_ENABLE;
		dcb.fDsrSensitivity = false;
		dcb.fOutX = false;
		dcb.fInX = false;
		dcb.fErrorChar = false;
		dcb.fNull = false;
		dcb.fRtsControl = comBCSPRtsControl;
		dcb.fAbortOnError = false;
		dcb.ByteSize = comBCSPDataBits;
		dcb.Parity = comBCSPParity;
		dcb.StopBits = comBCSPStopBits;
		dcb.ErrorChar = comBCSPErrorChar;
		if (!SetCommState(hCOMHnd, &dcb))
		{
			return 0;
		}

	}
}

// Initialize port
int com_init(char *s)
{
	//BCSP_init();
	//return 0;

	DCB DCBData;
	//char sPortName[512];

	//sprintf(sPortName, "\\\\.\\%s", s);
    //hCOMHnd = CreateFile(OpenComName, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);//FILE_ATTRIBUTE_NORMAL
	hCOMHnd = CreateFile(L"\\\\.\\COM6", GENERIC_READ | GENERIC_WRITE,0, NULL, OPEN_EXISTING, 0, NULL);

	if (hCOMHnd == INVALID_HANDLE_VALUE)
	{
#if VERBOSE
		dwError = GetLastError(); // ������� 
		printf("dwError = %d \n",dwError);
		printf("Init failed...\n");
		exit(-1);
#endif
		return(-1);
	}
	
	//���ö���ʱ
	COMMTIMEOUTS timeouts; //add 2016-11-16
	GetCommTimeouts(hCOMHnd, &timeouts);
	timeouts.ReadIntervalTimeout =  0;
	timeouts.ReadTotalTimeoutMultiplier = 10;
	timeouts.ReadTotalTimeoutConstant = 1;// 500;// 60000;  //2016-11-23 ʱ����С,����Ϊ0
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(hCOMHnd, &timeouts);
	

	// Initialize it: 9600 bauds, 8 bits of data, no parity and 1 stop bit
	ZeroMemory(&DCBData, sizeof(DCBData));
	DCBData.DCBlength = sizeof(DCBData);
	DCBData.fBinary = 1;
	DCBData.fParity = 1;
	DCBData.fOutxCtsFlow = 0;
	DCBData.fOutxDsrFlow = 0;
	DCBData.fDtrControl = DTR_CONTROL_DISABLE;//DTR_CONTROL_ENABLE;
	DCBData.fDsrSensitivity = 0;
	DCBData.fTXContinueOnXoff = 1;
	DCBData.fOutX = 0;
	DCBData.fInX = 0;
	DCBData.fErrorChar = 0;
	DCBData.fNull = 0;
	DCBData.fRtsControl = RTS_CONTROL_DISABLE; //RTS_CONTROL_ENABLE;//
	DCBData.fAbortOnError = 0;

	//DCBData.fParity = comParity != NOPARITY;
   //���������������BuildCommDCB
	DCBData.ByteSize = 8;
	DCBData.StopBits = ONESTOPBIT;
	DCBData.BaudRate = CBR_115200;
	DCBData.Parity = comBCSPParity; //2016-11-15  �мǣ�����ֹͣλ:2
	//BuildCommDCB("115200,N,8,1", &DCBData);  //ע�⣺�����������·���87�Ŵ���

	if (!SetCommState(hCOMHnd, &DCBData))
	{
#if VERBOSE
		dwError = GetLastError(); // ������� 
		printf("dwError = %d \n", dwError);
		printf("Port parameters setting failed...\n");
		exit(-1);
#endif
		return(-1);
	}

	return 0;
}


int com_write(unsigned char *data, DWORD length)
{
	BOOL bRes;
	DWORD dwWritten;

	//char length = strlen(data);
	//printf("length=%d ", length);

	bRes = WriteFile(hCOMHnd, data, (DWORD)length, &dwWritten, NULL);
	//bRes = WriteFile(hCOMHnd, data, (DWORD)(strlen(data)), &dwWritten, NULL);

	if (!bRes)
	{
#if VERBOSE
		dwError = GetLastError(); // ������� 
		printf("dwError = %d \n", dwError);
		printf("Put error...\n");
		exit(-1);
#endif
		return(-1);
	}

	if (dwWritten != length) //strlen(data))
	{
#if VERBOSE
		dwError = GetLastError(); // ������� 
		printf("dwError = %d \n", dwError);
		printf("Put error - buffer full...\n");
		exit(-1);
#endif
		return(-1);
	}
	
	return 0;
}


int com_read(char *data)
{
	int i = 0;
	int ch;

	while (((ch = com_get()) >= 0))
	{
		data[i++] = ch;
		if ((ch == '+') || (ch == '-')) break;
	}
	data[i] = '\0';

	if (ch == '+') return 1;
	if (ch == '-') return 0;
	return -1;
}


int com_put(char data)
{
	BOOL bRes;
	DWORD dwWritten;

	bRes = WriteFile(hCOMHnd, &data, 1, &dwWritten, NULL);

	if (!bRes)
	{
#if VERBOSE
		dwError = GetLastError(); // ������� 
		printf("dwError = %d \n", dwError);
		printf("Put error...\n");
		exit(-1);
#endif
		return(-1);
	}

	if (dwWritten != 1)
	{
#if VERBOSE
		dwError = GetLastError(); // ������� 
		printf("dwError = %d \n", dwError);
		printf("Put error - buffer full...\n");
		exit(-1);
#endif
		return(-1);
	}

	return 0;
}


unsigned char com_get()
{
	BOOL  bRes;
	unsigned char data;// = 0xcc;
	DWORD dwRead;

	BOOL bResult;
	DWORD Event = 0;
	COMSTAT ComStat;        //����״̬ 
	DWORD CommEvent = 0xff;

#if 1
	bRes = ReadFile(hCOMHnd, &data, 1, &dwRead, NULL);
#else
	//��ȡ�����¼����� 
	GetCommMask(hCOMHnd, &CommEvent);
	printf("CommEvent = %d  ", CommEvent);
	if (CommEvent & EV_RXCHAR) //�յ�һ���ַ� 
	{
		bRes = ReadFile(hCOMHnd, &data, 1, &dwRead, NULL);
		printf("data = 0x%2x ", data);	
	}
#endif

	return (unsigned char)data;
	/*
	bResult = WaitCommEvent(hCOMHnd, &Event, FILE_FLAG_OVERLAPPED);

	if (!bResult)
	{
		//���WaitCommEvent()����FALSE,����GetLastError()�ж�ԭ�� 
		switch (dwError = GetLastError())
		{
			case ERROR_IO_PENDING:      //�ص��������ں�̨���� 
			{
				//���������ʱ���ַ�,������������ֵ,���� 
				break;
			}
			case 87:
			{
				//��WIN NT������һ�����ܽ��������û���ҵ� 
				//�����ֵ�ԭ��,��ʲôҲ����,���� 
				break;
			}
			default:
			{
				//������������������ʾ���ڳ���,��ʾ������Ϣ 
				printf("�ȴ������¼�");

			}
		}
	}
	else
	{
		//���WaitCommEvent()����true,������뻺�����Ƿ�ȷʵ 
		//���ֽڿɶ�,��û��,�������һѭ�� 
		//ClearCommError()�����´���״̬�ṹ��������д���Ӳ������ 
		ClearCommError(hCOMHnd, &dwError, &ComStat);
		if (ComStat.cbInQue == 0)   //���뻺����г�Ϊ0,���ַ� 
			;//continue;
	}
	
	switch (Event)
	{
		case 0: //�رմ����¼�,���ȴ��� 
		{
			printf("�رմ���");
		}
		case 1: //�������¼� 
		{
			//��ȡ�����¼����� 
			GetCommMask(hCOMHnd, &CommEvent);
			if (CommEvent & EV_RXCHAR) //�յ�һ���ַ� 
			{
				//ReceiveChar(Port);//����һ���ַ� 
				bRes = ReadFile(hCOMHnd, &data, 1, &dwRead, NULL);
				printf("data = 0x%2x ", data);
			}
			break;
		}
     }
	 */
	//
/*	
	printf("dwRead = 0x%2x ",dwRead);
	if (!bRes)
	{
#if VERBOSE
		dwError = GetLastError(); // ������� 
		printf("dwError = %d \n", dwError);
		printf("Get error...\n");
		exit(-1);
#endif
		return(-1);
	}

	if (dwRead != 1)
	{
#if VERBOSE
		dwError = GetLastError(); // ������� 
		printf("dwError = %d \n", dwError);
		printf("Timeout...\n");
		exit(-1);
#endif
		return(-1);
	}
	
	for (int i = 0; i < 20; i++)
	{
		printf("0x%x ",data[i]);
	}
*/

}


int com_exit()
{
	CloseHandle(hCOMHnd);
	return 0;
}
