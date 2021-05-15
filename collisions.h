#ifndef __COLLISIONS_H__
#define __COLLISIONS_H__

#include <GL/glut.h>
#include "ball.h"
#include "GVector.h"
#include "team.h"
#include "grobjects.h"
#include "mainflags.h"

#define LATOR 0.42
//#define DEBUGROBOTVEL
#define USEVECTOR

extern Ball *B;


double Dist (Point p1, Point p2)
{
    double dx = p1.x - p2.x;
    double dy = p1.y - p2.y;
    if(dx==0.0)
	return fabs(dy);
    if(dy==0.0)
	return fabs(dx);
    double r = sqrt( dx*dx + dy*dy);
    return r;
}

int choose_segment(Segment* s, int n)
{
	int ret=0;
	float rif_dist=Dist(B->GetP(), s[ret]);
	for(int i=1; i<n; i++)
	{
		float new_dist=Dist(B->GetP(), s[i]);
		if(new_dist<rif_dist)
		{
			rif_dist=new_dist;
			ret=i;
		}
	}
	return ret;
}

void BPCollide(Point _CollP, Point _RPos, GVector _RVel, float _RJog)
{
	if(DRAWCOLLPOINT==true)
		_CollP.Draw(Gred);

	if(Dist(_CollP, B->GetP())<=0.15)
	{
        /* Velocità del punto di contatto */
		GVector CollP(_CollP.x, _CollP.y, Cartesian);
		GVector RPos(_RPos.x, _RPos.y, Cartesian);
		GVector CollPrelR=CollP-RPos;
		GVector PVelTan(_RJog*CollPrelR.GetRo(), CollPrelR.GetTheta()+(M_PI/2.0), Polar);	
		GVector CollPV=_RVel+PVelTan;
		GVector BallV(B->GetV().x, B->GetV().y, Cartesian);
		GVector CollPVrelB=CollPV-BallV;
		if(DRAWVECTORS==true)
			DrawVector(CollP, CollPV);


#ifdef USEVECTOR		
        /* Velocità della palla */		
		B->SetV(1.2*CollPVrelB, 0.0);
/*		
		Segment ortho_seg(B->GetP(), _CollP);
		float impact_angle=ortho_seg.GetTheta()-BallV.GetTheta();
		if(impact_angle<M_PI/2.0)
		{
			GVector vortho_after(BallV.GetRo(),impact_angle+M_PI);
			GVector vparallel(BallV.GetRo(),ortho_seg.GetTheta()+M_PI/2.0);
			GVector BallV_after=BALL_ELASTICITY*vortho_after+vparallel;
			if(DRAWVECTORS==true)
			{
				DrawVector(_CollP, vortho_after, Gyellow );
				DrawVector(_CollP, vparallel, Gcyan);
				DrawVector(_CollP, BallV_after, Gblack);
			}
			GVector resultantBV=CollPV+BallV_after;			
			B->SetV(BALL_ELASTICITY*resultantBV.GetX(),BALL_ELASTICITY*resultantBV.GetY());
		}
		else
		{
			GVector resultantBV=CollPV+BallV;
			B->SetV(BALL_ELASTICITY*resultantBV.GetX(),BALL_ELASTICITY*resultantBV.GetY());
		}	
*/
#else
		float dx = B->GetP().x - _CollP.x;
		float dy = B->GetP().y - _CollP.y;
		float dsx = B->GetV().x - CollPV.GetX();
		float dsy = B->GetV().y - CollPV.GetY();
		float ang_crash = atan2(dsy,dsx);
		float ang_diff = atan2(dy,dx);
		float ang_rel = ang_diff-_CollP.h;
		if(fabs(ang_crash-ang_diff) < 90.0)
		{
			if(dsx<0.0 && dx>0.0 || dsx>0.0 && dx<0.0 )
		   	 	B->SetV(Point(-dsx - BALL_ELASTICITY*B->GetV().x,B->GetV().y));
			if(dsy<0.0 && dy>0.00 || dsy>0.0 && dy<0.0 )
	   		 	B->SetV(Point(B->GetV().x, -dsy - BALL_ELASTICITY*B->GetV().y));
	    		//A->RV[i]->LockY(ball);    	
		}
#endif
	
	}	
}


void RBCollide(Team* T)
{
	bool THERSACOLLISION = false;
    Point BPos=B->GetP();
//    printf("%s%f\t%f\n","Ball Pos : ", B->GetP().x, B->GetP().y);
    short n = T->Getn();
    for(short i=0; i<n; i++)
    {
        Point RPos = T->RV[i]->GetNextPos();

#ifdef DEBUGROBOTVEL
        float RJog1;
        GVector RVel1;
        T->RV[i]->GetVelGlobal(RVel1, RJog1);	
        RJog1 *= M_PI/180.0;
        if(DRAWVECTORS==true)
        		DrawVector(RPos, RVel1);
#endif    	
    	
    	if(Dist (BPos, RPos)<0.5)
    	{
    		float RJog;
			GVector RVel;
			T->RV[i]->GetVelGlobal(RVel, RJog);	
        	RJog *= M_PI/180.0;
        	float KickVertDist=Dist(Point(0.22, 0.24), Point(0.0, 0.0));
        	float KickAngle=atan2(0.24, 0.22);
        	Point P0(RPos.x+LATOR/3.0*cos(RPos.h*M_PI/180.0),
            	       RPos.y+LATOR/3.0*sin(RPos.h*M_PI/180.0));
        	Point PVSX(RPos.x+LATOR*2.0/3.0*cos((RPos.h+60.0)*M_PI/180.0),
                       RPos.y+LATOR*2.0/3.0*sin((RPos.h+60.0)*M_PI/180.0));
        	Point PVDX(RPos.x+LATOR*2.0/3.0*cos((RPos.h-60.0)*M_PI/180.0),
            	       RPos.y+LATOR*2.0/3.0*sin((RPos.h-60.0)*M_PI/180.0));
        	Point PKSX(RPos.x+KickVertDist*cos(RPos.h*M_PI/180.0+KickAngle),
             	       RPos.y+KickVertDist*sin(RPos.h*M_PI/180.0+KickAngle));
        	Point PKDX(RPos.x+KickVertDist*cos(RPos.h*M_PI/180.0-KickAngle),
                 	   RPos.y+KickVertDist*sin(RPos.h*M_PI/180.0-KickAngle));
    		Point PBACK(RPos.x+LATOR*2.0/3.0*cos((RPos.h+180.0)*M_PI/180.0),
            	    	RPos.y+LATOR*2.0/3.0*sin((RPos.h+180.0)*M_PI/180.0));	
            		
        	Segment KickSX_seg(P0, PKSX);
        	Segment KickDX_seg(P0, PKDX);
      	    Segment BackSX_seg(PBACK, PVSX);
        	Segment BackDX_seg(PBACK, PVDX);
        	int n_seg=4;
        	Segment seg_vect[]={KickSX_seg, KickDX_seg, BackSX_seg, BackDX_seg};
        	int chosen_seg=choose_segment(seg_vect, n_seg);
			Line segment_line(seg_vect[chosen_seg]);
			Line collision_line=segment_line.ortho(BPos);
			if(DRAWLINES==true)
			{
				segment_line.Draw(Gmagenta);
				collision_line.Draw(Gmagenta);
			}
			
			/* punto intersezione fra la retta per il segmento e la retta
			ortogonale al segmento e passante per il centro della palla */
			Point cross_point=interception(segment_line, collision_line);
			
			/* punti estremi del segmento */
			Point vertex1=seg_vect[chosen_seg].GetP1();
			Point vertex2=seg_vect[chosen_seg].GetP2();	
			
			/* il punto di collisione sarà il più vicino alla
			palla fra i due estremi del segmento e il punto di
			intersezione fra le rette viste prima */
			Point collision_point=vertex1;
			if(Dist(BPos, collision_point) > Dist(BPos, vertex2))
				collision_point=vertex2;
			if(Dist(BPos, collision_point) > Dist(BPos, cross_point))
			
			    /* controlla che il punto intersezione
			    non stia "dentro" la palla oppure appartenga al segmento */
				if((Dist(BPos, cross_point) >= 0.15) ||
				  point_on_segment(cross_point, seg_vect[chosen_seg]))
					collision_point=cross_point;
					
			if(DRAWCOLLPOINT==true)
				collision_point.Draw(Gred);		
				
			/* controlla che il punto di collisione appartenga al segmento */
			if(point_on_segment(collision_point, seg_vect[chosen_seg]))

            /* chiama la funzione che calcola le velocità conseguenti all'urto */
			BPCollide(collision_point, RPos, RVel, RJog);
	
			
/*			if(DRAWVECTORS==true)
            {
				GVector *TraslVel=new GVector(RVel.GetX(), RVel.GetY(),Cartesian);
				GVector *V1=new GVector(KickVelTan+TraslVel->GetRo(), (RPos.h+150.0)*M_PI/180.0, Polar);
				DrawVector(P1, V1);
				GVector *V2=new GVector(PostVelTan+TraslVel->GetRo(), (RPos.h-90.0)*M_PI/180.0, Polar);
				DrawVector(P2, V2);
       			GVector *V3=new GVector(KickVelTan+TraslVel->GetRo(), (RPos.h+30.0)*M_PI/180.0, Polar);
	   			DrawVector(P3, V3);
			}
*/		
		}
    }
    if(THERSACOLLISION)
    {
    	BALL_FREE = false;
	}	
	else
	{
		BALL_FREE = true;
	}	
	
}    	
	
void RRCollide(Team *A, Team *B)
{
    if (A)
    {
	short na = A->Getn();	
	for(int i=0; i<na-1; i++)
	{
	    Point posi = A->RV[i]->GetNextPos();
	    for(int j=i+1; j<na; j++)
	    {
		Point posj = A->RV[j]->GetNextPos();
		float d = Dist(posi, posj);
		if(d < RRCOLLISION_DIST)
		{
		    float dx = A->RV[i]->GetP().x - A->RV[j]->GetP().x;
		    float dy = A->RV[i]->GetP().y - A->RV[j]->GetP().y;
		    float dsx = A->RV[i]->GetV().x - A->RV[j]->GetV().x;
		    float dsy = A->RV[i]->GetV().y - A->RV[j]->GetV().y;
		    float ang_crash = atan2(dsy,dsx);
		    float ang_diff = atan2(dy,dx);
		    if( fabs(ang_crash-ang_diff) < 90.0 )
		    {
			A->RV[j]->LockX(i);
			A->RV[i]->LockY(j);
			A->RV[j]->LockY(i);
		    }
		    else
		    {
	        	A->RV[i]->UnLockX(j);
			A->RV[j]->UnLockX(i);
			A->RV[i]->UnLockY(j);
			A->RV[j]->UnLockY(i);
		    }
	
		}
		else
		{
	            A->RV[i]->UnLockX(j);
		    A->RV[j]->UnLockX(i);
		    A->RV[i]->UnLockY(j);
		    A->RV[j]->UnLockY(i);
		}	
	    }	
	
	}

    } 	

    if (B)
    {
	short nb = B->Getn();	
	for(int i=0; i<nb-1; i++)
	{
	    Point posi = B->RV[i]->GetNextPos();
	    for(int j=i+1; j<nb; j++)
	    {
		Point posj = B->RV[j]->GetNextPos();
		float d = Dist(posi, posj);
		if(d < RRCOLLISION_DIST)
		{
		    float dx = B->RV[i]->GetP().x - B->RV[j]->GetP().x;
		    float dy = B->RV[i]->GetP().y - B->RV[j]->GetP().y;
		    float dsx = B->RV[i]->GetV().x - B->RV[j]->GetV().x;
		    float dsy = B->RV[i]->GetV().y - B->RV[j]->GetV().y;
		    float ang_crash = atan2(dsy,dsx);
		    float ang_diff = atan2(dy,dx);
		    if( fabs(ang_crash-ang_diff) < 90.0 )
		    {
			B->RV[i]->LockX(j);
			B->RV[j]->LockX(i);
			B->RV[i]->LockY(j);
			B->RV[j]->LockY(i);
		    }
		    else
		    {
	        	B->RV[i]->UnLockX(j);
			B->RV[j]->UnLockX(i);
			B->RV[i]->UnLockY(j);
			B->RV[j]->UnLockY(i);
		    }
	
		}
		else
		{
	            B->RV[i]->UnLockX(j);
		    B->RV[j]->UnLockX(i);
		    B->RV[i]->UnLockY(j);
		    B->RV[j]->UnLockY(i);
		}	
	    }	
	
	}

    } 	

    if (A && B)
    {
	short na = A->Getn();	
	short nb = B->Getn();		
	for(int i=0; i<na; i++)
	{
	    Point posi = A->RV[i]->GetNextPos();
	    for(int j=0; j<nb; j++)
	    {
		Point posj = B->RV[j]->GetNextPos();
		float d = Dist(posi, posj);
		if(d < RRCOLLISION_DIST)
		{
		    float dx = A->RV[i]->GetP().x - B->RV[j]->GetP().x;
		    float dy = A->RV[i]->GetP().y - B->RV[j]->GetP().y;
		    float dsx = A->RV[i]->GetV().x - B->RV[j]->GetV().x;
		    float dsy = A->RV[i]->GetV().y - B->RV[j]->GetV().y;
		    float ang_crash = atan2(dsy,dsx);
		    float ang_diff = atan2(dy,dx);
		    if( fabs(ang_crash-ang_diff) < 90.0 )
		    {
	        	A->RV[i]->LockX(na+j);
			B->RV[j]->LockX(nb+i);
			A->RV[i]->LockY(na+j);
			B->RV[j]->LockY(nb+i);
		    }
		    else
		    {
	        	A->RV[i]->UnLockX(na+j);
			B->RV[j]->UnLockX(nb+i);
			A->RV[i]->UnLockY(na+j);
			B->RV[j]->UnLockY(nb+i);
		    }
	
		}
		else
		{
	            A->RV[i]->UnLockX(na+j);
		    B->RV[j]->UnLockX(nb+i);
		    A->RV[i]->UnLockY(na+j);
		    B->RV[j]->UnLockY(nb+i);
		}	
	    }	
	
	}

    } 	

}

#endif