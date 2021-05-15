


#ifndef __GOLEM_ET_EXPERT_H__
#define __GOLEM_ET_EXPERT_H__

#include "etexpert.h"
#include "geometry.h"
#include "GVector.h"
#include "stdio.h"

//#include "drawvector.h"


class GControl : public ETExpert
{
public :

	GControl(ETDispatch *pKernel,char *sDevName="/dev/cua0");	
	~GControl();

	void Init();
	void DoYourDuty(int=0);
	void Close();
	void SetId(int=0);
	void SetSpeed_myX(int iSpeed); 
	void SetSpeed_myY(int iSpeed);
	void inline SetSpeed(int sx, int sy, int sh)
	{
		SetSpeed_myX(sx);
	    SetSpeed_myY(sy);
	    SetJog (sh);
	}
	void SetSpeed(GVector iSpeed, float imyh);
	void SUSetSpeed(GVector iSpeed, float imyh);
	void SetJog(int iJog);
	void SetPos(double dX=0.0,double dY=0.0,double dH=0.0);
	void SetPeriod(float period);
	void SU();
	void ExitSU();
	bool SuperUser();
	double X();
	double Y();
	double H();
	Point nextPos();
	void ResetSpeed();
	void ResetJog();
//	void inline ResetSpeed(){myActualSpeed_myX=myActualSpeed_myY=myActualJog=0.0;}
	void inline Stop() {SetSpeed(0,0,0);}
	int GetSpeed_myX();
	int GetSpeed_myY();
	void GetPos(GVector &iPos, float &imyh);
	void GetSpeed(GVector &iSpeed, float &imyh);
	int GetJog();
	int GetId() {return myId;}
	float GetPeriod(){return PERIOD;};
	bool LeftStalled();   
	bool RightStalled();  
	bool Connected();
	void RightKick(int r=9);
	void LeftKick(int l=9);
	void Kick(int i)
	{
/*		GVector v(0.5, 0.0);
		GVector w(myActualX/1000.0, myActualY/1000.0+0.1);
		DrawVector(w, v);
*/
		printf("\n Kick ");	
	}	

protected :

	char *mysDevName;
	int myId;
	
	bool mybConnected;
	bool myLastJogOK;
 	bool myLastSpeed_myXOK;double myActualX,myActualY,myActualH;
	bool myLastSpeed_myYOK;double myNextX,myNextY,myNextH;
	bool PosSetted;
	double myActualJog,myNewJog,myNextJog,
	       myActualSpeed_myX,myNewSpeed_myX,myNextSpeed_myX,
	       myActualSpeed_myY,myNewSpeed_myY,myNextSpeed_myY;
	double myActualAngle, myNewAngle, myNextAngle;					      

	bool mybLeftStalled,mybRightStalled;	
	

private :
	float PERIOD;
	bool SUPERUSER;
	int contacicli;
	double IntegratePos(double initialPos,double speed,double acceleration,double time);
	double IntegrateVel(double initialVel,double acceleration ,double time);
	int nearestInt(double number);
};

#endif // !__GOLEM_ET_EXPERT_H__






