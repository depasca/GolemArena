
// G3dof.h
//
// Header dell'esperto G3dof (movimento e obstacle avoidance)

#ifndef __G3dof_H__
#define __G3dof_H__

#include "etexpert.h"
#include "GControl.h"
#include "Gconst.h"
#include "GMemHandler.h"
#include "GVector.h"

//#include "GMath.h"


//definirla solo durante il debug
#define G3DOF_DEBUG


//massimo numero di iterazioni per l'inseguitore d'angolo
#define MAXCYCLE                        100

//raggio degli ostacoli
#define OBJRADIUS			315.0
//raggio di sicurezza per lisciare il pelo agli ostacoli
#define SAFERADIUS			1000.0
//numero di ostacoli che considero effettivamente per i conti
#define MAXOBJNUM			7
//raggio della mia zona di interdizione
#define MYRADIUS			315.0
//minima distanza entro la quale considero sempre gli ostacoli
#define MINDIST			        2000.0
//parametri fisici del robot
#define MAXVEL				2000.0
#define MAXACC				1000.0
//distanza dal target alla quale comincio a rallentare 
#define SAFESTOP			1000.0
//in realta' MAXACC e' funzione della velocita' attuale (calcolarla...)
//massimo angolo di scostamento dalla traiettoria desiderata entro il quale considero gli ostacoli (60 gradi in radianti)
#define MAXANGLE			1.6
//1.0471975512	

#define MAXODOMETRIA 1000

#define MINWALLDIST 1000

#define RUOTDIST 1200
 

class G3dof : public ETExpert

{
  private:

	int NumObj;		//num. ostacoli generico
	int ObstacleNum;	//num. ostacoli interferenti
	GVector myActualSpeed; //velocita' lette
	float myActualJog;
	GVector Speed; // velocita' selezionate
	float Jog;

	GVector DesiredSpeed;
	int Pivot;
	int UltimoRompiballeSx, UltimoRompiballeDx;
	GVector DetourSX;
	GVector DetourDX;
	GVector PseudoSpeed; //vettore pseudodefinitivo: devo solo controllare i muri a questo punto
        GVector LastSpeed;
	
	struct ObjZone
		{
		GVector Pos;
		GVector ApproachingSpeed;
		float R;
		GVector CollisionPoint;
                bool NORMAL;
		} IZone[MAXOBJNUM];
	
	int Target_Area;
	int Obstacle_Area;
	int Wall_Area;
        int Ball_Area; 

	Target_struct *Target;
	All_Robots_struct *ObjSeen;
	Walls_struct *Walls;
 	Balloon_struct *Ball; 	

	GVector Interceptor(GVector, GVector);
	void Ordinator();
	void SelectPivot();
	
 public:

   	G3dof (ETDispatch *pKernel, GControl *pGolem, GMemHandler *pMem, 
		int TargetAreaHandle, int Wall_Area_Handle,
		int Obstacle_Area_Handle, int Ball_Handle);
   
   	~G3dof();
   	void Init();
   	void DoYourDuty (int =0);
   	void Close() {}

 protected:
   	GControl *myGolem;
   	GMemHandler *myMem;	
};

#endif








