#include <fcntl.h>
#include <stdio.h>
#include <windows.h>

static HANDLE g_hCom;          // Handle serioveho portu

DWORD WINAPI ThreadSendMsg(LPVOID lpParameter);

/******************���߳�*********************/
int main_1(void)
{

	//���ص���ʽ�򿪴���
	//g_hCom = CreateFile(_T("COM6"), GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, 0);
	g_hCom = CreateFile(L"\\\\.\\COM6", GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, FILE_FLAG_OVERLAPPED, NULL);

	if (g_hCom == INVALID_HANDLE_VALUE)
	{
		int a = GetLastError();
		//CString str;
		//str.Format(_T("%d"), a);
		//AfxMessageBox(str);
		printf("%d \n", a);
		return false;
	}

	//���ö���ʱ
	COMMTIMEOUTS timeouts;
	GetCommTimeouts(g_hCom, &timeouts);
	timeouts.ReadIntervalTimeout = 0;
	timeouts.ReadTotalTimeoutMultiplier = 0;
	timeouts.ReadTotalTimeoutConstant = 60000;
	timeouts.WriteTotalTimeoutMultiplier = 0;
	timeouts.WriteTotalTimeoutConstant = 0;
	SetCommTimeouts(g_hCom, &timeouts);

	//���ö�д��������С
	static const int g_nZhenMax = 32768;
	if (!SetupComm(g_hCom, g_nZhenMax, g_nZhenMax))
	{
		//AfxMessageBox(_T("SetupComm() failed"));
		printf("SetupComm() failed \n");
		CloseHandle(g_hCom);
		return false;
	}

	//���ô���������Ϣ
	DCB dcb;
	if (!GetCommState(g_hCom, &dcb))
	{
		//AfxMessageBox(_T("GetCommState() failed"));
		printf("GetCommState() failed \n");
		CloseHandle(g_hCom);
		return false;
	}
	int nBaud = 115200;
	dcb.DCBlength = sizeof(DCB);
	dcb.BaudRate = nBaud;//������Ϊ115200    
	dcb.Parity = 0;//У�鷽ʽΪ��У��
	dcb.ByteSize = 8;//����λΪ8λ
	dcb.StopBits = ONESTOPBIT;//ֹͣλΪ1λ
	if (!SetCommState(g_hCom, &dcb))
	{
		//AfxMessageBox(_T("SetCommState() failed"));
		printf("SetCommState() failed \n");
		CloseHandle(g_hCom);
		return false;
	}

	//��ջ���
	PurgeComm(g_hCom, PURGE_RXCLEAR | PURGE_TXCLEAR);

	//�������
	DWORD dwError;
	COMSTAT cs;
	if (!ClearCommError(g_hCom, &dwError, &cs))
	{
		//AfxMessageBox(_T("ClearCommError() failed"));
		printf("ClearCommError() failed \n");
			CloseHandle(g_hCom);
		return false;
	}

	//���ô��ڼ����¼�
	SetCommMask(g_hCom, EV_RXCHAR);


	HANDLE hThread1 = CreateThread(NULL, 0, ThreadSendMsg, NULL, 0, NULL);

	while (1)
	{
		;
	}

	CloseHandle(hThread1);

	return 0;
}

/******************�����߳�********************/
DWORD WINAPI ThreadSendMsg(LPVOID lpParameter)
{
	while (1)
	{
		OVERLAPPED osWait;
		memset(&osWait, 0, sizeof(OVERLAPPED));
		osWait.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
		DWORD dwEvtMask;

		if (WaitCommEvent(g_hCom, &dwEvtMask, &osWait))
		{
			if (dwEvtMask & EV_RXCHAR)
			{
				DWORD dwError;
				COMSTAT cs;
				if (!ClearCommError(g_hCom, &dwError, &cs))
				{
					//AfxMessageBox(_T("ClearCommError() failed"));
					printf("ClearCommError() failed");
					CloseHandle(g_hCom);
					return false;
				}

				char buf[101] = { 0 };
				DWORD nLenOut = 0;
				DWORD dwTrans;
				OVERLAPPED osRead;
				memset(&osRead, 0, sizeof(OVERLAPPED));
				osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

				BOOL bReadStatus = ReadFile(g_hCom, buf, cs.cbInQue, &nLenOut, &osRead);
				if (!bReadStatus)
				{
					if (GetLastError() == ERROR_IO_PENDING)//�ص��������ڽ��� 
					{
						//GetOverlappedResult(g_hCom,&osRead2,&dwTrans,true);�ж��ص������Ƿ����
						printf(buf);
						//To do
					}
				}
				else//���������
				{
					printf(buf);
					//To do
				}

			}
		}
		else
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				WaitForSingleObject(osWait.hEvent, INFINITE);
				if (dwEvtMask & EV_RXCHAR)
				{
					DWORD dwError;
					COMSTAT cs;
					if (!ClearCommError(g_hCom, &dwError, &cs))
					{
						//AfxMessageBox(_T("ClearCommError() failed"));
						printf("ClearCommError() failed");
						CloseHandle(g_hCom);
						return false;
					}

					char buf[101] = { 0 };
					DWORD nLenOut = 0;
					DWORD dwTrans;
					OVERLAPPED osRead;
					memset(&osRead, 0, sizeof(OVERLAPPED));
					osRead.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

					BOOL bReadStatus = ReadFile(g_hCom, buf, cs.cbInQue, &nLenOut, &osRead);
					if (!bReadStatus)
					{
						if (GetLastError() == ERROR_IO_PENDING)//�ص��������ڽ��� 
						{
							//GetOverlappedResult(g_hCom,&osRead2,&dwTrans,true);�ж��ص������Ƿ����
							
							//To do
						}
					}
					else//���������
					{
						//To do
					}

				}
			}
		}
	}

	return 1;
}
