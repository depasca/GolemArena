// Esperto per il controllo della memoria codivisa

// Gestisce le CRCWs (Concurrent Reads Concurrent Writes) della memoria
// da parte di piu' esperti che funzionano in parallelo e in modo concorrente.

#include <iostream.h>
#include "GMemHandler.h"
#include <stdio.h>
#include <string.h>

#define PERIODO 20000

GMemHandler::GMemHandler (ETDispatch *pKernel,unsigned int TableSize) :
ETExpert (pKernel,PERIODO) {

  int NumLoc;

  if (TableSize == 0) NumLoc = 1;
  else
    NumLoc = TableSize;

  PointerAllocTable = new char*[TableSize];
  SwapArea = new char*[TableSize];

  NextFreeLoc = -1;
  PATSize = NumLoc;
  MaxCurrentAllocArea = 0;
 
  CRCW = new int[TableSize];
  AreaSize = new unsigned int[TableSize];
  
  for (int i = 0; i < NumLoc; i++) {
    CRCW[i] = 0;
    AreaSize[i] = 0;
  }
  
  //  cout << "Memory Area of Size " << NumLoc << " Allocated" << endl;
  // fflush (stdout);

}




GMemHandler::~GMemHandler() {
  
  delete[] PointerAllocTable; 
  delete[] SwapArea;
}


void GMemHandler::Close () {

  cout << "WAIT...Deallocating Memory Area." << endl;
  fflush (stdout);
}


void GMemHandler::Init() {

  printf ("Memory Handler v1.0......Now Installed\n"); 
  fflush(stdout);
}


void GMemHandler::DoYourDuty(int iST) {


  if (iST == 1) { 
    //cout << "Testing for RM conditions ...." << endl;
    //fflush(stdout);
  }           
  if (iST == 0) {

  switch (Signal.handle) {
    
  case WRITE_MEM : if(UpdateMem (Signal.id)) 
                      Signal.id = OK;
                   break;
 
  //  case DEALLOC_MEM : DeallocMem (Signal.id);
  //                     break;
  
  }
 }
}


int GMemHandler::CreateMemArea (unsigned int Size) {

  char *pMem;

  if (Size == 0) return -1;
  if (++NextFreeLoc > PATSize) return -1;

  pMem = new char[Size];

  PointerAllocTable[NextFreeLoc] = pMem;
  AreaSize[NextFreeLoc] = Size;
  MaxCurrentAllocArea++;

  return NextFreeLoc;
}
  

int GMemHandler::ReadMemArea (int handle,void *pMem) {

  if (handle > MaxCurrentAllocArea) return -1;

  
  

  if (CheckMemoryRW(handle)) {

  LockMem(handle);
    memcpy (pMem,PointerAllocTable[handle],AreaSize[handle]);
    UnlockMem (handle);
    return 1;
  }
  else {

    return 0;
  }
}


int GMemHandler::WriteMemArea (int handle,void *pArea) {

  char *pMem;

  if (handle > MaxCurrentAllocArea) return -1;
  
 
  if (CheckMemoryRW(handle)) {
   LockMem (handle);

    delete PointerAllocTable[handle];
    pMem = new char[AreaSize[handle]];
    for (int i = 0; i < AreaSize[handle]; i++)
      *(pMem+i) = *((char*)pArea+i);
    PointerAllocTable[handle] = pMem;
    UnlockMem(handle);
    Signal.id = CONTINUE;
    // Signal.handle = NO CARE
    return 1;
  }
  else {
    delete SwapArea[handle];
    pMem = new char[AreaSize[handle]];
    for (int i = 0; i < AreaSize[handle];i++)
      *(pMem+i) = *((char*)pArea+i);
    SwapArea[handle] = pMem;
    Signal.id = WRITE_MEM;
    Signal.handle = handle;
    //    delete pMem;
    return 0;
  }
}


int GMemHandler::CheckMemoryRW (int handle) {
 
   return (!CRCW[handle]);
}

void GMemHandler::LockMem (int handle) {

  CRCW[handle] = 1;
}

void GMemHandler::UnlockMem (int handle) {
  
  CRCW[handle] = 0;
}

int GMemHandler::UpdateMem (int handle) {
  
  char *pMem;


  if (CheckMemoryRW(handle)) {

  LockMem(handle);
    delete PointerAllocTable[handle];
    pMem = new char[AreaSize[handle]];
    for (int i = 0; i < AreaSize[handle]; i++)
      *(pMem+i) = *(SwapArea[handle]+i);
    PointerAllocTable[handle] = pMem;
    UnlockMem(handle);
    Signal.id = CONTINUE;
    //Signal.handle = NO CARE
    //delete pMem;
    return 1;     
  }  
  else {

    return 0;
  }
}











