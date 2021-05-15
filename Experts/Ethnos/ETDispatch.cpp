#include "ETDispatch.h"
#include "etexpert.h"
#include "etmsg.h"
#include <stdio.h>
 

ETDispatch::ETDispatch(int id1)
    {
      myid=0;
      idA=-1;
      idB=-1;
      idC=-1;  
      numexpert=0; 
      myid=id1;
      printf("\n Dispatch n° %d", myid);
      for(int i=0;i<MaxExpert;i++)
	{
	  ActivatedExpert[i]=0;
	  Activandum[i]=false;
	}
      for(int i=0;i<MaxExternMsgType;i++)
	{
	  ExternMsgType[i]=-1;
	  MsgRequested[i]=-1;
	}
    }

ETDispatch::~ETDispatch(){}
  
int ETDispatch::AddExpert(ETExpert* pExpert)
    {
      ExpertVector[numexpert]=pExpert;
      numexpert++;
      return (numexpert-1);
    }
  
void ETDispatch::ActivateExpert(short i) 
    {
      ActivatedExpert[i]=1;
      ExpertVector[i]->Init();     
    }     

short ETDispatch::GetId() {return myid;}

void ETDispatch::AddMsg(ETMessage* xxx)
    {
      for (int i=0;i<numexpert;i++)
	{
          if ((xxx->ReadType()==ExpertVector[i]->GetActivationCondition())||(xxx->ReadType()==ExpertVector[i]->GetRequestedMsg()))
            {
              ExpertVector[i]->SetPrimo(xxx);
	      Activandum[i]=true;
            } 
	  if (xxx->ReadType()==ExternMsgType[i])
            {
              
	      FromMe[i]=xxx;
            } 
	}
    }

void ETDispatch:: DoYourDuty(int i=0)
    {

//	printf("\nDispatch in funzione\n");	
//	  printf("\nDispatch: io sono il robot %d ", myid);

      for(i=0;i<(numexpert);i++)
        { //printf("\nActivandum[%d]%d\n",i,Activandum[i]);
          //printf("\nEVGAC]%d\t%d\n",ExpertVector[i]->GetActivationCondition(),i);
	  if((ExpertVector[i]->GetActivationCondition()==0)||(Activandum[i]==true))
	    { //printf("\nAttivato[%d]%d\n",i,Activandum[i]);
	      ExpertVector[i]->DoYourDuty();
	      Activandum[i]=false;
	    }
        }
    }   
    
  void ETDispatch::AddRequest (unsigned short type) 
    {
      int i=0;    
      while((MsgRequested[i]>=0)&&(i<MaxExternMsgType)) i++;
      MsgRequested[i]=type;
    } 

  int ETDispatch::IRequested(short type)
    {
      int n=-1;
/*      for(int i=0;i<MaxExternMsgType;i++)
      	if (MsgRequested[i]==type)
      		n=i;   // o n++ ???
*/      return n;
    }
  
  void ETDispatch::isRequested(short type)
    { 
      for(int i=0;i<MaxExternMsgType;i++) if (ExternMsgType[i]<0)
	ExternMsgType[i]=type;         
    }       

  void ETDispatch::Send(int idx,int n,ETMessage* xxx)
    {
      if (idx==idA) Asend( n, xxx);
      else if (idx==idB) Bsend( n, xxx);
      else if (idx==idC) Csend( n, xxx);
      else if (idA==-1) {idA=idx;Asend( n, xxx);}
      else if (idB==-1) {idB=idx;Bsend( n, xxx);}
      else if (idC==-1) {idC=idx;Csend( n, xxx);}
    }
  
  void ETDispatch::Asend(int n,ETMessage* xxx)
    {
      if (FromA[n]!=NULL) delete(FromA[n]);  
      FromA[n]=xxx;
    }
  void ETDispatch::Bsend(int n,ETMessage* xxx)
    {
      if (FromB[n]!=NULL) delete(FromB[n]);
      FromB[n]=xxx;
    }
  void ETDispatch::Csend(int n,ETMessage* xxx)
    {
      if (FromC[n]!=NULL) delete(FromC[n]);
      FromC[n]=xxx;

    }
  
  ETMessage* ETDispatch::GetMsg(int n)
    {
      ETMessage* z=FromMe[n];
      FromMe[n]=NULL; 
      return z;
    } 









