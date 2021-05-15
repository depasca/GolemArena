#ifndef __KEYS_H__
#define __KEYS_H__

#include "gautils.h"


#define GA_POV_MOVEMENT 0.05
#define GA_POV_ROTATION 0.01*M_PI
#define GA_MAX_BALL_SPEED 2.0
#define GA_STOP_BALL_AUTO false


extern Point pov, focus;
extern Ball *B;
extern Field *F;
extern Team *TeamA, *TeamB;
extern void reshape(int, int);


void key_press_callback(unsigned char key, int xi,int yi)
{
    switch(key)
    {
	case 27: //escape character
	case 'q':
	case 'Q':
	  exit(1);
	  break;
	case '+':
	  field_z += 0.2;
	  reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	  break;
	case '-':
	  field_z -= 0.2;
	  reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	  break;	
	case 'r':
	  if(TEAMA_PRESENT)
	  	TeamA->Reset();
	  if(TEAMB_PRESENT)
	  	TeamB->Reset();
	  B->Reset();
/*	
	  view_rotx=0.0;
	  view_roty=0.0;
	  view_rotz=0.0;
	  p=STARTP;
*/
	  break;
	case ' ':
		if(GA_STATE==MOVE_BALL)
			B->SetV(0.0, 0.0);
		break;	
	case 'a':
		if(GA_VIEW==GA_SUBJECTIVE_VIEW && GA_STATE==GA_WALK)
		{	
			GVector view_vector(focus.x-pov.x, focus.y-pov.y, Cartesian);
			view_vector.Rotate(GA_POV_ROTATION);
			focus.x=pov.x+view_vector.GetX();
			focus.y=pov.y+view_vector.GetY();
	  		reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		}
		break;
	case 'd':
		if(GA_VIEW==GA_SUBJECTIVE_VIEW && GA_STATE==GA_WALK)
		{	
			GVector view_vector(focus.x-pov.x, focus.y-pov.y, Cartesian);
			view_vector.Rotate(-GA_POV_ROTATION);
			focus.x=pov.x+view_vector.GetX();
			focus.y=pov.y+view_vector.GetY();
	  		reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
		}	
		break;
	case 's':
	  if(TEAMA_PRESENT)
	  	TeamA->ChangeSide();
	  if(TEAMB_PRESENT)
	  	TeamB->ChangeSide();
	  break;
	case 't':
	  switch(team_active)
	  {
	  	case TEAMA:
	  		if(TEAMB_PRESENT)
	  			team_active=TEAMB;
	  		break;
	  	case TEAMB:
	  		if(TEAMA_PRESENT)
	  			team_active=TEAMA;
	  		break;	
	  }	
	  break;
	case 'b':
	  BALLFLAG = !BALLFLAG;;
	  break;     	
	case 'F':
	  GA_STATE = ROTATE_FIELD;
	  break;
	case 'R':
	  GA_STATE = MOVE_ROBOT;
	  break;
	case 'e' :
	  BALL_ELASTICITY -= 0.05;
	  printf("\nkeys: Ball Elasticity=%f ", BALL_ELASTICITY);    
	  break;
	case 'E' :
	  BALL_ELASTICITY += 0.05;
	  printf("\nkeys: Ball Elasticity=%f ", BALL_ELASTICITY); 
	  break;
	case '1':
	  if(team_active==TEAMA && TEAMA_PRESENT)
	  {
	  	if(TeamA->Getn()>=1)
	  	r_man = 0;
	  }
	  else
	  	if(team_active==TEAMB && TEAMB_PRESENT)
	  	{
	  		if(TeamB->Getn()>=1)
	  		r_man = 0;
	  	}	
	  break;
	case '2':
	if(team_active==TEAMA && TEAMA_PRESENT)
	  {
	  	if(TeamA->Getn()>=2)
	  	r_man = 1;
	  }
	  else
	  	if(team_active==TEAMB && TEAMB_PRESENT)
	  	{
	  		if(TeamB->Getn()>=2)
	  		r_man = 1;
	  	}	
	  break;
	case '3':
	if(team_active==TEAMA && TEAMA_PRESENT)
	  {
	  	if(TeamA->Getn()>=3)
	  	r_man = 2;
	  }
	  else
	  	if(team_active==TEAMB && TEAMB_PRESENT)
	  	{
	  		if(TeamB->Getn()>=3)
	  		r_man = 2;
	  	}	
	  break;
	case '4':
	if(team_active==TEAMA && TEAMA_PRESENT)
	  {
	  	if(TeamA->Getn()>=4)
	  	r_man = 3;
	  }
	  else
	  	if(team_active==TEAMB && TEAMB_PRESENT)
	  	{
	  		if(TeamB->Getn()>=4)
	  		r_man = 3;
	  	}	
	  break;
	case 'G' :
	  if(TEAMA_PRESENT)
	  	for(int i=0; i<TeamA->Getn();i++)
	      TeamA->RV[i]->SetType(GOLEM);
	  if(TEAMB_PRESENT)
		for(int i=0; i<TeamB->Getn();i++)
	      TeamB->RV[i]->SetType(GOLEM);
	  break;
	case 'B' :
	  GA_STATE = MOVE_BALL;
	  break;
	case 'N' :
	  NEXT_STEP=true;
	  break;
	case 'm' :
		if(team_active==TEAMA && TEAMA_PRESENT)
			TeamA->RV[r_man]->Man();
		else
			if(team_active==TEAMB && TEAMB_PRESENT)
			TeamB->RV[r_man]->Man();
		break;		
	default:
	  return;
    }
	
	glutPostRedisplay();	
}

void key_press_callback2(unsigned char key, int xi,int yi)
{
    switch(key)
    {
		case 27: //escape character
		case 'q':
		case 'Q':
	  		exit(1);
	  		break;
		case '+':
	  		p -= 5.0;
	  		reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	  		break;
		case '-':
	  		p += 5.0;
	  		reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	  		break;
		default:
	  		return;
    }
    glutPostRedisplay();
}

void  SKey(int key, int x,int y)
{
    float dx=focus.x-pov.x;
    float dy=focus.y-pov.y;
    GVector view_vector(dx, dy, Cartesian);
    switch(GA_STATE)
    {
	case MOVE_ROBOT:
	    if(team_active == TEAMA)
	    {	
	        	float fzero=0.0, fmin=-200.0/180.0*M_PI, fmax=200.0/180.0*M_PI;
	        	GVector vxmax(2.0, 0.0, Cartesian);
	        	GVector vxmin(-2.0, 0.0, Cartesian);
	        	GVector vymax(0.0, 2.0, Cartesian);
	        	GVector vymin(0.0, -2.0, Cartesian);
	        	float factual;
	        	GVector vactual;
	        	TeamA->RV[r_man]->GetVel(vactual, factual);
	        	GVector newv;
	        	switch(key)
	        	{
	        		case GLUT_KEY_LEFT:
	        			newv=vactual+vymax;
	        			TeamA->RV[r_man]->SetVel(newv, factual);
	        			break;
	        		case GLUT_KEY_RIGHT:
	        			newv=vactual+vymin;
	        			TeamA->RV[r_man]->SetVel(newv, factual);
	        			break;	
	        		case GLUT_KEY_UP:
	        			newv=vactual+vxmax;
	        			TeamA->RV[r_man]->SetVel(newv, factual);
	        			break;	
	        		case GLUT_KEY_DOWN:
	        			newv=vactual+vxmin;
	        			TeamA->RV[r_man]->SetVel(newv, factual);
	        			break;
	        		case GLUT_KEY_F1:
	        			TeamA->RV[r_man]->SetVel(vactual, fmax);
	        			break;
	        		case GLUT_KEY_F2:
	        			TeamA->RV[r_man]->SetVel(vactual, fmin);
	        			break;
	        }			
	    }
	    else
	    {
				float fzero=0.0, fmin=-200.0/180.0*M_PI, fmax=200.0/180.0*M_PI;
	        	GVector vxmax(2.0, 0.0, Cartesian);
	        	GVector vxmin(-2.0, 0.0, Cartesian);
	        	GVector vymax(0.0, 2.0, Cartesian);
	        	GVector vymin(0.0, -2.0, Cartesian);	
	        	float factual;
	        	GVector vactual;
	        	TeamB->RV[r_man]->GetVel(vactual, factual);
				GVector newv;
				switch(key)
	        	{
	        		case GLUT_KEY_LEFT:
	        			newv=vactual+vymax;
	        			TeamB->RV[r_man]->SetVel(newv, factual);
	        			break;
	        		case GLUT_KEY_RIGHT:
	        			newv=vactual+vymin;
	        			TeamB->RV[r_man]->SetVel(newv, factual);
	        			break;	
	        		case GLUT_KEY_UP:
	        			newv=vactual+vxmax;
	        			TeamB->RV[r_man]->SetVel(newv, factual);
	        			break;	
	        		case GLUT_KEY_DOWN:
	        			newv=vactual+vxmin;
	        			TeamB->RV[r_man]->SetVel(newv, factual);
	        			break;
	        		case GLUT_KEY_F1:
	        			TeamB->RV[r_man]->SetVel(vactual, fmax);
	        			break;
	        		case GLUT_KEY_F2:
	        			TeamB->RV[r_man]->SetVel(vactual, fmin);
	        			break;
	    	}
	    }
	    break;
	
	case ROTATE_FIELD:
	    switch (key)
	    {
    		case GLUT_KEY_UP:
	          view_rotx += 5.0;
                  break;
        	case GLUT_KEY_DOWN:
	          view_rotx -= 5.0;
	          break;
        	case GLUT_KEY_LEFT:
	          view_rotz += 5.0;
	          break;
        	case GLUT_KEY_RIGHT:
	          view_rotz -= 5.0;
	          break;
        	default:
	          return;
    	}
	    break;
	
	case GA_WALK:
		if(GA_VIEW==GA_SUBJECTIVE_VIEW)
		{
	    	switch (key)
	    	{
    			case GLUT_KEY_UP:
	          		pov.x += GA_POV_MOVEMENT*cos(view_vector.GetTheta());
	          		pov.y += GA_POV_MOVEMENT*sin(view_vector.GetTheta());
	          		focus.x =pov.x+view_vector.GetX();
	          		focus.y =pov.y+view_vector.GetY();
                	break;
        		case GLUT_KEY_DOWN:
	          		pov.x -= GA_POV_MOVEMENT*cos(view_vector.GetTheta());
	          		pov.y -= GA_POV_MOVEMENT*sin(view_vector.GetTheta());
	          		focus.x =pov.x+view_vector.GetX();
	          		focus.y =pov.y+view_vector.GetY();
	          		break;
        		case GLUT_KEY_LEFT:
	          		pov.x -= GA_POV_MOVEMENT*cos(view_vector.GetTheta()-M_PI/2.0);
	          		pov.y -= GA_POV_MOVEMENT*sin(view_vector.GetTheta()-M_PI/2.0);
	          		focus.x =pov.x+view_vector.GetX();
	          		focus.y =pov.y+view_vector.GetY();
	          		break;
        		case GLUT_KEY_RIGHT:
	          		pov.x += GA_POV_MOVEMENT*cos(view_vector.GetTheta()-M_PI/2.0);
	          		pov.y += GA_POV_MOVEMENT*sin(view_vector.GetTheta()-M_PI/2.0);
	          		focus.x =pov.x+view_vector.GetX();
	          		focus.y =pov.y+view_vector.GetY();
	          		break;
	        	case GLUT_KEY_PAGE_UP:
	          		focus.x += GA_POV_MOVEMENT*cos(view_vector.GetTheta());
	          		focus.y += GA_POV_MOVEMENT*sin(view_vector.GetTheta());
	        		break;  	
	        	case GLUT_KEY_PAGE_DOWN:
	          		focus.x -= GA_POV_MOVEMENT*cos(view_vector.GetTheta());
	          		focus.y -= GA_POV_MOVEMENT*sin(view_vector.GetTheta());
	        		break;  	
        		default:
	          		return;
    		}
		  	reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
	    }
	    break;
	
	case MOVE_BALL:
	    if(BALL_FREE)
	    {
    		switch(GA_VIEW)
	    	{
	    		case GA_GLOBAL_VIEW:
	    			switch (key)
	    			{
    					case GLUT_KEY_UP:
	          				B->SetVY(2.0);
              				break;
        				case GLUT_KEY_DOWN:
		  	  				B->SetVY(-2.0);
	          				break;
        				case GLUT_KEY_LEFT:
		      				B->SetVX(-2.0);
	          				break;
        				case GLUT_KEY_RIGHT:
		      				B->SetVX(2.0);
	          				break;
	    	    	}
	    			break;
	    		case GA_SUBJECTIVE_VIEW:
	    		   switch (key)
	    			{
    					case GLUT_KEY_UP:
	          				B->SetV(GVector(GA_MAX_BALL_SPEED, view_vector.GetTheta()), 0.0);
              				break;
        				case GLUT_KEY_DOWN:
	          				B->SetV(GVector(GA_MAX_BALL_SPEED, view_vector.GetTheta()+M_PI), 0.0);
	          				break;
        				case GLUT_KEY_LEFT:
	          				B->SetV(GVector(GA_MAX_BALL_SPEED, view_vector.GetTheta()+M_PI/2.0), 0.0);
	          				break;
        				case GLUT_KEY_RIGHT:
	          				B->SetV(GVector(GA_MAX_BALL_SPEED, view_vector.GetTheta()-M_PI/2.0), 0.0);
	          				break;
	    	    	}
	    			break;
	    	}
	    }			
	    break;
    }
	
    glutPostRedisplay();
    	

}




void  SKeyUp(int key,int x,int y)
{
	switch(GA_STATE)
  	{
    	case MOVE_ROBOT:
        	if(team_active == TEAMA)
          	{
          		if(TeamA->RV[r_man]->Type()==PIONEER)
              	    switch (key)
                  	{
                    	case GLUT_KEY_LEFT:  	
                      	case GLUT_KEY_RIGHT:
		    	  	    	TeamA->RV[r_man]->SetJog(0.0);
		    		     	break;
                      	case GLUT_KEY_UP:  	
                      	case GLUT_KEY_DOWN:
      		    	    	TeamA->RV[r_man]->SetVel(0.0);
        	    	     	break;	
              	  	}
              	else
              	{  		
              	  	float fzero=0.0;
              	  	GVector vnull(0.0, 0.0, Cartesian);
	        		float factual;
	        		GVector vactual;
	        		TeamA->RV[r_man]->GetVel(vactual, factual);
              	  	switch(key)
              	  	{
              	  	  	case GLUT_KEY_LEFT:
              	  	  	case GLUT_KEY_RIGHT:
              	  	  	case GLUT_KEY_UP:
              	  	  	case GLUT_KEY_DOWN:
              	  	  		TeamA->RV[r_man]->SetVel(vnull, factual);
              	  	  		break;
              	  	  	case GLUT_KEY_F1:
              	  	  	case GLUT_KEY_F2:
              	  	  		TeamA->RV[r_man]->SetVel(vactual, fzero);
              	  	  		break;
              	  	  	
              	  	}
              	}  	  	
           	}
           	else
           	{
           		if(TeamB->RV[r_man]->Type()==PIONEER)	
                	switch (key)
              		{
                  		case GLUT_KEY_LEFT:  	
                  		case GLUT_KEY_RIGHT:
		    	  			TeamB->RV[r_man]->SetJog(0.0);
		    				break;
                  		case GLUT_KEY_UP:  	
                  		case GLUT_KEY_DOWN:
      		    			TeamB->RV[r_man]->SetVel(0.0);
        	    			break;	
              		}
              	else
              	{
              	  	float fzero=0.0;
              		GVector vnull(0.0, 0.0, Cartesian);
	        		float factual;
	        		GVector vactual;
	        		TeamB->RV[r_man]->GetVel(vactual, factual);
              		switch(key)
              	  	{
              	  	  	case GLUT_KEY_LEFT:
              	  	  	case GLUT_KEY_RIGHT:
              	  	  	case GLUT_KEY_UP:
              	  	  	case GLUT_KEY_DOWN:
              	  	  		TeamB->RV[r_man]->SetVel(vnull, factual);
              	  	  		break;
              	  	  	case GLUT_KEY_F1:
              	  	  	case GLUT_KEY_F2:
              	  	  		TeamB->RV[r_man]->SetVel(vactual, fzero);
              	  	  	    break;
              	  	}
              	} 	
           }
	   break;	  	
      case ROTATE_FIELD:
       	  break;
      case MOVE_BALL:
      	if(GA_STOP_BALL_AUTO)
	    {
	    	switch (key)
	    	{
    			case GLUT_KEY_UP:
        		case GLUT_KEY_DOWN:
			  	  	B->SetV(0.0, 0.0);
	    		    break;
        		case GLUT_KEY_LEFT:
        		case GLUT_KEY_RIGHT:
			      	B->SetV(0.0, 0.0);
			        break;
    	    }
	    }
	    break;
      default:
          return;
  }
}



#endif