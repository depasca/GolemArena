#include "Bigkernel.h"
#include "ETDispatch.h"
#include "etmsg.h"
#include "etexpert.h"
#include <stdio.h>

  void Bigkernel::InsDispatcher(ETDispatch* a)
  {
      //printf("\nBK : inserisco dispatcher\n");
       Vect[imax]=a;imax++;
  }
  void Bigkernel::CommunicateRequest()
    {
      int i=0;
      while(Vect[i]!=null)
	{
	  for(int a=0;a<MaxType;a++){
          int x=Vect[i]->IRequested(a);
          if (x!=-1) 
            for(int h=0;h<imax;h++)
	      if (h!=i) Vect[h]->isRequested(a);            
          }
	  i++;
        }
    }
  void Bigkernel::CommunicateMsg()
    {

      int i=0;
/*sostituisco
      while(Vect[i]!=null)
*/
	while(i<imax)
//fine sostituisco	
	{
	  for(int a=0;a<MaxType;a++){
          if (Vect[i]->FromMe[a]!=null)
            for(int h=0;h<imax;h++)
              {
               short b=(Vect[h]->IRequested(a));
	       if ((h!=i)&&(b!=-1))
{Vect[i]->Send(Vect[h]->GetId(),b,Vect[i]->GetMsg(a) );}      
                       }           }
	  i++;
        }

    }

void Bigkernel::DoYourDuty(int i)
{
 	switch(i)
 	{
 		case -1:
    	

//			printf("\nBigKernel in funzione\n");

			CommunicateRequest();
//	    	printf("\nBK : CommunicateRequest eseguita\n");
        	CommunicateMsg();
//	  		printf("\nBK : CommunicateMsg eseguita\n");
//	  		printf("\nimax : %d\n",imax);
	    	for(int a=0;a<imax;a++)
	    		if (Vect[a]!=null)
	    		{
					Vect[a]->DoYourDuty();
//					cout<<"va"<<a<<" imax : "<<imax<<endl;
	    		}
//          	else printf("\nBK : Vect[a]==NULL\n");
	  		break;
	  		
         case 0:
			CommunicateRequest();
        	CommunicateMsg();
			if (Vect[0]!=null)
				Vect[0]->DoYourDuty();
	  		break;
	  		
		case 1:
			CommunicateRequest();
        	CommunicateMsg();
			if (Vect[1]!=null)
				Vect[1]->DoYourDuty();
	  		break;
	  		
	  	case 2:
    	
			CommunicateRequest();
        	CommunicateMsg();
			if (Vect[2]!=null)
				Vect[2]->DoYourDuty();
	  		break;	
	  	default:
	  		;
	}
	return;	  		
	  	
}






