#ifndef __GFAKE_H__
#define __GFAKE_H__

#include <stdio.h>
#include "etexpert.h"
#include "etmsg.h"
#include "ettimer.h"
#include "Gconst.h"
#include "GControl.h"
#include "GMemHandler.h"

//#define GDEBUG_VISIONE
//#define GDEBUG_TIME


class GFake:public ETExpert
{

	GControl *myGolem;
	int myId;
	ETTimer *myTimer;
	GMemHandler *myMem;
	int Ball_Area;
	int Target_Area;
	Balloon_struct myBall;
	

  public:
  	
  	GFake(ETDispatch*, GControl*, ETTimer*, GMemHandler*, GGoalie*, int, int);	
    ~GFake(){Close();}
    void Close(){};
    void Init();
    void DoYourDuty(int);

};

#endif




