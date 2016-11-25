
#include "common/types.h"
//#include "DFUEngine.h"
#include "DFURequest.h"


extern  Result DoReconfigure(bool checkUpload ,bool checkDownload ,DFUFile *checkFile);
extern	Result DoUpload(DFUFile file);
extern	Result DoDownload(DFUFile file);
extern	Result DoManifest();
extern	Result DoRecover(const Result failure);

	//Hid operations
extern bool EnableHidMode(bool enable);

	// Composite operations
extern Result DoConnect(bool hintDFU, bool abort );
extern Result DoDFUIdle(bool clear , bool abort );
extern Result DoDownloadStatus(struct DFUStatus *status);

	// Status checking
extern bool IsDFUMode(const struct DeviceDescriptor *dev_descriptor, const struct InterfaceDescriptor *if_descriptor);

extern Result Do_After_Connect(void);

extern Result Progress(int num);


#ifndef min
#define min(a,b)            (((a) < (b)) ? (a) : (b))
#endif


