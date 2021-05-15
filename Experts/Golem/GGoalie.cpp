
#include <iostream.h>

#include "GGoalie.h"
#include "GBehaviour.h"

//#define __GGOALIE_DEBUG__
//#define __DRAW_VECTORS__

#define MIN_WEIGHT_CHANGE 0.0
#define MIN_BALL_WEIGHT 1300.0
#define MAX_BALL_WEIGHT 2000.0
#define MAX_BALL_WEIGHT2 500.0
#define CATCH_BALL_WEIGHT 2500.0
#define DELTA_MOD 1000.0
#define MIN_GOALPOST_WEIGHT 350.0
#define MAX_GOALPOST_WEIGHT 1000.0
#define MAX_GOALIE_SPEED 5000.0

bool GOALIE_GO_DIRECT=true;

GGoalie::GGoalie(GControl *GGCtrl, GMemHandler *GGMem, unsigned short GGTA):GBehaviour(GGCtrl, GGMem, GGTA)
{
	myActualPos=0;
	myActualH=0;
	myTargetnew=myTarget=GVector(0.0, 0.0);
	GP1weightnew=GP2weightnew=GP1weight=GP2weight=Bweightnew=Bweight=GVector(0.0, 0.0);
	#ifdef __GGOALIE_DEBUG__
	cout << "GGoalie::GGoalie()" << endl;
	#endif
}

GGoalie::~GGoalie()
{
	cout << "GGoalie::~GGoalie()" << endl;
}




ETMessage* GGoalie::Run()
{

	if(Goals.MyGoalSeen==Unseen)
    {
    	cout << "GGoalie: ***** Non vedo i MIEI PALI" << endl;
    	myGControl->SetSpeed(0,0);
    	return NULL; //c'e' qualcosa di piu' furbo da fare ?
    }
	if(Ball.Ball_Vision==Unseen)
    {
    	cout << "GGoalie: Non vedo LA PALLA" << endl;
    	myGControl->SetSpeed(0,0);
    	return NULL; //sara' meglio fare qualcosa di piu' furbo...
    }
    GVector absPos;
	float absH;
	myGControl->GetPos(absPos , absH);
	
	GVector P1toP2=Goals.MyGoalPos2-Goals.MyGoalPos1;
	
	/* aggiorno i vettori-peso dei pali */
	//GVector Delta(1000.0, M_PI/2.0);
	GVector Delta1(DELTA_MOD, Goals.MyGoalPos1.GetTheta());  // non li
	GVector Delta2(DELTA_MOD, Goals.MyGoalPos2.GetTheta());  // uso
	float GoalLineAngle=P1toP2.GetTheta();
	if(GoalLineAngle<0.0)
		GoalLineAngle=2.0*M_PI+GoalLineAngle; // impongo il range 0-2M_PI
	float BallDirection=Ball.Pos.GetTheta();
	if(BallDirection<0.0)
		BallDirection=2.0*M_PI+BallDirection; // impongo il range 0-2M_PI
	GP1weight=Goals.MyGoalPos1;
	GP2weight=Goals.MyGoalPos2;
	
	/* aggiorno il vettore-peso della palla */
	Bweight=Ball.Pos;
	GVector BSpeed=Ball.Speed;
	BSpeed.Rotate(M_PI);
	GVector Bweight2=Ball.Pos.Proiezione(P1toP2.GetTheta());
	if(Bweight2.GetRo()>MAX_BALL_WEIGHT2)
		Bweight2.SetRo(MAX_BALL_WEIGHT2);
	//Bweightnew=Bweightnew+0.1*BSpeed;    // per ora scelgo di trascurare la velocita' della palla
	
	/* limito il peso della palla */
	if(Bweight.GetRo()>MAX_BALL_WEIGHT)
		Bweight.SetRo(MAX_BALL_WEIGHT);
	
	
	if(Bweight.GetRo()<MIN_BALL_WEIGHT)
		Bweight.SetRo(CATCH_BALL_WEIGHT);
	
	/* limito il peso del palo sinistro */
	if(GP1weight.GetRo()>MAX_GOALPOST_WEIGHT)
		if(GP2weight.GetRo()<=MAX_GOALPOST_WEIGHT)
			GP1weight.SetRo(MAX_GOALPOST_WEIGHT);	
	
/*	if(GP1weight.GetRo()<MIN_GOALPOST_WEIGHT)
	    GP1weight=GVector(-GP1weight.GetX(), GP1weight.GetY(), Cartesian);
*/	
	/* limito il peso del palo destro */
	if(GP2weight.GetRo()>MAX_GOALPOST_WEIGHT)
		if(GP1weight.GetRo()<=MAX_GOALPOST_WEIGHT)
			GP2weight.SetRo(MAX_GOALPOST_WEIGHT); 	
 	
/*	if(GP2weight.GetRo()<MIN_GOALPOST_WEIGHT)
	    GP2weight=GVector(-GP2weight.GetX(), GP2weight.GetY(), Cartesian);
*/	
	/* disegno i tre vettori */
/*	gaDrawVector(absPos, GP1weight, Gcyan);
	gaDrawVector(absPos, GP2weight, Gmagenta);
	gaDrawVector(absPos, Bweight, Gred);
	gaDrawVector(absPos, Bweight2, Gred);
*/
	//gaDrawVector(absPos+Goals.MyGoalPos1, P1toP2, Gwhite);
	
	/* calcolo il baricentro dei vettori precedenti */
	GVector baricentro=Bweight+GP1weight+GP2weight;
	
	/* aggiorno il target */
	myTargetnew=baricentro;	
    GVector Tdiff=myTargetnew-myTarget;
	if(Tdiff.GetRo()>MIN_WEIGHT_CHANGE)
	{
//		printf("\nGGoalie: Target changed ");
		myTarget=myTargetnew;
		if(myTarget.GetRo() <= GGOALIE_INERZIA)
			myTarget.SetRo(0);
	}

	ts.Pos=myTarget;

//	ts.Speed=Ball.Speed;
	ts.AvoidBall=false;
	ts.Style=BestGuess;
//	ts.Heading=Ball.Speed.GetTheta();

	if(GOALIE_GO_DIRECT)
	{
		if(myTarget.GetRo()>500.0)
			myGControl->SetSpeed(GVector(MAX_GOALIE_SPEED, myTarget.GetTheta()), 0.0);
		else
			myGControl->SetSpeed(myTarget,0.0);
	}	
	else
	{
		WriteTarget(ts);
		mess = new ETMessage (0, GActivateG3dof);
		return mess;
	}
}