#ifndef __GFAKE_H__
#define __GFAKE_H__

#include <stdio.h>
#include "etexpert.h"
#include "etmsg.h"
#include "ettimer.h"
#include "Gconst.h"
#include "GControl.h"
#include "GMemHandler.h"
#include "GGoalie.h"
#include "GBehaviour.h"

//#define GDEBUG_VISIONE
//#define GDEBUG_TIME


class GFake:public ETExpert
{

	GControl *myGolem;
	GGoalie *myGoalie;
	int myId;
	GMemHandler *myMem;
	int Ball_Area;
	int Target_Area;
	int Goals_Area;
	Balloon_struct myBall;
	Goals_struct myGoals;
	

  public:
  	
  	GFake(ETDispatch*, GControl*, GMemHandler*, GGoalie*, int, int, int);	
    ~GFake(){Close();}
    void Close(){};
    void Init();
    void DoYourDuty(int);

};

#endif




