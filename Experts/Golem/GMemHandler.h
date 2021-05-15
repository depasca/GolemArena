// GMemHandler.h

// INCLUDE FILE

#ifndef __G_MEMHANDLER_H__
#define __G_MEMHANDLER_H__

#include <etexpert.h>
#include "Gconst.h"

#define WRITE_MEM 	21
#define DEALLOC_MEM 	22
#define CONTINUE 	23
#define OK 		24

class GMemHandler : public ETExpert {

   private :

	
  	char* *PointerAllocTable;
        char* *SwapArea;
  	int *CRCW;
  	unsigned int *AreaSize;

  	int NextFreeLoc;
  	unsigned int PATSize;
  	unsigned int MaxCurrentAllocArea;

  	int CheckMemoryRW (int);
  	void LockMem (int);
  	void UnlockMem (int);
	int UpdateMem (int);

	struct {
		int id;
		int handle;
	} Signal;

   public:

  	GMemHandler (ETDispatch*, unsigned int);
  	~GMemHandler ();
        void Init();
	void DoYourDuty (int =0);
	void Close ();

	int CreateMemArea (unsigned int);
  	int WriteMemArea (int,void*);
  	int ReadMemArea (int,void*);

};


#endif
