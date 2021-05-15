#ifndef __BALL_H__
#define __BALL_H__
#include <math.h>
#include <GL/glut.h>
#include "simobj.h"

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


class Ball:public SimObj
{

 
 public:
	Ball();
	Ball(float , float); //x, y
	Ball(float , float, float, float); //x, y, vx, vy
	int Draw();

};

#endif 	