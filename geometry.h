#ifndef __GEOMETRY_H__
#define __GEOMETRY_H__

#include <GL/glut.h>
#include "Gconst.h"
#include "flags.h"


class Point
{

 public:

	double x,y,h; 
	Point();
	Point(double , double );
	Point(double , double , double );
	Point operator =(const Point &p){ x = p.x; y = p.y; h = p.h; return *this;}
	Point operator +(const Point &p){ x+=p.x; y+=p.y; h+=p.h; return *this;}
	Point operator *(const double &d) {x*=d; y*=d; return *this;}
	~Point(){}				
	void Draw(GColor GColor);

};

class Segment
{
	Point P1, P2;
	
  public:

	Segment(Point, Point);
	~Segment(){}
	Segment operator=(const Segment &s){ P1=s.P1; P2=s.P2; return *this;};
	Point GetP1();	
	Point GetP2();	
	float GetRo();	
	float GetTheta();	
	Point middle();
	bool Has_on_it(Point);
	void Draw(GColor =Gblack);
};


class Line
{
	float m;
	float q;
	
  public:

  	Line(float =0.0, float =0.0);
  	Line(Point, Point);
  	Line(Segment S);
  	Line(Segment *S);
  	Line(Point P, GVector *V);
  	~Line(){}
  	Line ortho(Point);
  	Line parallel(Point);
  	float Get_m();  		
  	float Get_q();
  	void Draw(GColor =Gblack);
};


Point interception (Line, Line);
Point interception (Line*, Line*);
			
/*************** funzione appartenenza di un punto a un segmento ****************/
bool point_on_segment(Point, Segment);

/*************** funzione distanza punto-segmento *****************/
float Dist(Point, Segment);




#endif 	