#ifndef __GBEHAVIOUR_H__
#define __GBEHAVIOUR_H__

#include <iostream.h>
#include <math.h> 
#include <stdlib.h>
#include <stdio.h>
#include <etmsg.h>
#include "GMemHandler.h"
#include "GControl.h"
#include "Gconst.h"
#include "GVector.h"

/*---------------------------------------------------------------------------

    Classe astratta: ogni behaviour deve ereditarla e ridefinire il metodo
    Run(), oltre al distruttore.
    
-----------------------------------------------------------------------------*/

class GBehaviour
{

 protected:

 	GVector myActualSpeed;
  	float myActualJog;
  	Balloon_struct Ball;
  	int NumRobot;	
  	All_Robots_struct Robot;
  	Goals_struct Goals;
  	Walls_struct Walls;
  	unsigned short TargetArea;
  	Task_Type TASK;
  	Status_Type STATUS;
  	unsigned short TM_Num;
  
  	GControl *myGControl;
  	GMemHandler *myMem;
  
 public:
  
   	GBehaviour::GBehaviour(GControl *GCtrl, GMemHandler *MMem, unsigned short TA)
    {
    	printf("\nGBehaviour creato ");
    	myGControl=GCtrl;
      	myMem=MMem;
      	TargetArea=TA;
    }  
  
  	virtual  ~GBehaviour() {};
  	virtual  ETMessage* Run()=0;
  
  
  
  
/*---------------------------------------------------------------------

Funzioni ridefinite per evitare di scrivere delle cose lunghe...

------------------------------------------------------------------------*/
  
  	void SetSpeed(const GVector &a, const float &b)
    {
    	myGControl->SetSpeed(a,b);
    }
            
  	int WriteTarget(Target_struct &a)
    {
    	int k=myMem->WriteMemArea(TargetArea,&a); // verificare
      	return k;
    }  

  	void Update (const GVector &v,
                 const float &j,
                 const Balloon_struct &Bs,
                 const int &n,
                 const All_Robots_struct Rp,
                 const Goals_struct &g,
                 const Walls_struct &w,
                 const Task_Type t,
	             const Status_Type s,
	             const unsigned short &tm_num)
	{
 		myActualSpeed=v;
		myActualJog=j;
        Ball=Bs;
		NumRobot=n;
        Robot=Rp;
		Goals=g;
        Walls=w;
		TASK=t;
		STATUS=s;
		TM_Num=tm_num;
	}


};




/*---------------------------------------------------------------------

    Questa classe serve per aggiornare i dati statici comuni a tutti
    i behaviour che ereditano la classe precedete
    
-----------------------------------------------------------------------*/



class GBehaviourDataUpdater : public GBehaviour
{
    private: 
		ETMessage* Run()
	  	{
	  		return NULL;
	  	}

    public:
     
    	GBehaviourDataUpdater(GControl *GCtrl, GMemHandler *MMem, unsigned short TA):GBehaviour( GCtrl, MMem, TA)
     	{
     		cout<<"Updater creato!"<<endl;
     	}

    	~GBehaviourDataUpdater() {}

    	ETMessage* Run(const GVector &v,
    	               const float &j,
    	               const Balloon_struct &Bs,
                       const int &n,
                       const All_Robots_struct Rp,
                       const Goals_struct &g,
                       const Walls_struct &w,
                       const Task_Type t,
		  	           const Status_Type s)
		{
    		GBehaviour::myActualSpeed=v;
			GBehaviour::myActualJog=j;
    		GBehaviour::Ball=Bs;
			GBehaviour::NumRobot=n;
    		GBehaviour::Robot=Rp;
			GBehaviour::Goals=g;
    		GBehaviour::Walls=w;
			GBehaviour::TASK=t;
			GBehaviour::STATUS=s;
        	return NULL;
		}

};


#endif
