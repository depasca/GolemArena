#ifndef ETD_S
#define ETD_S TRUE 

#define MaxExpert 100
#define MaxExternMsgType 100

//#include "ETExpert.h"
#include "etmsg.h"

class ETExpert;

 
class ETDispatch
{
  private:
  short myid;
  int idA;
  int idB;
  int idC;  
  int numexpert;
  ETExpert* ExpertVector[MaxExpert];
  short ActivatedExpert[MaxExpert];
  bool Activandum[MaxExpert];
  short ExternMsgType[MaxExternMsgType];
  short MsgRequested[MaxExternMsgType];          
  ETMessage* FromA[MaxExternMsgType];
  ETMessage* FromB[MaxExternMsgType];
  ETMessage* FromC[MaxExternMsgType];
 public:
  ETMessage* FromMe[MaxExternMsgType];
  
  ETDispatch(int id1);
  
  ~ETDispatch();
  
  int AddExpert(ETExpert* pExpert);
  
  void ActivateExpert(short i) ;
  short GetId() ;
  short GetKernelNum();
  void AddMsg(ETMessage* xxx);
   
  void DoYourDuty(int i=0);
  void AddRequest (unsigned short type) ;
 
  int IRequested(short type);
  
  void isRequested(short type);

  void Send(int idx,int n,ETMessage* xxx);
  
  void Asend(int n,ETMessage* xxx);
  void Bsend(int n,ETMessage* xxx);
  void Csend(int n,ETMessage* xxx);
  
  ETMessage* GetMsg(int n);
};

#endif






