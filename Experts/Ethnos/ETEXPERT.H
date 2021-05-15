#ifndef BBBBBBBBBB
#define BBBBBBBBBB TRUE

#include "Bigkernel.h"
#include "ETDispatch.h" 
#include <stdlib.h>

#define null NULL
#define ETPRIO_MIN 0

class ETExpert
{
 public:
  long periodo;
  ETDispatch* ker;
  unsigned short minonna;
  virtual void 	Init() {}
  virtual void 	Close() {}
  virtual void 	DoYourDuty(int wc=0) = 0;
  unsigned short ActivationCondition;	    
  ETMessage* mymsg[200];
  int primo;
  int secondo;
  ETMessage* cancellandum; 

  ETExpert(ETDispatch* dis,long period = 0,int priority =0,int
	     flags  =0, int type = 0) 
    {
      ker=dis;
      periodo=period;
      ActivationCondition=0;
      primo=secondo=0;
    }  
  virtual ~ETExpert (){}
  long GetPeriod() {return periodo;} 
  short GetAllMsgTypeMask() {return 1;}
  void SetActivationCondition(short a) {ActivationCondition=minonna;}
  void AddLocalRequest(unsigned short type) {minonna=type;}
  void AddRequest(unsigned short type) {ker->AddRequest(type);}
  void RemoveCurrentMsg() 
    { 
    
    }

  short MsgToBeRead()
    {
      return(primo>0);
    }    
  const ETMessage* GetNextMsg()
    {
      if (!MsgToBeRead()) return null;
      ETMessage* tmp;
      tmp=mymsg[primo];
      mymsg[primo]=null; 
//      cout<<"lettura"<<primo<<endl;
      if (primo>0) primo--;
      if(cancellandum!=null) delete(cancellandum); 
      cancellandum=tmp; 
      return tmp;
    }     
  unsigned short GetActivationCondition() {return ActivationCondition;}
  unsigned short GetRequestedMsg() {return minonna;}
  void SetPrimo(ETMessage* xxx)
{
// cout<<"Messaggio"<<primo<<endl;
 primo++;
 if (primo>100)
 	cout<<"Un esperto ha 100 messaggi in coda!!!Non e' che sta succedendo un porcaio???"
     	<<endl;mymsg[primo]=xxx;

}   
 void ShareMsg(ETMessage*msg,char prio)     
{       ker->AddMsg(msg); // AddMsg da definire in ETDispatcher     
}  

}; 
#endif









