#include <GL/glut.h>
#include <stdio.h>

#include <stdlib.h>
#include <math.h>
#include "flags.h"
#include "mainflags.h"
#include "collisions.h"
#include "geometry.h"
#include "ball.h"
#include "field.h"
#include "team.h"
#include "keys.h"
#include "menu.h"
#include "mouse.h"
#include "gjoycontrol.h"
#include "gautils.h"
#include "utility.h"
#include "gatimer.h"

#define TOTAL_DEBUG
#define DEBUG_WORLD false
#define CAMERACAR
#define GA_RADAR_WINDOW true
//#define FULLSCREEN

/****************************************** GOLEM RADAR *************************************/
#include "grobjects.h"
#include "grparser.h"

void reshape2(int, int);
void reshape(int, int);
void DrawBattery();
void DrawGas();
void DrawEngines();

bool GRPAUSE=false;
bool TIME_TO_REDRAW=true;
bool TIME_TO_REDRAW2=true;

float RadarRadius=GRRADAR_RADIUS;



/******************* GLPRINTF **********************/
float x=0.2;//(GLfloat)glutGet(GLUT_WINDOW_WIDTH);
float y=0.075;//(GLfloat)glutGet(GLUT_WINDOW_HEIGHT);
int behave_x=220;
int behave_dx=90;
int behave_y=38;
int status_y=23;
int task_y=8;

/***************************************************/

/* messaggio STATUS */
float BatteryValue=0.0;  /* valoree reala in Volts */
float BatteryCharge=0.0; /* compresa fra 0.0 e 1.0 */
bool EnginesOn=false;
float GasValue=0.0;      /* valoree reala in Volts */
float GasPercentage=0.0; /* compresa fra 0.0 e 1.0 */

/* messaggio Q */
bool NewQ=false;
GBehaviour_Type Behaviour;
Task_Type Task;
Status_Type Status;
/***************************************************/

int mainwin_radar, subwin_radar;

gaTimer *T_radar=NULL;
Point center(0.0, 0.0);
GRBackGround BG(RadarRadius);

void init_radar(void)
{
	T_radar=new gaTimer();
}


void initGL_radar()
{
    GLfloat a=100.0, aw=8000.0,
			d0=600.0, d0w=1000.0,
			p0x=5000.0, p0y=0.0, p0z=1000.0, p0w=1000.0;
			
    glClearColor(0.1, 0.1, 0.1, 0.0);
    glEnable(GL_LIGHTING);
    if(USE_DEPTH_TEST)
    	glEnable(GL_DEPTH_TEST);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_FLAT);
    glEnable(GL_NORMALIZE);
    GLfloat amb0[]={a, a, a, aw};
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
    GLfloat dif0[]={d0, d0, d0, d0w};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dif0);
    GLfloat pos0[]={p0x, p0y, p0z, p0w};
    glLightfv(GL_LIGHT0, GL_POSITION, pos0);


}


void display3(void)
{
  	
  	static int lastdrawtime;
  	int ddt;	
	if(FORCE_REDRAW_PERIOD)
	{
	  /*aggiorno la finestra solo se sono passati almeno
	  MILLI_PERIOD millisecondi dall'aggiornamento precedente */
	  ddt=glutGet(GLUT_ELAPSED_TIME)-lastdrawtime;
	  TIME_TO_REDRAW = ddt>=MILLI_PERIOD ? true:false;
 	}
	
	if(GRPAUSE==false && TIME_TO_REDRAW)
 	{
		
		lastdrawtime=glutGet(GLUT_ELAPSED_TIME);
    	
		if(USE_DEPTH_TEST)
   			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   		else
   			glClear(GL_COLOR_BUFFER_BIT); 	
    	
    	if(USE_DEPTH_TEST)
            {
            	/* scrivo informazioni utili sulla finestra */
                static int ilasttime;
				int dt=glutGet(GLUT_ELAPSED_TIME)-ilasttime;
  	            ilasttime=glutGet(GLUT_ELAPSED_TIME);
				float fps=FPS(dt);
				glColor3f(1.0,1.0,1.0);
				glPrintf(10,10,0,10,"fps %f\n",fps);
   				glPrintf(glutGet(GLUT_WINDOW_WIDTH)-100,10,0,10,
           		 	"time %d:%d\n",(T_radar->ReadTimer()/60), T_radar->ReadTimer()%60);
			}
			
		/* disegno */
    	glPushMatrix();

            BG.Draw(center);
    	
    		float robot_orientation=0.0;
        	Team *T =NULL;
    		bool THERESATEAM=false;
    		if(team_active==TEAMA)
    		{
    			if(TEAMA_PRESENT)
    			{
    				T=TeamA;
    				THERESATEAM=true;
    			}	
    		}
    		else
    		{
    			if(TEAMB_PRESENT)
    			{	
    				T=TeamB;
    				THERESATEAM=true;
    			}
    		}
    		if(THERESATEAM)
    			robot_orientation=T->RV[r_man]->GetP().h;		
        	if(T->RV[r_man]->GetSide()==Gblue)
        		robot_orientation -= 180.0;
        	glRotatef(90.0-robot_orientation, 0.0, 0.0, 1.0);
    		
    		if(THERESATEAM)
    		{		
    			T->RV[r_man]->myParser->DrawBall();
    			T->RV[r_man]->myParser->DrawVector();
    			T->RV[r_man]->myParser->DrawTarget();
    			T->RV[r_man]->myParser->DrawObjects();
    			T->RV[r_man]->myParser->DrawMG();
    			T->RV[r_man]->myParser->DrawOG();
    			T->RV[r_man]->myParser->DrawFWall();
    			T->RV[r_man]->myParser->DrawBWall();
    			T->RV[r_man]->myParser->DrawLWall();
    			T->RV[r_man]->myParser->DrawRWall();
    	    }
    	
    	glPopMatrix();
		
		glutSwapBuffers();

		TIME_TO_REDRAW=false;
	}
	glutPostRedisplay();

//	reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void reshape3(int w, int h)
{
    glViewport(0, 0, w, h-50);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(455.0, (GLfloat)w/(GLfloat)(h-50), 1000.0, RadarRadius+1000.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, RadarRadius, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    TIME_TO_REDRAW=true;

}

void display4(void)
{

 	if(GRPAUSE==false && TIME_TO_REDRAW2)
 	{
 		Team *T =NULL;
    	bool THERESATEAM=false;
    	if(team_active==TEAMA)
    	{
    		if(TEAMA_PRESENT)
    		{
    			T=TeamA;
    			THERESATEAM=true;
    		}	
    	}
    	else
    	{
    		if(TEAMB_PRESENT)
    		{	
    			T=TeamB;
    			THERESATEAM=true;
    		}
    	}
    	if(THERESATEAM)
    	{		
 			BatteryValue=T->RV[r_man]->myParser->BatteryCharge();
   			BatteryCharge=(BatteryValue-BATTERY_CHARGE_MIN)/BATTERY_CHARGE_RANGE;
//			printf("\ndisplay2: BVal=%f, BCarge=%f ",BatteryValue, BatteryCharge);
   			if(BatteryCharge<0.0)
   				BatteryCharge=0.0;
   			if(BatteryCharge>1.0)
   				BatteryCharge=1.0;
   			EnginesOn=T->RV[r_man]->myParser->EnginesOn();
   			GasValue=T->RV[r_man]->myParser->GasCharge();
   			GasPercentage=(GasValue-GAS_CHARGE_MIN)/GAS_CHARGE_RANGE;
//			printf("\ndisplay2: GasVal=%f, GPer=%f ",GasValue, GasPercentage);
    		if(GasPercentage<0.0)
   				GasPercentage=0.0;
   			if(GasPercentage>1.0)
   				GasPercentage=1.0;
   			NewQ=T->RV[r_man]->myParser->NewQ();
//   			printf("\ndisplay2: NewQ=%f ", NewQ);
   			Behaviour=T->RV[r_man]->myParser->Behaviour();
   			Task=T->RV[r_man]->myParser->Task();
   			Status=T->RV[r_man]->myParser->Status();
   		}
    	glClearColor(0.7, 0.7, 0.7, 0.0);
  		if(USE_DEPTH_TEST)
   			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
   		else
   			glClear(GL_COLOR_BUFFER_BIT); 	
    	
    	/* disegno */
    	DrawBattery();
    	DrawGas();
    	DrawEngines();
     		
    	glColor3f(1.0, 1.0, 1.0);
		glPrintf(5,17,0,10,"Battery\n");
		glPrintf(5,5,0,10,"%.1f", BatteryValue);
		glPrintf(80,17,0,10,"Gas\n");
		glPrintf(80,5,0,10,"%.1f", GasValue);
		glPrintf(145,17,0,10,"Engines\n");
		if(EnginesOn)
			glPrintf(178,33,0,10,"ON\n");
		else
			glPrintf(178,33,0,10,"OFF\n");	
		
		glPrintf(behave_x, behave_y, 0, 10, "Behaviour\n");	
		if(NewQ)
		{
			switch(Behaviour)
			{
				case SearchBall:
					glPrintf(behave_x+behave_dx,behave_y, 0, 10, "SRC\n");
					break;
				case GoToBall:
					glPrintf(behave_x+behave_dx,behave_y, 0, 10, "GTB\n");
					break;
				case CarryBall:
					glPrintf(behave_x+behave_dx,behave_y, 0, 10, "CAR\n");
					break;
				case NearWall:
					glPrintf(behave_x+behave_dx,behave_y, 0, 10, "NEW\n");
					break;
				case CatchBall:
					glPrintf(behave_x+behave_dx,behave_y, 0, 10, "CAT\n");
					break;
				case Defend:
					glPrintf(behave_x+behave_dx,behave_y, 0, 10, "DEF\n");
					break;
				case Wingman:
					glPrintf(behave_x+behave_dx,behave_y, 0, 10, "WIN\n");
					break;
				case AllBack:
					glPrintf(behave_x+behave_dx,behave_y, 0, 10, "ALB\n");
					break;
				case Stopper:
					glPrintf(behave_x+behave_dx,behave_y, 0, 10, "STP\n");
					break;
				case Goalie:
					glPrintf(behave_x+behave_dx,behave_y, 0, 10, "GOA\n");
					break;
			}  	
		}
		
		glPrintf(behave_x, task_y, 0, 10, "Task\n");	
		if(NewQ)
		{
			switch(Task)
			{
				case RoleG:
					glPrintf(behave_x+behave_dx, task_y, 0, 10, "RG\n");
					break;
				case Role1:
					glPrintf(behave_x+behave_dx, task_y, 0, 10, "R1\n");
					break;
				case Role2:
					glPrintf(behave_x+behave_dx, task_y, 0, 10, "R2\n");
					break;
				case Role3:
					glPrintf(behave_x+behave_dx, task_y, 0, 10, "R3\n");
					break;
			}
		}
		
		glPrintf(behave_x, status_y, 0, 10, "Status\n");	
		if(NewQ)
		{
			switch(Status)
			{
				case PlayerStopped:
					glPrintf(behave_x+behave_dx, status_y, 0, 10, "PST\n");	
					break;
				case EnemyHasBall:
					glPrintf(behave_x+behave_dx, status_y, 0, 10, "EHB\n");	
					break;
				case FreeBall:
					glPrintf(behave_x+behave_dx, status_y, 0, 10, "FRB\n");	
					break;
				case BallMine:
					glPrintf(behave_x+behave_dx, status_y, 0, 10, "BMN\n");	
					break;
				case Danger:
					glPrintf(behave_x+behave_dx, status_y, 0, 10, "DNG\n");	
					break;
				case Quiet:
					glPrintf(behave_x+behave_dx, status_y, 0, 10, "QIET\n");	
					break;
			}													
		}
															
    	glutSwapBuffers();
    	glutPostRedisplay();
		
		TIME_TO_REDRAW2=false;
	
	}
}
   	
/*************************************** Battery **********************************************/   	
void DrawBattery()
{    	
    	glPushMatrix();

			glColor3f(1.0, 0.0, 0.0);			
			glTranslatef(-SUBWIN_X/2.0, SUBWIN_Y/2.0, 0.0);

            glPushMatrix();

            	glTranslatef(0.0, 0.0, GRZ_UP/1000.0);
				glBegin(GL_POLYGON);
			  		if(BatteryCharge>0.66667)
						glColor3f(0.0, 0.0, 1.0);
			  		else	
			  		{	
						if(BatteryCharge>0.33333)
					  		glColor3f(1.0, 1.0, 0.0);
						else
					  		glColor3f(1.0, 0.0, 0.0);
				  	}	
			  		glVertex3f(0.0, 0.0, 0.0);
			  		glVertex3f(0.0, -y, 0.0);
			  		glVertex3f(x*BatteryCharge, -y, 0.0);
			  		glVertex3f(x*BatteryCharge, 0.0, 0.0);
				glEnd();
            	
				glPushMatrix();
				
				    glTranslatef(0.0, 0.0, GRZ_UP/1000.0);
					glBegin(GL_LINE_STRIP);
			  			glColor3f(0.0, 0.0, 0.0);
			  			glVertex3f(0.0, 0.0, 0.0);
			  			glVertex3f(0.0, -y, 0.0);
			  			glVertex3f(x, -y, 0.0);
			  			glVertex3f(x, -y*3.0/4.0, 0.0);
			  			glVertex3f(x+y/4.0, -y*3.0/4.0, 0.0);
			  			glVertex3f(x+y/4.0,-y/4.0, 0.0);
			  			glVertex3f(x, -y/4.0, 0.0);
			  			glVertex3f(x, 0.0, 0.0);
			  			glVertex3f(0.0, 0.0, 0.0);
					glEnd(); 	

				glPopMatrix();	
			
			glPopMatrix();	
			  			
    	glPopMatrix();
}    	
/*****************************************  Gas  **********************************************/       	
void DrawGas()
{    	
    	glPushMatrix();
    	
    		glTranslatef(-SUBWIN_X/2.0+x+x/4.0, SUBWIN_Y/2.0, GRZ_UP/1000.0);
			
			glColor3f(0.5, 0.9, 0.8);
			
			glBegin(GL_POLYGON);
				glVertex3f(GasPercentage*x, 0.0, 0.0);
				glVertex3f(GasPercentage*x, -y, 0.0);
				glVertex3f(0.0, -y, 0.0);
				glVertex3f(0.0, 0.0, 0.0);
			glEnd();
				
			glPushMatrix();
    		
    			glTranslatef(0.0, 0.0, GRZ_UP/1000.0);
			
				glColor3f(0.0, 0.0, 0.0);
			
				glBegin(GL_LINE_STRIP);
					glVertex3f(x, 0.0, 0.0);
					glVertex3f(0.0, 0.0, 0.0);
					glVertex3f(0.0, -y, 0.0);
					glVertex3f(x, -y, 0.0);
				glEnd();	
			
				glBegin(GL_LINE_STRIP);
					glVertex3f(GasPercentage*x, 0.0, 0.0);
					glVertex3f(GasPercentage*x, -y, 0.0);
				glEnd();
			
				glBegin(GL_LINE_STRIP);
					glVertex3f(GasPercentage*x, -y/2.0, 0.0);
					glVertex3f((GasPercentage*x+x/4.0), -y/2.0, 0.0);
				glEnd();
				
			glPopMatrix();
					
		glPopMatrix();	
		
} 		
/************************************************* Engines **********************************************/		
void DrawEngines()
{ 		
 		glPushMatrix();
 		
			float dx=x/5.0;
			float lato=x/10.0;
			glTranslatef(-dx, y*0.6, GRZ_UP/1000.0);
				
			glPushMatrix();
				
			    GRCircle C(lato);
				GColor enginescolor=Gblack;
				
				if(EnginesOn==true)
					enginescolor=Gred;
				
				C.Draw(Point(0.0, 0.0), 10, enginescolor, 1);
				
				glPushMatrix();
					
					glTranslatef(0.0, 0.0, GRZ_UP/1000.0);
					C.Draw(Point(0.0, 0.0), 10, Gblack, 0);
					
				glPopMatrix();	
							
			glPopMatrix();
		
		glPopMatrix();	
}

void reshape4(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(1.0, (GLfloat)w/(GLfloat)h, 0.1, 20.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    TIME_TO_REDRAW2=true;
}



void key_press_callback_radar(unsigned char key,int x, int y)
{
    switch(key)
    {
    	case 27:
    	case 'Q':
    	case 'q':
    		exit(1);
    		break;

/*    	case '1':
    		P.ListenToRobot(1);
    		glutSetWindowTitle("Golem Radar       ERONE");
    		break;
    	case '2':
    		P.ListenToRobot(2);
    		glutSetWindowTitle("Golem Radar       ARCHIMEDE");
    		break;
    	case '3':
    		P.ListenToRobot(3);
    		glutSetWindowTitle("Golem Radar       EUCLIDE");
    		break;
    	case '4':
    		P.ListenToRobot(4);
    		glutSetWindowTitle("Golem Radar       EULERO");
    		break;
*/
    	case '-':
    		if(RadarRadius<20000.0)
    		{	
    			RadarRadius+=1000.0;
	    		BG=GRBackGround(RadarRadius);
    			TIME_TO_REDRAW=true;
			}
		break;
    	case '+':
    		if(RadarRadius>2000.0)
    		{
    			RadarRadius-=1000.0;
    			BG=GRBackGround(RadarRadius);
    			TIME_TO_REDRAW=true;
    		}
    		break;
/*		
		case 's':
		case 'S':
	 		P.SendMsg(StartMsg);
	  		break;
		case 't':
		case 'T':
	  		P.SendMsg(StopMsg);
	 		break;  	
		case 'a':
		case 'A':
	  		P.SendMsg(SetupAttackMsg);
	  		break;
		case 'd':
		case 'D':
	  		P.SendMsg(SetupDefendMsg);
	  		break;
*/
    }
    reshape3(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void key_press_callback2_radar(unsigned char key,int x, int y)
{
    switch(key)
    {
    	case 'r':
    	    TIME_TO_REDRAW2=true;
    	    break;
    }	
    glutPostRedisplay();
}

void visibility_callback(int state)
{
  if(state==1)
  {
	TIME_TO_REDRAW=true;
	TIME_TO_REDRAW2=true;
	glutPostRedisplay();
  }	
}

/****************************************** GOLEM RADAR END *********************************/


Point pov(0.0, 0.0, 90.0);
Point focus(-3.0, 0.0, 0.0);

int mainwin, camcarwin;


Field *F=NULL;
Team *TeamA=NULL;
Team *TeamB=NULL;
Ball *B=NULL;
gaTimer *T=NULL;
ETTimer *ET;
GJoyControl *Joy=NULL;

bool JOY_ENABLED=false;
bool GA_TIME_TO_REDRAW=true;

void GiveSightToAll();
void reshape2(int, int );
void ReadJoy();


void init(void)
{
    F = new Field();
	B = new Ball(0.0, 0.0, 0.0, 0.0);
	BALL_PRESENT=true;
    T=new gaTimer();
    ET=new ETTimer();
    ET->ResetTimer();
    TeamA=new Team(Gcyan, Gyellow, 1);
	TEAMA_PRESENT=true;
	r_man=0;
	team_active=TEAMA;
	GA_STATE=MOVE_BALL;
	GA_VIEW=GA_GLOBAL_VIEW;
}

void initGL()
{
	GLfloat a=0.1, aw=8.0,
			d0=0.6, d0w=1.0,
			d1=0.8, d1w=1.0,
			p0x=5.0, p0y=0.0, p0z=1.0, p0w=1.0,
			p1x=6.0, p1y=3.0, p1z=1.0, p1w=0.5;
			
    glClearColor(0.1, 0.1, 0.1, 0.0);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    GLfloat amb0[]={a, a, a, aw};
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
    GLfloat dif0[]={d0, d0, d0, d0w};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dif0);
    GLfloat pos0[]={p0x, p0y, p0z, p0w};
    glLightfv(GL_LIGHT0, GL_POSITION, pos0);
    GLfloat dif1[]={d1, d1, d1, d1w};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dif1);
    GLfloat pos1[]={p1x, p1y, p1z, p1w};
    glLightfv(GL_LIGHT0, GL_POSITION, pos1);


}

void display(void)
{

#ifdef TOTAL_DEBUG
	if(NEXT_STEP)
 	{
#endif
	float time=ET->ReadTimer();
	static float lasttime;
	GA_TIME_TO_REDRAW = time-lasttime>0.05 ? true: false;
 	if(GAPAUSE==false && GA_TIME_TO_REDRAW)
 	{
 		lasttime=time;
   		static float Period = 20000.0;   // microsecondi !!
   		float LastPeriod=Period;
   		if(TEAMA_PRESENT)
   	   		LastPeriod = TeamA->GetPeriod();
   		else
       		if(TEAMB_PRESENT)
   	       		LastPeriod = TeamB->GetPeriod();
   		static float lasttime = 0.0;
   		static bool PERIOD_CHANGED = false;
   		
   		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    	
    	glPushMatrix();
    		if(GA_VIEW==GA_GLOBAL_VIEW)
    		{
    			glRotatef(view_rotx, 1.0, 0.0, 0.0);
    			glRotatef(view_roty, 0.0, 1.0, 0.0);
    			glRotatef(view_rotz, 0.0, 0.0, 1.0);
    			glTranslatef(field_x, field_y, field_z);
            }
    		static int ilasttime;
  			int dt;
  			dt=glutGet(GLUT_ELAPSED_TIME)-ilasttime;
  			ilasttime=glutGet(GLUT_ELAPSED_TIME);

			float fps=FPS(dt);

			glColor3f(1,1,1);
			glPrintf(10,10,0,10,"fps %f\n",fps);
   			glPrintf(glutGet(GLUT_WINDOW_WIDTH)-100,10,0,10,
            	"time %d:%d\n",(T->ReadTimer()/60), T->ReadTimer()%60);
			F->Draw();  // fondo campo
	    	if(TEAMA_PRESENT)
	    		TeamA->Draw();
	    	if(TEAMB_PRESENT)
	    		TeamB->Draw();
	    	if(BALLFLAG)
				B->Draw();  // palla	    	

    		Period=((float)glutGet(GLUT_ELAPSED_TIME)-lasttime)*1000.0;
    		PERIOD_CHANGED = (fabs(Period-LastPeriod) > 1000.0) ? true : false;    	
//  		if (PERIOD_CHANGED)
    		{   if(TEAMA_PRESENT)
					TeamA->SetPeriod(Period);
				if(TEAMB_PRESENT)
					TeamB->SetPeriod(Period);
				if(BALLFLAG)
	   				B->SetPeriod(Period);
    		}
    		lasttime = (float)glutGet((GLenum)GLUT_ELAPSED_TIME);
//  		printf("%s%f\n","Period (microseconds) : ",Period);


			/* aggiorno le squadre */     	
/*			
			if(JOY_ENABLED)
	    		ReadJoy();
*/
    		if(TEAMA_PRESENT)
	    		TeamA->Update();
    		if(TEAMB_PRESENT)
    			TeamB->Update();
    	
    		/* seleziono il robot del team attivo */	
    		if(team_active==TEAMA)	
    		{
    			if(TEAMA_PRESENT)
    				TeamA->Select(r_man);
    			if(TEAMB_PRESENT)
    				TeamB->DeselectAll();
    		}
    	
    		else	
    		{
    			if(TEAMB_PRESENT)
    				TeamB->Select(r_man);
         		if(TEAMA_PRESENT)
         			TeamA->DeselectAll();
        	}		
    			    	
    		/* chiamo la funzione degli urti robot-robot */
    		RRCollide(TeamA, TeamB);
    	
    		/* ricostruisco il mondo in coordinate globali
    		e lo passo ai robot che lo manipoleranno */
			GiveSightToAll();
		
			/* chiamo la funzione che calcola gli urti palla-robot */
			if(BALLFLAG)
    		{
				B->Update();
				if(TEAMA_PRESENT)
					RBCollide(TeamA);
				if(TEAMB_PRESENT)	
					RBCollide(TeamB);
    		}
	
    	glPopMatrix();
    	  	
    	GA_TIME_TO_REDRAW=false;
	}
	
	glutSwapBuffers();
    glutPostRedisplay();

#ifdef TOTAL_DEBUG
	}
#endif
  	NEXT_STEP=false;

}

void reshape(int w, int h)
{
	if(GA_VIEW==GA_SUBJECTIVE_VIEW)
    {
    	glViewport(0, 0, w, h);
    	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();
    	gluPerspective(45.0, (GLdouble)w/(GLdouble)h, 0.1, 20.0);
    	glMatrixMode(GL_MODELVIEW);
    	glLoadIdentity();
    	GLfloat myangle = pov.h;
    	GLfloat myx = pov.x;
    	GLfloat lookatx = focus.x;//myx+(1.1*cos(myangle*M_PI/180.0));
    	GLfloat myy = pov.y;
    	GLfloat lookaty = focus.y;//myy+(1.1*sin(myangle*M_PI/180.0));
    	GLfloat myz = 1.8;
    	GLfloat lookatz = 0.0;
    	gluLookAt(myx, myy, myz, lookatx, lookaty, lookatz, 0.0, 0.0, 1.0);
    }
    else
    {
    	glViewport(0, 0, w, h);
    	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();
    	gluPerspective(p, (GLfloat)w/(GLfloat)h, l, m);
    	glMatrixMode(GL_MODELVIEW);
    	glLoadIdentity();
    	gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
    }
}

void display2()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glPushMatrix();
    	F->Draw();  // fondo campo
	    if(TEAMA_PRESENT)
	    	TeamA->Draw();
	    if(TEAMB_PRESENT)
	    	TeamB->Draw();
	    if(BALLFLAG)
			B->Draw();  // palla	    	

    glPopMatrix();
    glutSwapBuffers();
    glutPostRedisplay();
    glutSetWindow(camcarwin);
    reshape2(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));;

}

void reshape2(int w, int h)
{
 	
	Team *T;
	bool TEAM_PRESENT=false;
	if(TEAMA_PRESENT && team_active==TEAMA)
	{
		T=TeamA;
		TEAM_PRESENT=true;
	}	
	else
		if(TEAMB_PRESENT && team_active==TEAMB)
		{
			T=TeamB;
			TEAM_PRESENT=true;
		}	
    if(TEAM_PRESENT)
    {
    	glViewport(0, 0, w, h);
    	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();
    	gluPerspective(60.0, (GLdouble)w/(GLdouble)h, 0.3, 20.0);
    	glMatrixMode(GL_MODELVIEW);
    	glLoadIdentity();
    	GLfloat myangle = T->RV[r_man]->GetP().h;
    	GLfloat myx = T->RV[r_man]->GetP().x;
    	GLfloat lookatx = myx+(1.1*cos(myangle*M_PI/180.0));
    	GLfloat myy = T->RV[r_man]->GetP().y;
    	GLfloat lookaty = myy+(1.1*sin(myangle*M_PI/180.0));
    	GLfloat myz = 0.6;
    	GLfloat lookatz = 0.0;
    	gluLookAt(myx, myy, myz, lookatx, lookaty, lookatz, 0.0, 0.0, 1.0);
    }

}

/************** Gestione del Joystick *****************************/
void ReadJoy()
{
		
	Team *T;
	bool THERESATEAM=false;
	if(team_active==TEAMA)
	{
		if(TEAMA_PRESENT)
		{	
			T=TeamA;
			THERESATEAM=true;
		}
	}
	else
	{
		if(TEAMB_PRESENT)
			if(team_active==TEAMB)
			{
				T=TeamB;
				THERESATEAM=true;
			}
     }
     if(THERESATEAM)
     {
     	float jog=0.0;
     	GVector vel;
     	Joy->ReadAndParse();
    	jog=Joy->ReadJog();
     	Joy->ReadSpeed(vel);
 //    	printf("\nmain: jog=%f, vel=%f,%f ",jog, vel.GetRo(), vel.GetTheta());
   //  	fflush(stdout);
     	T->RV[r_man]->SetVel(vel*0.001, jog);
     	/*
     	kd = gj->ReadKickDX();
     	ks = gj->ReadKickSX();
     	kf = gj->ReadKickFWD();
     	*/
     }	
}

void GiveSightToAll()
{


	    	
//************** Comunica gli oggetti ai Golem *******************	    	
	
	int gindex=0;	    	

  	Balloon_struct bs;
  	Goals_struct gs;
  	Walls_struct ws;
  	int Robot_Seen=0;
  	All_Robots_struct rs;
	
  	
  	
	/**********************************/
	/*            palla               */
	/**********************************/
	if(BALL_PRESENT)
	{	
		bs.Pos=GVector(B->GetP().x, B->GetP().y, Cartesian);
		bs.Speed=GVector(B->GetV().x, B->GetV().y, Cartesian);
		bs.Ball_Vision=Seen;
	
	    /* TimeStamp e PointOfView vengono settati in Robot.cpp */
	}
	else
		bs.Ball_Vision=Unseen;
	
		
		
	/**********************************/
	/*            robots              */
	/**********************************/	
	if(TEAMA_PRESENT)
		for(int i=0;i<TeamA->Getn(); i++)
		{
		    rs.Robot[gindex].ID=gindex; //Assolutamente non index!!!!!!!!!!!!!!!!!!!!!! by Roby
		    rs.Robot[gindex].Pos=GVector(TeamA->RV[i]->GetP().x, TeamA->RV[i]->GetP().y, Cartesian);
		    rs.Robot[gindex].Speed=GVector(TeamA->RV[i]->GetV().x, TeamA->RV[i]->GetV().y, Cartesian);
		
		    /* momentaneamente imposto come TeamMate tutti i robot
		    Gcyan e come Enemy tutti i Gmagenta. Ci penserà Robot.cpp a correggere */
		    rs.Robot[gindex].Type= TeamA->GetColor()==Gcyan ? TeamMate : Enemy;
		    gindex++;	
		
	        /* TimeStamp e PointOfView vengono settati in Robot.cpp */
	    	}
	    	
         if(TEAMB_PRESENT)
		for(int i=0;i<TeamB->Getn(); i++)
		{
		    rs.Robot[gindex].ID=gindex;
		    rs.Robot[gindex].Pos=GVector(TeamB->RV[i]->GetP().x, TeamB->RV[i]->GetP().y, Cartesian);
		    rs.Robot[gindex].Speed=GVector(TeamB->RV[i]->GetV().x, TeamB->RV[i]->GetV().y, Cartesian);
		
		    /* momentaneamente imposto come TeamMate tutti i robot
		    Gcyan e come Enemy tutti i magente. Ci penserà Robot.cpp a correggere */
		    rs.Robot[gindex].Type= TeamB->GetColor()==Gcyan ? TeamMate : Enemy;
		    gindex++;	
		
	        /* TimeStamp e PointOfView vengono settati in Robot.cpp */
	    }
	if(DEBUG_WORLD)
	{
		Point center(0.0, 0.0);
		for(int i=0; i<gindex; i++)
			DrawVector(center, rs.Robot[i].Pos);
	}		

	
	if(TEAMA_PRESENT)
		Robot_Seen += TeamA->Getn();
	if(TEAMB_PRESENT)
		Robot_Seen += TeamB->Getn();
		
		
		
	/**********************************/
	/*            porte               */
	/**********************************/
	gs.MyGoalSeen=Seen;
	gs.EnemyGoalSeen=Seen;
	Point BlueSX(-XDIM, 1.0);
	Point BlueDX(-XDIM, -1.0);
	Point YellowSX(XDIM, 1.0);
	Point YellowDX(XDIM, -1.0);
	
	/* qui suppongo che la mia porta sia blu e gialla quella degli avversari
	   MyGoal = porta Blue, EnemyGoal = porta Gialla */
	gs.MyGoalPos1=GVector(BlueSX.x, BlueSX.y, Cartesian);
	gs.MyGoalPos2=GVector(BlueDX.x, BlueDX.y, Cartesian);
	gs.EnemyGoalPos1=GVector(YellowSX.x, YellowSX.y, Cartesian);
	gs.EnemyGoalPos2=GVector(YellowDX.x, YellowDX.y, Cartesian);
 	if(DEBUG_WORLD)
	{
		Point center(0.0, 0.0);
		DrawVector(center, gs.MyGoalPos1, Gwhite);
		DrawVector(center, gs.MyGoalPos2, Gwhite);
		DrawVector(center, gs.EnemyGoalPos1, Gwhite);
		DrawVector(center, gs.EnemyGoalPos2, Gwhite);
        }
	/* GoalType, TimeStamp e PointOfView vengono settati in Robot::OpenGEyes */
	
	
	
	/**********************************/
	/*            muri                */
	/**********************************/
 	ws.FW=GVector(XDIM, 0.0, Cartesian);
    ws.BW=GVector(-XDIM, 0.0, Cartesian);
    ws.RW=GVector(0.0, -YDIM, Cartesian);
    ws.LW=GVector(0.0, YDIM, Cartesian);
	
	if(DEBUG_WORLD)
	{
		GVector center(0.0, 0.0);
		DrawVector(center, ws.FW, 0.15, false, Gblack);
		DrawVector(center, ws.BW, 0.15, false, Gblack);
		DrawVector(center, ws.LW, 0.15, false, Gblack);
		DrawVector(center, ws.RW, 0.15, false, Gblack);
  	}
	
	/* le Speed, TimeStamp e PointOfView vengono settati in Robot::OpenGEyes */
	
        if(TEAMA_PRESENT)
			for(int i=0; i<TeamA->Getn(); i++)
	    		TeamA->RV[i]->OpenGEyes(bs, ws, rs, gs, Robot_Seen);	
		if(TEAMB_PRESENT)
			for(int i=0; i<TeamB->Getn(); i++)
	    		TeamB->RV[i]->OpenGEyes(bs, ws, rs, gs, Robot_Seen);	
}	    	   		
	    		





int main(int argc, char *argv[])
{
    printf("\n\n\n*****************************************************************");
    printf("\n*                         Golem Arena                           *");
    printf("\n*****************************************************************\n\n\n");

   glutInit(&argc, argv);
#ifdef CAMERACAR
    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(200, 200);
    glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH)-200, 0);
    camcarwin = glutCreateWindow("Camera Car");
    glutKeyboardFunc(key_press_callback2);
    glutDisplayFunc(display2);
    glutReshapeFunc(reshape2);
    initGL();
#endif

if(GA_RADAR_WINDOW)
{


   	glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
   	glutInitWindowSize(WINDOW_W, WINDOW_H);
   	glutInitWindowPosition(glutGet(GLUT_SCREEN_WIDTH)-WINDOW_W, 0);
   	mainwin_radar = glutCreateWindow("Golem Radar       ERONE");

   	glutKeyboardFunc(key_press_callback_radar);
   	glutDisplayFunc(display3);
   	glutReshapeFunc(reshape3);
	glutVisibilityFunc(visibility_callback);
   	initGL_radar();

   	glutInitWindowPosition(0, 0);
   	subwin_radar = glutCreateSubWindow(mainwin_radar, 0, 0, WINDOW_W, 50);

   	glutMouseFunc(mouse_callback);
	glutDisplayFunc(display4);
   	glutReshapeFunc(reshape4);
   	glutKeyboardFunc(key_press_callback2_radar);
   	initGL_radar();
   	init_radar();
    	
}


    glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
    glutInitWindowSize(640, 400);
    glutInitWindowPosition(0, 0);
    mainwin = glutCreateWindow("Golem Arena");
    initGL();

#ifdef FULLSCREEN
    glutFullScreen();
#endif


    glutKeyboardFunc(key_press_callback);
    glutSpecialFunc(SKey);
    glutSpecialUpFunc(SKeyUp);
    glutMouseFunc(mouse_callback);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    createmenus();
    glutMenuStateFunc(menu_state_func);

    init();
    glutMainLoop();
    return 0;



}


