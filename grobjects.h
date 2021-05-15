#ifndef __GROBJECTS_H__
#define __GROBJECTS_H__

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include "grconst.h"
#include "Gconst.h"
#include "geometry.h"
#include "GVector.h"
#include "geometry.h"

void DrawVector(GVector, GVector, float, bool, GColor=Gblack);
void DrawVector(Point, GVector, float, bool, GColor=Gblack);
void DrawVector(GVector, GVector, GColor=Gblack);
void gaDrawVector(GVector, GVector, GColor=Gblack);
void DrawVector(Point, GVector, GColor=Gblack);

class GRCircle
{
	float Radius;
	
  public:
  	GRCircle(float=0.1);
  	~GRCircle() {}
  	void Draw(Point, int, GColor=Gblack, bool=false);
    void Draw(GVector, int, GColor=Gblack, bool=false);
};

class GRBall
{
	GRCircle C;
	
  public:
  	
  	GRBall();
  	~GRBall(){}
  	void Draw(Point, GVector, GColor);
  	void Draw(GVector, GVector, GColor);
};

class GRTarget
{
		
  public:
  	
  	GRTarget();
  	~GRTarget(){}
  	void Draw(Point, GVector);
  	void Draw(GVector, GVector);
};

class GRObject
{
	GRCircle *Cext;
	GRCircle *Cint;
	
  public:
  	
  	GRObject();
  	~GRObject(){}
  	void Draw(Point, GVector, GColor=Gblack);
  	void Draw(GVector, GVector, GColor=Gblack);
};

class GRBackGround
{
	GRCircle C;
	float Radius;
	
  public:
  	
  	GRBackGround(float);
  	~GRBackGround(){}
  	void Draw(Point);
  	void Draw(GVector);
};

class GRWall
{
	Point P1, P2;
		
  public:
  	
  	GRWall();
  	GRWall(Point, Point);
  	GRWall(GVector, GVector);
  	GRWall(GVector);
  	~GRWall(){}
  	void Draw();
};

class GRGoalPost
{
	GRCircle C;
  public:
  	
  	GRGoalPost();
  	~GRGoalPost(){}
  	void Draw(Point, int, GColor);
  	void Draw( GVector, int, GColor);
};
#endif
