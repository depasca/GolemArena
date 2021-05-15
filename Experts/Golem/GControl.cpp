#include <math.h>
#include <stdio.h>
#include <iostream.h>

#include "GControl.h"
#include "Gconst.h"

//#define PERIOD 			20000.0               // micro-s
#define TIME 				(PERIOD/1000000)      // s
#define LINEAR_ACC 			1800.0
#define ANGULAR_ACC 		400.0
#define STOP_ANGULAR_ACC 	10.0
#define STOP_LINEAR_ACC		1.09
#define MAX_SPEED 			2000.0                  
#define MAX_JOG 			300.0
#define SPEED_SENSITIVITY 	0.005
#define JOG_SENSITIVITY 	0.005
#define P_GREEC 			3.14159265359


GControl::GControl(ETDispatch *pKernel,char *sDevName) : ETExpert(pKernel,PERIOD)
{

//       mysDevName=sDevName;

       myActualX=myActualY=myActualH=myNextX=myNextY=myNextH=0.0;
       myActualJog=myNewJog=myNextJog=myActualSpeed_myX=myNewSpeed_myX=myNextSpeed_myX=
                                      myActualSpeed_myY=myNewSpeed_myY=myNextSpeed_myY=0;
       myActualAngle=myNewAngle=myNextAngle=0.0;					      
       
       myLastJogOK = myLastSpeed_myXOK = myLastSpeed_myYOK = true;    
       mybLeftStalled=mybRightStalled=false;
       mybConnected=false;
       PosSetted = false;
       PERIOD = 20000.0;
       SUPERUSER=false;
}

GControl::~GControl()
{ 
    if (mybConnected) Close();
}

void GControl::SetId(int id)
{
    myId = id;
}    

void GControl::SU()
{
    SUPERUSER=true;
}

void GControl::ExitSU()
{
    SUPERUSER=false;
}        

bool GControl::SuperUser()
{
	return SUPERUSER;
}
	
void GControl::RightKick(int r)
{ 
}

void GControl::LeftKick(int l)
{ 
}

void GControl::SetSpeed_myX(int iSpeed)
{ 
//    cout <<"\nmi e' stata impostata la velocita' "<<iSpeed;
    myNewSpeed_myX = iSpeed; 
    if(iSpeed != myNextSpeed_myX)
	myLastSpeed_myXOK = false;
	
}

void GControl::SetSpeed_myY(int iSpeed)
{ 
//    cout <<"\nmi e' stata impostata la velocita' "<<iSpeed;
    myNewSpeed_myY = iSpeed; 
    if(iSpeed != myNextSpeed_myY)
	myLastSpeed_myYOK = false;
	
}

void GControl::SetSpeed(GVector iSpeed, float imyh)
{
    if(!SUPERUSER)
    {
//  	cout<<"Velocita' impostata "<<iSpeed.GetRo()<<endl;
		SetSpeed_myX(iSpeed.GetX());
		SetSpeed_myY(iSpeed.GetY());
		SetJog(imyh*180.0/M_PI);
    }
}

void GControl::SUSetSpeed(GVector iSpeed, float imyh)
{
	if(SUPERUSER)
	{
//  	cout<<"Velocita' impostata "<<iSpeed.GetRo()<<endl;
		SetSpeed_myX(iSpeed.GetX());
		SetSpeed_myY(iSpeed.GetY());
		SetJog(imyh*180.0/M_PI);
	}
}
	
void GControl::SetJog(int iJog)
{ 
//    cout <<"\nmi e' stato impostato il jog "<<iJog;
    myNewJog = iJog*180.0/M_PI;
    if(iJog != myNextJog)
        myLastJogOK = false;
}

void GControl::ResetSpeed()
{
    myActualSpeed_myX = myActualSpeed_myY = 0.0;
}

void GControl::ResetJog()
{
    myActualJog = 0.0;
}

void GControl::SetPos(double dX,double dY,double dH)
{ 
//       cout <<"\nmi e' stata impostata la pos (" <<dX<<","<<dY<<","<<dH<<")";
       myActualX=dX; myActualY=dY; myActualH=dH*180.0/M_PI;
       PosSetted = true;		
}

void GControl::SetPeriod(float p)
{
    PERIOD = p;
}    
double GControl::X()
{ 
       return myActualX; 
}

double GControl::Y()
{ 
       return myActualY; 
}

double GControl::H()
{ 
       return myActualH/180.0*M_PI;
}

Point GControl::nextPos()
{
       return Point(myNextX,myNextY,myNextH/180.0*M_PI);
}

int GControl::GetSpeed_myX()
{
	 return nearestInt(myActualSpeed_myX);
}

int GControl::GetSpeed_myY()
{
	 return nearestInt(myActualSpeed_myY);
}


void GControl::GetSpeed(GVector &iSpeed, float &imyh)
{
	iSpeed=GVector(myActualSpeed_myX, myActualSpeed_myY, Cartesian);
	imyh=myActualJog/180.0*M_PI;
}

void GControl::GetPos(GVector &iPos, float &iH)
{
	iPos=GVector(myActualX, myActualY, Cartesian);
	iH=myActualH/180.0*M_PI;
}
	
bool GControl::LeftStalled()
{
	return mybLeftStalled;
}
  
bool GControl::RightStalled()
{
	return mybRightStalled;
}
  
bool GControl::Connected()
{ 
       return mybConnected; 
}

void GControl::Init()
{
	printf("\n\n********Simulatore*********\n\n");
	printf("Robot connected on %s\n",mysDevName);
	fflush(stdout);
 	mybConnected=true;
	contacicli=0;
}





/**************************************************************************************************
				     Inizio DoYourDuty
**************************************************************************************************/

void GControl::DoYourDuty(int iCulo)
{

if(!PosSetted)
{
    myActualX = myNextX;
    myActualY = myNextY;  
    myActualH = myNextH;
    myActualSpeed_myX = myNextSpeed_myX; 
    myActualSpeed_myY = myNextSpeed_myY; 
    myActualAngle = myNextAngle;
    myActualJog = myNextJog;      
}


/***************************************   JOG , H  **********************************************/

    if (fabs(myNewJog) <= JOG_SENSITIVITY) SetJog(0);

    double  myAccel;

    if ( myActualJog!=myNewJog)   //sto per accelerare
    {
	if (myActualJog*myNewJog <= 0)
	{
	     if(myActualJog==0)
		myAccel = copysign(2.5*ANGULAR_ACC,myNewJog);
	    else	 
		myAccel = copysign(STOP_ANGULAR_ACC*fabs(myActualJog),myNewJog);

	}
	else
	{
	    if(myActualJog > 0)
		myAccel = (myActualJog < myNewJog) ? ANGULAR_ACC : -STOP_ANGULAR_ACC;
	    else
		myAccel = (myActualJog > myNewJog) ? -ANGULAR_ACC : STOP_ANGULAR_ACC;    
	}
	myNextJog = IntegrateVel(myActualJog,myAccel,TIME);
	myNextJog = ( fabs(myNextJog) < fabs(myNewJog) ) ? myNextJog : myNewJog ;
	myNextJog = ( fabs(myNextJog) < MAX_JOG ) ? myNextJog : copysign(MAX_JOG,myNextJog) ;
	if(fabs(myNextJog) <= JOG_SENSITIVITY) myNextJog = 0;    
  
    }
   else
    {	
	myAccel = 0;
	myNextJog = myNewJog;
	myLastJogOK = true;
    }

    myNextH = IntegratePos(myActualH,myActualJog,myAccel,TIME);
    float absmyNextH = fabs(myNextH);
    float rounds=absmyNextH/360;
    absmyNextH = absmyNextH-absmyNextH*floor(rounds);  
    myNextH = copysign(absmyNextH,myNextH);

/************************************** Fine Jog ,H **********************************************/




/***************************************  Speed , X, Y  ******************************************/

/* 
    if(myNewSpeed_myX==0 && myNewSpeed_myY==0)
	myNewAngle = 0.0;
    else	 
	myNewAngle = atan2(myNewSpeed_myY,myNewSpeed_myX);
    if(myNextSpeed_myX==0 && myNextSpeed_myY==0)
	myNextAngle = 0.0;
    else	 
	myNextAngle = atan2(myNextSpeed_myY,myNextSpeed_myX);
    double faa = fabs(myActualAngle);
    double fnewa = fabs(myNewAngle);
    double fnexta = fabs(myNextAngle);
    double myActualSpeed, myNewSpeed, myNextSpeed;
    if(faa>15.0)
	myActualSpeed = (myActualSpeed_myX/cos(myActualAngle*P_GREEC/180));
    else
	myActualSpeed = (myActualSpeed_myY/sin(myActualAngle*P_GREEC/180));
    if(fnewa>15.0)
	myNewSpeed = (myNewSpeed_myX/cos(myNewAngle*P_GREEC/180));
    else
	myNewSpeed = (myNewSpeed_myY/sin(myNewAngle*P_GREEC/180));	
    if(fnexta>15.0)
	myNextSpeed = (myNextSpeed_myX/cos(myNextAngle*P_GREEC/180));
    else
	myNextSpeed = (myNextSpeed_myY/sin(myNextAngle*P_GREEC/180));				    
*/
    double myAccelX,myAccelY;
    
    
    double myLinAccel_myX;
    double myActualSpeedX = myActualSpeed_myX * cos(myActualH*P_GREEC/180)+
                            myActualSpeed_myY * cos((90.0+myActualH)*P_GREEC/180);
    double myActualSpeedY = myActualSpeed_myX * sin(myActualH*P_GREEC/180)+
                            myActualSpeed_myY * sin((90.0+myActualH)*P_GREEC/180);
    if ( fabs(myNewSpeed_myX) < SPEED_SENSITIVITY ) 
	SetSpeed_myX(0);
    if ( myActualSpeed_myX!=myNewSpeed_myX )  //sto per accelerare
    {
		    
        if(myActualSpeed_myX*myNewSpeed_myX <= 0)
	{
	    if(myActualSpeed_myX==0)
		myLinAccel_myX = copysign(2.5*LINEAR_ACC,myNewSpeed_myX);
	    else	 
		//myLinAccel_myX = copysign(STOP_LINEAR_ACC*fabs(myActualSpeed_myX),myNewSpeed_myX);

        {
	        float k=((fabs(myNewSpeed_myX)<100)&&(fabs(myActualSpeed_myX)<100)) ? 6 : 1;
			myLinAccel_myX = copysign((LINEAR_ACC/k),myNewSpeed_myX);
                
		}
	
      }

	else
	{
	    if(myActualSpeed_myX > 0)
		myLinAccel_myX = (myActualSpeed_myX < myNewSpeed_myX) ? LINEAR_ACC : -STOP_LINEAR_ACC;
	    else
		myLinAccel_myX = (myActualSpeed_myX > myNewSpeed_myX) ? -LINEAR_ACC : STOP_LINEAR_ACC;    
	}
	myNextSpeed_myX = IntegrateVel (myActualSpeed_myX,myLinAccel_myX,TIME);
     	myNextSpeed_myX = ( fabs(myNextSpeed_myX) < fabs(myNewSpeed_myX)) ? myNextSpeed_myX : myNewSpeed_myX ;
      	myNextSpeed_myX = ( fabs(myNextSpeed_myX) < MAX_SPEED ) ? myNextSpeed_myX : copysign(MAX_SPEED,myNextSpeed_myX) ;
	if (fabs(myNextSpeed_myX) < SPEED_SENSITIVITY)
	    myNextSpeed_myX = 0;
    	
    }	
    else
    {	
	myLastSpeed_myXOK = true;
	myNextSpeed_myX = myNewSpeed_myX;
	myLinAccel_myX = 0; 

    }
    
    
/*************************************/    
    
    
    double myLinAccel_myY;
    if ( fabs(myNewSpeed_myY) < SPEED_SENSITIVITY ) 
	SetSpeed_myY(0);
    if ( myActualSpeed_myY!=myNewSpeed_myY )  //sto per accelerare
    {
		    
        if(myActualSpeed_myY*myNewSpeed_myY <= 0)
	{
	    if(myActualSpeed_myY==0)
                {  
		myLinAccel_myY = copysign(2.5*LINEAR_ACC,myNewSpeed_myY);            
	        } 
	    else	 
		//	myLinAccel_myY = copysign(STOP_LINEAR_ACC*fabs(myActualSpeed_myY),myNewSpeed_myY);

        {
	        float k=((fabs(myNewSpeed_myY)<100)&&(fabs(myActualSpeed_myY)<100)) ? 6 : 1;
			myLinAccel_myY = copysign((LINEAR_ACC/k),myNewSpeed_myY);
        }

	}

	else
	{
	    if(myActualSpeed_myY > 0)
		myLinAccel_myY = (myActualSpeed_myY < myNewSpeed_myY) ? LINEAR_ACC : -STOP_LINEAR_ACC;
	    else
		myLinAccel_myY = (myActualSpeed_myY > myNewSpeed_myY) ? -LINEAR_ACC : STOP_LINEAR_ACC;    
	}
	myNextSpeed_myY = IntegrateVel (myActualSpeed_myY,myLinAccel_myY,TIME);
     	myNextSpeed_myY = ( fabs(myNextSpeed_myY) < fabs(myNewSpeed_myY)) ? myNextSpeed_myY : myNewSpeed_myY ;
      	myNextSpeed_myY = ( fabs(myNextSpeed_myY) < MAX_SPEED ) ? myNextSpeed_myY : copysign(MAX_SPEED,myNextSpeed_myY);
	if (fabs(myNextSpeed_myY) < SPEED_SENSITIVITY)
	    myNextSpeed_myY = 0;
    	
    }	
    else
    {	
	myLastSpeed_myYOK = true;
	myNextSpeed_myY = myNewSpeed_myY;
	myLinAccel_myY  = 0; 

    }    
    
    
    
    
    
    
/************************************/    
    
    
    
    
    
    myNextAngle = atan2(myNextSpeed_myY, myNextSpeed_myX);
    myAccelX = myLinAccel_myX*cos(myActualH*P_GREEC/180)+
               myLinAccel_myY*cos((90.0+myActualH)*P_GREEC/180);
    myAccelY = myLinAccel_myX*sin(myActualH*P_GREEC/180)+
               myLinAccel_myY*sin((90.0+myActualH)*P_GREEC/180);
    myNextX = IntegratePos(myActualX,myActualSpeedX,myAccelX,TIME);
    myNextY = IntegratePos(myActualY,myActualSpeedY,myAccelY,TIME);	 
	

/*    if (fabs(myNextSpeed_myY)<10) 
     {
      cout<<"My next Speed "<<myNextSpeed_myY<<endl;
      cout<<"My New Speed "<<myNewSpeed_myY<<endl;
      cout<<"My Actual accel "<<myAccelY<<endl;
     }
*/    
/*************************************** Fine Speed X Y ******************************************/
    
    
    
 
    contacicli++; 

    PosSetted = false;		


    return;

}


/**************************************************************************************************
	                               Fine DoYourDuty
**************************************************************************************************/




double GControl::IntegratePos(double initialPos,double speed,double accel,double time )
{
    return initialPos + speed*time + accel*time*time/2;
}    	


double GControl::IntegrateVel(double initialVel,double accel,double time)
{
	return (initialVel + accel*time);
}

void GControl::Close()
{
	if (!mybConnected) return;
        mybConnected=false;
	printf("Disconnecting from robot server.\n");
	fflush(stdout);
}

int GControl::nearestInt(double ris)
{
        int int_inf = (int)floor(ris);  // troncamento a intero inf.
	int int_sup = (int)ceil(ris);   // troncamento a intero sup. 
	return ( fabs(ris-int_inf) <= fabs(ris-int_sup) ) ? int_inf : int_sup;
}

 















