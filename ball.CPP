#include <fstream.h>
#include "ball.h"

#define GA_BALL_FRICTION 0.98

Ball::Ball():SimObj()
{
    D = 0.15; // m
    pdpK = 0.6;
    E = GA_BALL_FRICTION;
    M = 0.3; // Kg 
}
    
Ball::Ball(float x , float y) // starting position
{
    D = 0.15; // m
    pdpK = 0.6;
    E =  GA_BALL_FRICTION;
    M = 0.3; // Kg
    P.x = x;
    P.y = y;
}

Ball::Ball(float x , float y, float vx, float vy)
{
    D = 0.15; // m
    pdpK = 0.6;
    E =  GA_BALL_FRICTION;
    M = 0.3; // Kg    
    P.x = x;
    P.y = y;
    V.x = vx;
    V.y = vy;
}
    
int Ball::Draw() // translation in Main		
{
    glPushMatrix();	
		glColor3f(1.0, 0.0, 0.0);
    	glTranslatef(P.x, P.y, D);
		glutSolidSphere(D, 10, 10);
    glPopMatrix();	
    return 1;       
}    

    


	