#ifndef _GCONST_H
#define _GCONST_H

// Versione dell'1 Agosto

//#include "etexpert.h"

#include "GVector.h"

#define MAXOBJSEEN	20

#define GActivateG3dof 10
/* colori */
typedef enum {Gblack, Gwhite, Gblue, Gcyan, Gyellow, Gred, Glowred, Gmagenta, Ggreen}GColor;

// Messaggi per le comunicazioni col monitor -------------------
// Spediti
#define GMSG_Ball 21
#define GMSG_Obstacle 22
#define GMSG_Goal 23
#define GMSG_Wall 24
#define GMSG_Target 27
#define GMSG_Status 28
#define GMSG_DrawVector 29
// Ricevuti
#define GMSG_StartStop 25
 
// Messaggio di coordinamento ----------------------------------

#define GMSG_Q 26

#define GPERIODO 100000


// enum varie --------------------------------------------------

/* MSG_STARTSTOP
   -------------

Type:	0 - Posizionati con possesso di palla
	1 - Posizionati senza possesso palla
	2 - Stop (sul posto)
	3 - Start (immediato)

***********************************************************/
enum MsgStartStop_Type
    {
	SetupAttackMsg,
	SetupDefendMsg,
	StopMsg,
	StartMsg
    };

enum Robot_Type
{
    TeamMate,
    Enemy,
    Unknown
};

enum GBehaviour_Type
{
    SearchBall,
    GoToBall,
    CarryBall,
    NearWall,
    CatchBall,
    Defend,
    Wingman,
    AllBack,
    Stopper,    
    Goalie
};

enum Goal_Type
{
    Blue,
    Yellow
};

enum Vision_Type
{
Seen,
Unseen,
Reconstructed
};


enum Task_Type
{
    RoleG,
    Role1,
    Role2,
    Role3
};

enum Status_Type
{
    PlayerStopped,
    EnemyHasBall,
    FreeBall,
    BallMine,
    Danger,
	Quiet
};

enum Trool
{
    Left,
    Right,
    BestGuess
};

// Strutture contenute nei Messaggi --------------------------------------
// tutti quelli di attivazione sono vuoti

struct Target_struct
{
    GVector Pos; 		//relativa al golem
    GVector Speed;		//realtiva al golem supposto fermo
    float  Heading;		//direzione di approccio desiderata
    int Priority;		//legato al task
    bool AvoidBall;             // se e' 1 piazzo un bellissimo ostacolo sotto la palla cosi' non la urto   
    Trool Style;
};

struct Obstacle_Header_struct
{
    int Mate_Seen, Enemy_Seen, Unknown_Seen;
};

struct Robot_struct {
    int ID;
    GVector Pos;
    GVector Speed;
    Robot_Type Type;
};

struct All_Robots_struct {
	Obstacle_Header_struct Header;
	Robot_struct Robot[MAXOBJSEEN];
    double TimeStamp;		
    GVector PointOfView;
};

struct Balloon_struct
{
    Vision_Type Ball_Vision;
    GVector Pos;
    GVector Speed;
    double TimeStamp;
    GVector PointOfView;
};

struct Goals_struct
{
    Vision_Type MyGoalSeen;
    Vision_Type EnemyGoalSeen;
    GVector MyGoalPos1;
    GVector MyGoalPos2;
    double MyGoalTimeStamp;
    GVector MyGoalPointOfView;
    GVector EnemyGoalPos1;
    GVector EnemyGoalPos2;
    double EnemyGoalTimeStamp;
    GVector EnemyGoalPointOfView;
};

struct Walls_struct
{
    Vision_Type FW_Seen, BW_Seen, RW_Seen, LW_Seen;  //F=front, B=back, R=right, L=guess it...

    double WallTimeStamp;
    GVector WallPointOfView;
    
    GVector FW;		
    GVector LW;		
    GVector RW;		
    GVector BW;		
    
     
};

struct MsgStartStop_struct
{
    MsgStartStop_Type	Type;
    char Mask;    // 255 -> vale per tutti, altrimenti e' l'IP del destinatario
};

struct MsgQ_struct
{
    GBehaviour_Type ActiveBehaviour;
    float Q;
    Task_Type Task;
    Status_Type Status;
    double TimeStamp;
};

struct Golem_status
{
float battery;
bool engineson;
float gas;
};

struct MsgVector
{
	GColor color;
	GVector pos;
	GVector draw;
};
		
#endif
