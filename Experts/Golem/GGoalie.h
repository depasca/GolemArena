/***************************************************************************
                          GGoalie.h  -  description
                             -------------------
    begin                : Wed Aug 2 2000
    copyright            : (C) 2000 by Robert Rosati - GolemTeam
    email                : cork@dei.unipd.it
 ***************************************************************************/

#ifndef __GGOALIE_H__
#define __GGOALIE_H__

#include "GBehaviour.h"
#include "Gconst.h"

#define __GGOALIE_SIMULATO__

#ifdef  __GGOALIE_SIMULATO__
#include "grobjects.h"
#endif


#define GGOALIE_ELLIPTIC_RADIUS 4000
#define GGOALIE_KICKDIST	350
#define GGOALIE_INERZIA		15

class GGoalie : public GBehaviour
{
private:

	
	GVector GP1weightnew;
	GVector GP2weightnew;
	GVector GP1weight;
	GVector GP2weight;
	GVector Bweightnew;
	GVector Bweight;
	
    GVector myActualPos;
    float myActualH;
    GVector myLastPos;
    float myLastH;
    GVector myTargetnew;
    GVector myTarget;

    ETMessage *mess;
    Target_struct ts;

    bool ARBITER_POSSO_USCIRE;

public:
    GGoalie(GControl *GGCtrl, GMemHandler *GGMem, unsigned short);
    ~GGoalie();
    ETMessage* Run();
    void GoalieCanGo(bool b=true)
	{
		ARBITER_POSSO_USCIRE=b;
    };
};

#endif