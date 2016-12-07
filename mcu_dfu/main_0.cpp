
#include "bcspimplementation.h"

#include <windows.h>
#include "stdio.h"
#include "scheduler.h"

using namespace std;

extern "C" void Device_Init(void); //C++ ���� C����

//extern "C" void BCSPImplementation_Test(void);
extern "C" void main_run(void);
extern "C" void * Thread2(void *);
extern "C" void * Thread3(void *);
extern "C" void * Thread4(void *);
//extern CRITICAL_SECTION g_cs;
int main()
{
	
	Device_Init();
	::CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Thread2, (LPVOID)0, 0, (LPDWORD)0);  // �����߳�

	::CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Thread3, (LPVOID)0, 0, (LPDWORD)0);  // �����߳�
	
	::CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Thread4, (LPVOID)0, 0, (LPDWORD)0);  // �����߳�
	while (1)  // ������
	{
		main_run();
		//printf("  1  ");
	}
	return 0;
}
