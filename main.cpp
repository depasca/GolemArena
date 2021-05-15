#include <cstdlib>
#include <iostream>

using namespace std;

#include <GL/openglut.h>
#include <stdio.h>

#include <stdlib.h>
#include <math.h>
#include "flags.h"
#include "mainflags.h"
#include "collisions.h"
#include "field.h"
#include "keys.h"
#include "menu.h"
#include "mouse.h"
//#include "gjoycontrol.h"
#include "gautils.h"
#include "utility.h"
#include "gatimer.h"

//#define TOTAL_DEBUG
#define DEBUG_WORLD false
//#define CAMERACAR
#define GA_RADAR_WINDOW true
//#define FULLSCREEN

/****************************************** GOLEM RADAR *************************************/
#include "grobjects.h"

void reshape2(int, int);
void reshape(int, int);
void DrawBattery();
void DrawGas();
void DrawEngines();

bool GRPAUSE = false;
bool TIME_TO_REDRAW = true;
bool TIME_TO_REDRAW2 = true;

float RadarRadius = GRRADAR_RADIUS;

/******************* GLPRINTF **********************/
float x = 0.2;	 //(GLfloat)glutGet(GLUT_WINDOW_WIDTH);
float y = 0.075; //(GLfloat)glutGet(GLUT_WINDOW_HEIGHT);
int behave_x = 220;
int behave_dx = 90;
int behave_y = 38;
int status_y = 23;
int task_y = 8;

/***************************************************/

/* messaggio STATUS */
float BatteryValue = 0.0;  /* valoree reala in Volts */
float BatteryCharge = 0.0; /* compresa fra 0.0 e 1.0 */
bool EnginesOn = false;
float GasValue = 0.0;	   /* valoree reala in Volts */
float GasPercentage = 0.0; /* compresa fra 0.0 e 1.0 */

/* messaggio Q */
bool NewQ = false;
GBehaviour_Type Behaviour;
Task_Type Task;
Status_Type Status;
/***************************************************/

int mainwin_radar, subwin_radar;

gaTimer *T_radar = NULL;
GA_Point center(0.0, 0.0);
GRBackGround BG(RadarRadius);

void init_radar(void)
{
	T_radar = new gaTimer();
}

void initGL_radar()
{
	GLfloat a = 100.0, aw = 8000.0,
			d0 = 600.0, d0w = 1000.0,
			p0x = 5000.0, p0y = 0.0, p0z = 1000.0, p0w = 1000.0;

	glClearColor(0.1, 0.1, 0.1, 0.0);
	glEnable(GL_LIGHTING);
	if (USE_DEPTH_TEST)
		glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHT0);
	glEnable(GL_COLOR_MATERIAL);
	glShadeModel(GL_FLAT);
	glEnable(GL_NORMALIZE);
	GLfloat amb0[] = {a, a, a, aw};
	glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
	GLfloat dif0[] = {d0, d0, d0, d0w};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, dif0);
	GLfloat pos0[] = {p0x, p0y, p0z, p0w};
	glLightfv(GL_LIGHT0, GL_POSITION, pos0);
}

void display3(void)
{

	static int lastdrawtime;
	int ddt;
	if (FORCE_REDRAW_PERIOD)
	{
		/*aggiorno la finestra solo se sono passati almeno
	  MILLI_PERIOD millisecondi dall'aggiornamento precedente */
		ddt = glutGet(GLUT_ELAPSED_TIME) - lastdrawtime;
		TIME_TO_REDRAW = ddt >= MILLI_PERIOD ? true : false;
	}

	if (GRPAUSE == false && TIME_TO_REDRAW)
	{

		lastdrawtime = glutGet(GLUT_ELAPSED_TIME);

		if (USE_DEPTH_TEST)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		else
			glClear(GL_COLOR_BUFFER_BIT);

		if (USE_DEPTH_TEST)
		{
			/* scrivo informazioni utili sulla finestra */
			static int ilasttime;
			int dt = glutGet(GLUT_ELAPSED_TIME) - ilasttime;
			ilasttime = glutGet(GLUT_ELAPSED_TIME);
			float fps = FPS(dt);
			glColor3f(1.0, 1.0, 1.0);
			glPrintf(10, 10, 0, 10, "fps %f\n", fps);
			glPrintf(glutGet(GLUT_WINDOW_WIDTH) - 100, 10, 0, 10,
					 "time %d:%d\n", (T_radar->ReadTimer() / 60), T_radar->ReadTimer() % 60);
		}

		/* disegno */
		glPushMatrix();

		BG.Draw(center);

		float robot_orientation = 0.0;
		Team *T = NULL;
		bool THERESATEAM = false;
		if (team_active == TEAMA)
		{
			if (TEAMA_PRESENT)
			{
				T = TeamA;
				THERESATEAM = true;
			}
		}
		else
		{
			if (TEAMB_PRESENT)
			{
				T = TeamB;
				THERESATEAM = true;
			}
		}
		if (THERESATEAM)
			robot_orientation = T->RV[r_man]->GetP().h;
		if (T->RV[r_man]->GetSide() == Gblue)
			robot_orientation -= 180.0;
		glRotatef(90.0 - robot_orientation, 0.0, 0.0, 1.0);

		if (THERESATEAM)
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

		TIME_TO_REDRAW = false;
	}
	glutPostRedisplay();

	//	reshape(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
}

void reshape3(int w, int h)
{
	glViewport(0, 0, w, h - 50);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(455.0, (GLfloat)w / (GLfloat)(h - 50), 1000.0, RadarRadius + 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, RadarRadius, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	TIME_TO_REDRAW = true;
}

void display4(void)
{

	if (GRPAUSE == false && TIME_TO_REDRAW2)
	{
		Team *T = NULL;
		bool THERESATEAM = false;
		if (team_active == TEAMA)
		{
			if (TEAMA_PRESENT)
			{
				T = TeamA;
				THERESATEAM = true;
			}
		}
		else
		{
			if (TEAMB_PRESENT)
			{
				T = TeamB;
				THERESATEAM = true;
			}
		}
		if (THERESATEAM)
		{
			BatteryValue = T->RV[r_man]->myParser->BatteryCharge();
			BatteryCharge = (BatteryValue - BATTERY_CHARGE_MIN) / BATTERY_CHARGE_RANGE;
			//			printf("\ndisplay2: BVal=%f, BCarge=%f ",BatteryValue, BatteryCharge);
			if (BatteryCharge < 0.0)
				BatteryCharge = 0.0;
			if (BatteryCharge > 1.0)
				BatteryCharge = 1.0;
			EnginesOn = T->RV[r_man]->myParser->EnginesOn();
			GasValue = T->RV[r_man]->myParser->GasCharge();
			GasPercentage = (GasValue - GAS_CHARGE_MIN) / GAS_CHARGE_RANGE;
			//			printf("\ndisplay2: GasVal=%f, GPer=%f ",GasValue, GasPercentage);
			if (GasPercentage < 0.0)
				GasPercentage = 0.0;
			if (GasPercentage > 1.0)
				GasPercentage = 1.0;
			NewQ = T->RV[r_man]->myParser->NewQ();
			//   			printf("\ndisplay2: NewQ=%f ", NewQ);
			Behaviour = T->RV[r_man]->myParser->Behaviour();
			Task = T->RV[r_man]->myParser->Task();
			Status = T->RV[r_man]->myParser->Status();
		}
		glClearColor(0.7, 0.7, 0.7, 0.0);
		if (USE_DEPTH_TEST)
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		else
			glClear(GL_COLOR_BUFFER_BIT);

		/* disegno */
		DrawBattery();
		DrawGas();
		DrawEngines();

		glColor3f(1.0, 1.0, 1.0);
		glPrintf(5, 17, 0, 10, "Battery\n");
		glPrintf(5, 5, 0, 10, "%.1f", BatteryValue);
		glPrintf(80, 17, 0, 10, "Gas\n");
		glPrintf(80, 5, 0, 10, "%.1f", GasValue);
		glPrintf(145, 17, 0, 10, "Engines\n");
		if (EnginesOn)
			glPrintf(178, 33, 0, 10, "ON\n");
		else
			glPrintf(178, 33, 0, 10, "OFF\n");

		glPrintf(behave_x, behave_y, 0, 10, "Behaviour\n");
		if (NewQ)
		{
			switch (Behaviour)
			{
			case SearchBall:
				glPrintf(behave_x + behave_dx, behave_y, 0, 10, "SRC\n");
				break;
			case GoToBall:
				glPrintf(behave_x + behave_dx, behave_y, 0, 10, "GTB\n");
				break;
			case CarryBall:
				glPrintf(behave_x + behave_dx, behave_y, 0, 10, "CAR\n");
				break;
			case NearWall:
				glPrintf(behave_x + behave_dx, behave_y, 0, 10, "NEW\n");
				break;
			case CatchBall:
				glPrintf(behave_x + behave_dx, behave_y, 0, 10, "CAT\n");
				break;
			case Defend:
				glPrintf(behave_x + behave_dx, behave_y, 0, 10, "DEF\n");
				break;
			case Wingman:
				glPrintf(behave_x + behave_dx, behave_y, 0, 10, "WIN\n");
				break;
			case AllBack:
				glPrintf(behave_x + behave_dx, behave_y, 0, 10, "ALB\n");
				break;
			case Stopper:
				glPrintf(behave_x + behave_dx, behave_y, 0, 10, "STP\n");
				break;
			case Goalie:
				glPrintf(behave_x + behave_dx, behave_y, 0, 10, "GOA\n");
				break;
			}
		}

		glPrintf(behave_x, task_y, 0, 10, "Task\n");
		if (NewQ)
		{
			switch (Task)
			{
			case RoleG:
				glPrintf(behave_x + behave_dx, task_y, 0, 10, "RG\n");
				break;
			case Role1:
				glPrintf(behave_x + behave_dx, task_y, 0, 10, "R1\n");
				break;
			case Role2:
				glPrintf(behave_x + behave_dx, task_y, 0, 10, "R2\n");
				break;
			case Role3:
				glPrintf(behave_x + behave_dx, task_y, 0, 10, "R3\n");
				break;
			}
		}

		glPrintf(behave_x, status_y, 0, 10, "Status\n");
		if (NewQ)
		{
			switch (Status)
			{
			case PlayerStopped:
				glPrintf(behave_x + behave_dx, status_y, 0, 10, "PST\n");
				break;
			case EnemyHasBall:
				glPrintf(behave_x + behave_dx, status_y, 0, 10, "EHB\n");
				break;
			case FreeBall:
				glPrintf(behave_x + behave_dx, status_y, 0, 10, "FRB\n");
				break;
			case BallMine:
				glPrintf(behave_x + behave_dx, status_y, 0, 10, "BMN\n");
				break;
			case Danger:
				glPrintf(behave_x + behave_dx, status_y, 0, 10, "DNG\n");
				break;
			case Quiet:
				glPrintf(behave_x + behave_dx, status_y, 0, 10, "QIET\n");
				break;
			}
		}

		glutSwapBuffers();
		glutPostRedisplay();

		TIME_TO_REDRAW2 = false;
	}
}

/*************************************** Battery **********************************************/
void DrawBattery()
{
	glPushMatrix();

	glColor3f(1.0, 0.0, 0.0);
	glTranslatef(-SUBWIN_X / 2.0, SUBWIN_Y / 2.0, 0.0);

	glPushMatrix();

	glTranslatef(0.0, 0.0, GRZ_UP / 1000.0);
	glBegin(GL_POLYGON);
	if (BatteryCharge > 0.66667)
		glColor3f(0.0, 0.0, 1.0);
	else
	{
		if (BatteryCharge > 0.33333)
			glColor3f(1.0, 1.0, 0.0);
		else
			glColor3f(1.0, 0.0, 0.0);
	}
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, -y, 0.0);
	glVertex3f(x * BatteryCharge, -y, 0.0);
	glVertex3f(x * BatteryCharge, 0.0, 0.0);
	glEnd();

	glPushMatrix();

	glTranslatef(0.0, 0.0, GRZ_UP / 1000.0);
	glBegin(GL_LINE_STRIP);
	glColor3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, -y, 0.0);
	glVertex3f(x, -y, 0.0);
	glVertex3f(x, -y * 3.0 / 4.0, 0.0);
	glVertex3f(x + y / 4.0, -y * 3.0 / 4.0, 0.0);
	glVertex3f(x + y / 4.0, -y / 4.0, 0.0);
	glVertex3f(x, -y / 4.0, 0.0);
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

	glTranslatef(-SUBWIN_X / 2.0 + x + x / 4.0, SUBWIN_Y / 2.0, GRZ_UP / 1000.0);

	glColor3f(0.5, 0.9, 0.8);

	glBegin(GL_POLYGON);
	glVertex3f(GasPercentage * x, 0.0, 0.0);
	glVertex3f(GasPercentage * x, -y, 0.0);
	glVertex3f(0.0, -y, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glEnd();

	glPushMatrix();

	glTranslatef(0.0, 0.0, GRZ_UP / 1000.0);

	glColor3f(0.0, 0.0, 0.0);

	glBegin(GL_LINE_STRIP);
	glVertex3f(x, 0.0, 0.0);
	glVertex3f(0.0, 0.0, 0.0);
	glVertex3f(0.0, -y, 0.0);
	glVertex3f(x, -y, 0.0);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(GasPercentage * x, 0.0, 0.0);
	glVertex3f(GasPercentage * x, -y, 0.0);
	glEnd();

	glBegin(GL_LINE_STRIP);
	glVertex3f(GasPercentage * x, -y / 2.0, 0.0);
	glVertex3f((GasPercentage * x + x / 4.0), -y / 2.0, 0.0);
	glEnd();

	glPopMatrix();

	glPopMatrix();
}
/************************************************* Engines **********************************************/
void DrawEngines()
{
	glPushMatrix();

	float dx = x / 5.0;
	float lato = x / 10.0;
	glTranslatef(-dx, y * 0.6, GRZ_UP / 1000.0);

	glPushMatrix();

	GRCircle C(lato);
	GColor enginescolor = Gblack;

	if (EnginesOn == true)
		enginescolor = Gred;

	C.Draw(GA_Point(0.0, 0.0), 10, enginescolor, 1);

	glPushMatrix();

	glTranslatef(0.0, 0.0, GRZ_UP / 1000.0);
	C.Draw(GA_Point(0.0, 0.0), 10, Gblack, 0);

	glPopMatrix();

	glPopMatrix();

	glPopMatrix();
}

void reshape4(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(1.0, (GLfloat)w / (GLfloat)h, 0.1, 20.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	TIME_TO_REDRAW2 = true;
}

void key_press_callback_radar(unsigned char key, int x, int y)
{
	switch (key)
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
		if (RadarRadius < 20000.0)
		{
			RadarRadius += 1000.0;
			BG = GRBackGroundehave_dx = 90;
