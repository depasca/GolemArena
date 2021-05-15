#include "GFake.h"

//#define __DEBUG_GFAKE__

typedef GVector vector;


GFake::GFake(ETDispatch *pDis,
             GControl *pGolem,
			 GMemHandler *pMem,
			 GGoalie *gGol,
			 int ba,
			 int ta,
			 int ga):ETExpert(pDis,GPERIODO)
{
	myGolem=pGolem;
	myGoalie=gGol;
	myId=myGolem->GetId();
	myMem=pMem;
	Ball_Area=ba;
	Target_Area=ta;
	Goals_Area=ga;


}

void GFake::Init()
{
#ifdef __DEBUG_GFAKE__
cout << "GFake::Init()" << endl;
fflush(stdout);
#endif
}




void GFake::DoYourDuty(int iST)
{
	if (iST) return;
//  	iST = 0;

#ifdef __DEBUG_GFAKE__
cout << "GFake::DYD Begin" << endl;
fflush(stdout);
#endif
	
	int OKREAD;

//	printf ("Porca Vacca%d\n",Ball_Area);
//	fflush(stdout);

	OKREAD = myMem->ReadMemArea (Ball_Area,&myBall);
	if (OKREAD==-1)
	    {
	    cout << "\nGoToBall: Errore in lettura mem condivisa: Ball_Area";
	    fflush(stdout);
	    return;
	    }          	
	OKREAD = myMem->ReadMemArea (Goals_Area,&myGoals);
	if (OKREAD==-1)
	    {
	    cout << "\nGoToBall: Errore in lettura mem condivisa: Goals_Area";
	    fflush(stdout);
	    return;
	    }
	Walls_struct ws;
	Task_Type tt;
	Status_Type st;

#ifdef __DEBUG_GFAKE__
cout << "GFake::DYD Pre-Update" << endl;
fflush(stdout);
#endif

	All_Robots_struct ars;
	ars.Robot[0].Pos=GVector(3550.0, 0.15);
	ars.Robot[0].Speed=GVector(1000.0, M_PI);
	ars.Robot[0].Type=Enemy;
	ars.Robot[1].Pos=GVector(2500.0, 0.5);
	ars.Robot[1].Speed=GVector(1500.0, 3.5);
	ars.Robot[1].Type=TeamMate;
	ars.Header.Mate_Seen=1;
	ars.Header.Enemy_Seen=1;
	ars.Header.Unknown_Seen=0;
	myGoalie->Update(GVector(0,0), 0.0, myBall, 0, ars, myGoals, ws, tt, st, 4);	

#ifdef __DEBUG_GFAKE__
cout << "GFake::DYD Post-Update" << endl;
fflush(stdout);
#endif

	ETMessage *MsgSent=myGoalie->Run(); 	

#ifdef __DEBUG_GFAKE__
cout << "GFake::DYD Post-Run" << endl;
fflush(stdout);
#endif

	if(MsgSent!=NULL)
	{
//        MsgSent->SetKernelNum(myGolem->GetId());
	ShareMsg(MsgSent,0);	

#ifdef __DEBUG_GFAKE__
cout << "GFake::DYD MsgSent" << endl;
fflush(stdout);
#endif
	
	}

#ifdef __DEBUG_GFAKE__
cout << "GFake::DYD End" << endl;
fflush(stdout);
#endif

}


