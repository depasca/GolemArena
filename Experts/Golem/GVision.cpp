#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "etexpert.h"
#include "etmsg.h"
#include "ettimer.h"
#include "GVision.h"
#include "Gconst.h"
#include <iostream.h>

//#define GDEBUG_VISIONE
//#define GDEBUG_TIME

#define PERIODO 20000

GVision::GVision(ETDispatch *pKernel, GControl *pGolem,
                 ETTimer *pTimer, GMemHandler *pMem,
                 int bah, int wah, int oah, int gah):ETExpert(pKernel,PERIODO)
{
	myGolem=pGolem;
	myId=myGolem->GetId();
	myTimer=pTimer;
	myMem=pMem;
	num_robot=0;
	ball_area_handle=bah;
   	wall_area_handle=wah;
   	obstacle_area_handle=oah;
   	goals_area_handle=gah;
}

void GVision::Init()
{
	myTimer->ResetTimer();
}


void GVision::TellObjects(Balloon_struct _bs,
                          Walls_struct _ws,
                          All_Robots_struct _rs,
                          Goals_struct _gs,
                          int n_rob)
{
	bs=_bs;
	ws=_ws;
	gs=_gs;
	rs=_rs;		
}

void GVision::DoYourDuty(int iST)
{

	if (iST) return;
  	iST = 0;
//    cout<<"Inizio visione"<<endl;
  	double inizio_timer=myTimer->ReadTimer();
#ifdef DEBUG_TIME
  	printf("\nVision : inizio_timer=%f micros\n", inizio_timer);
#endif
    	
    double now_is=myTimer->ReadTimer();
#ifdef DEBUG_TIME
    printf("Vision : l'ora esatta è %f micros\n",now_is);
#endif

//    bs.Pos.SetX (10.00);

//printf("GVISION\n");
//printf("%f",bs.Pos.GetRo());
int write_bs=myMem->WriteMemArea(ball_area_handle, &bs);
int write_ws=myMem->WriteMemArea(wall_area_handle, &ws);
int write_rs=myMem->WriteMemArea(obstacle_area_handle, &rs);
int write_gs=myMem->WriteMemArea(goals_area_handle, &gs);

//printf("\nVision: write_bs=%d, write_ws=%d, write_ohs=%d, write_vr=%d ",
//       write_bs, write_ws, write_ohs, write_rv);

#ifdef DEBUG_VISIONE


#endif


#ifdef DEBUG_TIME
    printf("Vision :  ci ho messo : %f micros\n",now_is-inizio_timer);
#endif
//        cout<<"fine Visione"<<endl;
}


