#include "GFake.h"




GFake::GFake(ETDispatch *pDis,
					 GControl *pGolem,
					 ETTimer *pTimer,
					 GMemHandler *pMem,
					 GGoalie *gGol,
					 int ba, int ta):ETExpert(pDis,PERIODO)
{
	myGolem=pGolem;
	myGoalie=gGol;
	myId=myGolem->GetId();
	myTimer=pTimer;
	myMem=pMem;
	Ball_Area=ba;
	Target_Area=ta;
   

}

void GFake::Init()
{
	myTimer->ResetTimer();
}




void GFake::DoYourDuty(int iST)
{

	if (iST) return;
  	iST = 0;
	
	int OKREAD;

//	printf ("Porca Vacca%d\n",Ball_Area);
//	fflush(stdout);

	OKREAD = myMem->ReadMemArea (Ball_Area,&myBall);
//	printf ("2");
//	fflush(stdout);
	if (OKREAD==-1)
	    {
	    cout << "\nGoToBall: Errore in lettura mem condivisa: Target";
	    fflush(stdout);
	    return;
	    }
//	printf("\nGFake: vedo la palla in %f ", myBall.Pos.GetRo());
//	fflush(stdout);
	Target_struct ts;
	ts.Pos=myBall.Pos;
	ts.Speed=myBall.Speed;
	ts.Heading=0.0;
	ts.Style=BestGuess;
	ts.Priority=2;
        ts.AvoidBall=false;
	
	int write_ts=myMem->WriteMemArea(Target_Area, &ts);
	
//	printf("\nGFake: ho scritto in memoria il target: write_ts=%d ",write_ts);
//	fflush (stdout);
	ETMessage *MsgSent=new ETMessage(0,GActivateG3dof); 	
    MsgSent->SetKernelNum(myGolem->GetId());
    ShareMsg(MsgSent,MAXMSG);	


}


