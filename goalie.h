#ifndef __ROBOT_H__
#define __ROBOT_H__

#include <math.h>
#include <GL/glut.h>

#include "simobj.h"

#include "const.h"
#include "ettimer.h"
#include <iostream.h>
#include <etexpert.h>
#include "Golem.h"
#include "GolemMove.h"
#include "ETBat.h"
#include "ETVision.h"
#include "ETMemHandler.h"
#include "ETArbiter.h"
#include "ETMove.h"
#include "ETFindBall.h"
#include "ETSuperCarry.h"
#include "ETGoToBall.h"
#include "ETNearWall.h"
#include "ETAvoidObstacle.h"
#include "ETPosizionati.h"
#include "ETBridge.h"
#include "ETCatchBall.h"
#include "ETRobot3.h"
#include "ETAvoidObstacle1.h"
#include "ETGoToBall1.h"



#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define BALL_LOCK 7

typedef enum{PIONEER, GOLEM}robot_type;



class Robot:public SimObj
{
	int id; 
	robot_type t;
	color c;
	side s;
	Point StartPos, nextP, Target;
	float S,J;		  // speed & jog del pioneer
	bool LOCKWALLX,LOCKWALLY;
	bool LockedX[8];
	bool LockedY[8];
	bool MANUALE; 
	bool DONE;
	Golem *Gol;
	ETPioneer *Pio;
	GolemMove *MoveG;
	ETTimer *pTimer;
    	ETArbiter *pArbiter;
    	ETBat *pBat;
    	ETMemHandler *pMem;
	ETFindBall *pFindBall;
	ETGoToBall *pGoToBall;
	ETMove *Move;
	ETNearWall *pNearWall;
	ETVision *pVision;
	ETSuperCarry *pSCarry;
	ETAvoidObstacle *pAvoid;
	ETAvoidObstacle1 *pAvoid1;
	ETPosizionati *pPos;
	ETCatchBall *pCatch;
	ETRobot3 *pR3;
	
	BallPos ball;
	
 public:
        ETDispatch *Dis; 
	Robot(int);
	~Robot();
	Robot(int,color, side);
	Robot(int,Point);
	Robot(int,color, side, Point); 
	void SetType(robot_type);
	float GetS();
	float GetJ();
    void SetPos(Point);
	void SetVel(float);
	void SetJog(float newJog);
	void SetPeriod(float);
	void SetTarget(Point p);
	bool Done();
	void SetSide(side);
	Point GetNextPos();
	void Man();
	bool IsMan();
	int Draw();	
	void CheckWalls();
	void Update();
	void UpdateGolem();
	void UpdatePioneer();
	bool CollideX();
	bool CollideY();
	void LockX(short);
	void LockY(short);
	void UnLockX(short);
	void UnLockY(short);
	bool IsFreeX();
	bool IsFreeY();
	bool IsFree();
	void Reset();
	void ResetGolem();
	void ResetPioneer();
	void SetBall(BallPos);
	double Dist(double, double, double, double);
	void OpenEyes(MsgVision*);
	BallPos GetBall();

};

#endif // ifndef __IUF_Robot	