#ifndef _GVISION_H_
#define _GVISION_H_
#include "Gconst.h"
#include "GControl.h"
#include "GMemHandler.h"
#include "ettimer.h"

// *************************************************
// ************    TUTTI I DEFINE   ****************
// *************************************************
#ifndef MAXROBOTSEEN
#define MAXROBOTSEEN MAXOBJSEEN
#endif
// *************************************************
// ***************    FINE DEFINE   ****************
// *************************************************

struct BallPos
{
	float x;
	float y;
};	

class GVision : public ETExpert {

 private:  
  GControl *myGolem;
  int myId;
  ETTimer *myTimer;
  GMemHandler *myMem;
  BallPos ball;
  int num_robot;
  Balloon_struct bs;
  Walls_struct ws;
  All_Robots_struct rs;
  Goals_struct gs;

  int ball_area_handle;
  int wall_area_handle;
  int obstacle_area_handle;
  int goals_area_handle;

 public:
  GVision(ETDispatch*, GControl*, ETTimer*, GMemHandler*, int, int, int, int);
  ~GVision() {
  }
  void Init();
  void DoYourDuty(int =0);
  void Close() {}
  void SetNumRobot(int n) {num_robot=n;}
  void TellObjects (Balloon_struct,
                    Walls_struct,
                    All_Robots_struct,
                    Goals_struct,
                    int);

};

#endif


