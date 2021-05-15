#include <iostream.h>
#include "grparser.h"

bool GR_DEBUG_RECEIVE=true;
bool GR_DEBUG_SEND=true;

extern "C" {
#include <ethbc.h>
}

extern bool TIME_TO_REDRAW;
extern bool TIME_TO_REDRAW2;
  	
GRParser::GRParser(char *_ip, int _port, int _id, GColor myc, GColor mygoalc)
{
	ip=_ip;
	port=_port;
	id=_id;
	mycolor=myc;
	opcolor = mycolor==Gcyan ? Gmagenta : Gcyan;
	mygoalcolor=mygoalc;
	opgoalcolor = mygoalcolor==Gblue ? Gyellow : Gblue;
	
	QArrived=0;
	BallSeen=0;
	VectorSeen=0;
	TargetSeen=0;
	ObjectsSeen=0;
	MGSeen=0;
	OGSeen=0;
	FW_WallSeen=0;
	BW_WallSeen=0;
	RW_WallSeen=0;
	LW_WallSeen=0;
	
	HowManyObjects=0;
}

GRParser::~GRParser(){}
  	
void GRParser::Init()
{
}

void GRParser::ListenToRobot(int _id)
{
	id=_id;
}	
  	
void GRParser::DoYourDuty(void *rmsg, int type)
{


	
/*
	gdata.MyGoalPos1=GVector(5000.0, -1.0);
	gdata.MyGoalPos2=GVector(2000.0, 1.0);
	MGSeen=20;
*/	
	TIME_TO_REDRAW=true;

   	switch(type)
   	{
    	case GMSG_Status:
        	TIME_TO_REDRAW2=true;	
	  		sdata= *(struct Golem_status*)rmsg;
	   		break;
		
	   	case GMSG_Q:
        	TIME_TO_REDRAW2=true;	
	  		qdata= *(struct MsgQ_struct*)rmsg;
	   		QArrived=20;
	   		break;
		
    	case GMSG_Ball:
			bdata = *(struct Balloon_struct*)rmsg;
			if(bdata.Ball_Vision!=Unseen)
		  	BallSeen=20;
			break;
        	
        case GMSG_DrawVector:
        	vdata = *(MsgVector*)rmsg;
			VectorSeen=20;
			break;
				
        case GMSG_Target:
			tdata = *(struct Target_struct*)rmsg;
			TargetSeen=20;
			break;
				
/************************************************ Beta ***************************************************/				
		case GMSG_Obstacle:
			rdata = *(struct All_Robots_struct*)rmsg;
        	HowManyObjects=rdata.Header.Mate_Seen+rdata.Header.Enemy_Seen+rdata.Header.Unknown_Seen;
        	ObjectsSeen=20;
        	break;
        			
        case GMSG_Goal:
		   	gdata = *(struct Goals_struct*)rmsg;
        	if(gdata.MyGoalSeen!=Unseen)
        	{
				MGSeen=20;
        	}
        		
        	if(gdata.EnemyGoalSeen!=Unseen)
        	{
				OGSeen=20;
        	}	
        	break;
    	case GMSG_Wall:
			wdata = *(struct Walls_struct*)rmsg;
		    if(wdata.FW_Seen!=Unseen)
        	{
        		fw=GRWall(wdata.FW);
        		FW_WallSeen=20;
        	}	
        	
        	if(wdata.BW_Seen!=Unseen)
        	{
        		bw=GRWall(wdata.BW);
        		BW_WallSeen=20;
        	}
        	        			
        	if(wdata.RW_Seen!=Unseen)
        	{
        		rw=GRWall(wdata.RW);
        		RW_WallSeen=20;
        	}	
        		
        	if(wdata.LW_Seen!=Unseen)
        	{
        		lw=GRWall(wdata.LW);
        		LW_WallSeen=20;
        	}	
        	break;
        	
    } // chiude lo switch					

    fflush(stdout);

	UpdateBooleans();
		
} // chiude il DoYourDuty

void GRParser::UpdateBooleans()
{
	if(QArrived>0)
		QArrived--;
}

void GRParser::SendMsg(MsgStartStop_Type value)
{
	struct ETMessage *smsg;
	smsg=(struct ETMessage*)malloc(sizeof(struct ETMessage));
	ETCreateMsg(smsg, sizeof(MsgStartStop_Type), GMSG_StartStop);
	*(MsgStartStop_Type*)smsg->Data=value;
	ShareMsg(smsg, 10);
	
	if(CONFIRM_OUTGOING_MESSAGES)
		switch(value)
	  	{
			case SetupAttackMsg:
				if(GR_DEBUG_SEND)
					printf("\nParser: sent message SetupAttack");
				break;
	  
			case SetupDefendMsg:
		  		if(GR_DEBUG_SEND)
					printf("\nParser: sent message SetupDefend");
	  	  		break;	
	  
			case StartMsg:
		  		if(GR_DEBUG_SEND)
					printf("\nParser: sent message Start");
		  		break;	
		
			case StopMsg:
		  		if(GR_DEBUG_SEND)
					printf("\nParser: sent message Stop");
		  		break;	
	  	}
	fflush(stdout);	
//	TransmitMail();
}

float GRParser::BatteryCharge()
{
	return sdata.battery;
}

float GRParser::GasCharge()
{
	return sdata.gas;
}

bool GRParser::EnginesOn()
{
	return sdata.engineson;
}

bool GRParser::NewQ()
{	
	return QArrived;
}	
	
GBehaviour_Type GRParser::Behaviour()
{
	return qdata.ActiveBehaviour;
}

Task_Type GRParser::Task()
{
	return qdata.Task;
}

Status_Type GRParser::Status()
{
	return qdata.Status;
}
	
void GRParser::DrawBall()
{
	if(BallSeen<=0)
		return;
	switch(bdata.Ball_Vision)
	{
	    case Seen:
		b.Draw(bdata.Pos, bdata.Speed, Gred);
	    	break;
	    case Reconstructed:
		b.Draw(bdata.Pos, bdata.Pos, Gmagenta);
		break;
	}
	BallSeen--;		
			
}

void GRParser::DrawVector()
{
	if(VectorSeen<=0)
		return;;
	::DrawVector(vdata.pos, vdata.draw, vdata.color); 	glPushMatrix();

	VectorSeen--;
}	

void GRParser::DrawTarget()
{   	
	if(TargetSeen<=0)
		return;
    t.Draw(tdata.Pos, tdata.Speed);
	TargetSeen--;
}

void GRParser::DrawObjects()
{
 	if(ObjectsSeen<=0)
 		return;
 	for(int i=0; i<HowManyObjects; i++)
 	{
   			GColor c;
    		switch(rdata.Robot[i].Type)
       	 	{
        		case TeamMate:
        			c=mycolor;
        			break;
        				
        		case Enemy:
        			c=opcolor;
        			break;
        				
        		case Unknown:
        			c=Gblack;
        	}
        	o.Draw(rdata.Robot[i].Pos, rdata.Robot[i].Speed, c); // usa lo stesso oggetto per disegnarli tutti
    }    				
    ObjectsSeen--;		
}

void GRParser::DrawMG()
{
	if(MGSeen<=0)
	    return;
	switch(gdata.MyGoalSeen)
	{
	  case Seen:	
	        Mp1.Draw(gdata.MyGoalPos1, 3, mygoalcolor);
  		Mp2.Draw(gdata.MyGoalPos2, 4, mygoalcolor);
  		
  		glPushMatrix();
  			switch(mygoalcolor)
  			{
  				case Gblue:
  					glColor3f(0.0, 0.0, 1.0);
  					break;
  				case Gyellow:
  					glColor3f(1.0, 1.0, 0.0);
  					break;
  			}			
  			glTranslatef(0.0, 0.0, GRZ_UP);
  			glBegin(GL_LINE_STRIP);
  				glVertex3f(gdata.MyGoalPos1.GetX(), gdata.MyGoalPos1.GetY(), 0.0);
  				glVertex3f(gdata.MyGoalPos2.GetX(), gdata.MyGoalPos2.GetY(), 0.0);
  			glEnd();
  			
  		glPopMatrix();
  				
  		break;
	}
	
	MGSeen--;
}

void GRParser::DrawOG()
{
	if(OGSeen<=0)
	    return;
	switch(gdata.EnemyGoalSeen)
	{
	  case Seen:
	  	Op1.Draw(gdata.EnemyGoalPos1, 3, opgoalcolor);
  		Op2.Draw(gdata.EnemyGoalPos2, 4, opgoalcolor);
  		
  		glPushMatrix();
  			switch(opgoalcolor)
  			{
  				case Gblue:
  					glColor3f(0.0, 0.0, 1.0);
  					break;
  				case Gyellow:
  					glColor3f(1.0, 1.0, 0.0);
  					break;
  			}			
  			glTranslatef(0.0, 0.0, GRZ_UP);
  			glBegin(GL_LINE_STRIP);
  				glVertex3f(gdata.EnemyGoalPos1.GetX(), gdata.EnemyGoalPos1.GetY(), 0.0);
  				glVertex3f(gdata.EnemyGoalPos2.GetX(), gdata.EnemyGoalPos2.GetY(), 0.0);
  			glEnd();
  			
  		glPopMatrix();
  				
  		break;
	}	
	OGSeen--;
}

void GRParser::DrawFWall()
{   	
	if(FW_WallSeen<=0)
		return;
	switch(wdata.FW_Seen)
	{
	  case Seen:
		fw.Draw();
		break;
	}	
	FW_WallSeen--;
}

void GRParser::DrawBWall()
{   	
	if(BW_WallSeen<=0)
		return;
	switch(wdata.BW_Seen)
	{
	  case Seen:
		bw.Draw();
		break;
	}	
	BW_WallSeen--;
}
	
void GRParser::DrawLWall()
{
	if(LW_WallSeen<=0)
		return;
	switch(wdata.LW_Seen)
	{
	  case Seen:
		lw.Draw();
		break;
	}	
	LW_WallSeen--;
}

void GRParser::DrawRWall()
{   	
	if(RW_WallSeen<=0)
		return;
	switch(wdata.RW_Seen)
	{
	  case Seen:
		rw.Draw();
		break;
	}	
	RW_WallSeen--;
}
