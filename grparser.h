#ifndef __GRPARSER_H__
#define __GRPARSER_H__

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <malloc.h>
#include <time.h>
#include <iostream.h>
#include <stdio.h>
#include "grconst.h"
#include "Gconst.h"
#include "grobjects.h"

class GRParser
{
	GColor mycolor;
	GColor opcolor;
	GColor mygoalcolor;
	GColor opgoalcolor;
	char* ip;
	int port;
	int id;
	
	GRBall b;
 	GRTarget t;
    GRObject o;
 	GRGoalPost Mp1, Mp2, Op1, Op2;
  	GRWall fw, bw, rw, lw;
        			
  	struct Golem_status sdata;
	struct MsgVector vdata;
    struct MsgQ_struct qdata;
    struct Balloon_struct bdata;
    struct Target_struct tdata;
    struct All_Robots_struct rdata;
    struct Goals_struct gdata;
    struct Walls_struct wdata;		
	
    /* contatori per le draw */
    int BallSeen;
    int VectorSeen;
    int QArrived;
    int TargetSeen;
    int ObjectsSeen;
    int MGSeen;
    int OGSeen;
    int FW_WallSeen;
    int BW_WallSeen;
    int RW_WallSeen;
    int LW_WallSeen;

    /* quanti robots ho visto */
    int HowManyObjects;
    	
  public:
  	
  	GRParser(char*, int, int, GColor, GColor);  	
  	~GRParser(); 	
  	void Init();  	
  	void ListenToRobot(int);
  	void DoYourDuty(void*, int);
  	void UpdateBooleans();
	void SendMsg(MsgStartStop_Type);	
	void DrawBall();
	void DrawVector();	
	void DrawTarget();
	void DrawObjects();
	void DrawMG();
	void DrawOG();
	void DrawFWall();
	void DrawBWall();
	void DrawRWall();
	void DrawLWall();
	float BatteryCharge();
	float GasCharge();
	bool EnginesOn();
	bool NewQ();
	GBehaviour_Type Behaviour();
	Task_Type Task();
	Status_Type Status();	
};

#endif  			
