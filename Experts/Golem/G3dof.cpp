// G3dof.cpp
//
// Esperto per la navigazione sicura
//
//

#include "GControl.h"
#include "etexpert.h"
#include <math.h>
#include <iostream.h>
#include <stdio.h>
#include "grobjects.h"
#include "G3dof.h"
#include "GVector.h"
#include "Gconst.h"
#include "GMemHandler.h"

 


// ------------ Costruttore

G3dof::G3dof (ETDispatch *pKernel, GControl *pGolem, GMemHandler *pMem, 
		int Target_Area_Handle, int Wall_Area_Handle,
		int Obstacle_Area_Handle, int Ball_Handle) : ETExpert (pKernel,100000)

{
	Target_Area = Target_Area_Handle;
	Obstacle_Area = Obstacle_Area_Handle;
	Wall_Area = Wall_Area_Handle;
	Ball_Area= 
	Ball_Handle;	
	myActualSpeed = GVector(0.0, 0.0, Cartesian);
	myActualJog=0.0;
	
  	myGolem = pGolem;
  	myMem = pMem;



}


//--------Distruttore


G3dof::~G3dof () 

{
delete Target;
delete Walls;
delete[] ObjSeen;
}


// -------------------- Init

void G3dof::Init ()
{
	AddLocalRequest(GActivateG3dof);
	SetActivationCondition(GetAllMsgTypeMask());
    Target= new Target_struct;
	Walls= new Walls_struct;
    Ball = new Balloon_struct;
	ObjSeen = new All_Robots_struct;
}



// --------------------------DoYourDuty

void G3dof::DoYourDuty (int iST)
{
       if ((!myGolem->Connected())&&(!iST))
 		return;

	const ETMessage *MsgReceived;
    	MsgReceived=NULL;
	
   	while(MsgToBeRead()) MsgReceived=GetNextMsg();

    // leggo i dati dalla zona di memoria condivisa
        
	int OKREAD;

	OKREAD = myMem->ReadMemArea (Target_Area, Target);

	if (OKREAD==-1) 
	    {
	    cout << "\nG3dof: Errore in lettura mem condivisa: Target";
	    fflush(stdout);
	    return;
	    }
		
        if (Target->AvoidBall)
	   {
	    OKREAD = myMem->ReadMemArea (Ball_Area, Ball);
  	    if (OKREAD==-1) 
	     {
	      cout << "\nG3dof: Errore in lettura mem condivisa: Ball";
	      fflush(stdout);
	      return;
	     }
	   }    

	OKREAD = myMem->ReadMemArea (Obstacle_Area, ObjSeen);

	if (OKREAD==-1) 
	    {
	    cout << "\nG3dof: Errore in lettura mem condivisa: ObjSeen";
	    fflush(stdout);
	    return;
	    }
	
	int en,am,bo;
	en=ObjSeen->Header.Enemy_Seen;
	bo=ObjSeen->Header.Unknown_Seen;
	am=ObjSeen->Header.Mate_Seen;
	NumObj = en+bo+(Target->Priority!=1)*am;	

	    
	OKREAD = myMem->ReadMemArea (Wall_Area, Walls);

	if (OKREAD==-1) 
	    {
	    cout << "\nG3dof: Errore in lettura mem condivisa: Walls_seen";
	    fflush(stdout);
	    return;
	    }

    // calcolo il vettore agognato, ovvero la direzione lungo la quale
    // approcciare il target (in assenza di ostacoli)
	myGolem->GetSpeed(myActualSpeed, myActualJog);

         	DesiredSpeed = Interceptor(Target->Pos, Target->Speed);

	bool IN_RANGE=false, IN_SIGHT=false, APPROACHING=false, COLLIDING=false;
	GVector ApproachingSpeed;

 	ObstacleNum=0;

	GVector flee1, flee2;

	for(int i=0; i < NumObj; i++)
	  {
	    
	  IN_SIGHT=((fabs(ObjSeen->Robot[i].Pos.GetTheta()-((myActualSpeed+LastSpeed).GetTheta()))) < MAXANGLE);  //considero se il robot rientra in un certo angolo di fastidio
	    
	  ApproachingSpeed=ObjSeen->Robot[i].Speed.Proiezione((-ObjSeen->Robot[i].Pos).GetTheta()); //velocita' di avvicinamento del robot a me
	    
	  APPROACHING=(ApproachingSpeed.GetTheta()!=ObjSeen->Robot[i].Pos.GetTheta()); //booleano che mi dice se il robot si avvicina o allontana con quella velocita'
	    
	  IN_RANGE=(((ObjSeen->Robot[i].Pos.GetRo() - APPROACHING*ApproachingSpeed.GetRo()) < MINDIST )); //mi dice se lo devo considerare, valutando la sua velocita'
	    

		if ( (ObjSeen->Robot[i].Type!=TeamMate || Target->Priority!=1)  && IN_RANGE && IN_SIGHT )
                    {
		     ObstacleNum++;
		     IZone[i].Pos = ObjSeen->Robot[i].Pos;	
                     IZone[i].ApproachingSpeed =ObjSeen->Robot[i].Speed;
		     IZone[i].R = OBJRADIUS;
		    }
		if( (ObjSeen->Robot[i].Type==TeamMate) && (Target->Priority!=1) && (!IN_SIGHT) && (APPROACHING))	
		    {
		    //verifico se un compagno mi vuole passare attraverso...
		    float Security_Angle = atan(SAFERADIUS/ObjSeen->Robot[i].Pos.GetRo());
		    if(fabs(ObjSeen->Robot[i].Pos.GetTheta()+(ObjSeen->Robot[i].Speed.GetTheta()) < Security_Angle))
			{
			//soncazzi : modifico DesiredSpeed in modo da scansarmi dalla sua traiettoria
			flee1 = ObjSeen->Robot[i].Speed;
			flee2 = ObjSeen->Robot[i].Speed;
			flee1.Rotate(M_PI/2.0);
			flee2.Rotate(-M_PI/2.0);
			DesiredSpeed = (fabs((flee1-DesiredSpeed).GetTheta()) < fabs((flee2-DesiredSpeed).GetTheta())) ? flee1 : flee2;
                        cout<<"VIA! "<<i<<Target->Priority<<endl;
			}
		    }    
	  } // a questo punto ho in IZone[] tutti gli ostacoli da considerare e in ObstacleNum il numero di ostacoli da considerare


         if ((Target->AvoidBall)&&(Ball->Ball_Vision!=Unseen))
	  {
	  IN_SIGHT=((fabs(Ball->Pos.GetTheta()-myActualSpeed.GetTheta())) < MAXANGLE);  //considero se il robot rientra in un certo angolo di fastidio
	    
	  ApproachingSpeed=Ball->Speed.Proiezione((-Ball->Pos).GetTheta()); //velocita' di avvicinamento del robot a me
	    
	  APPROACHING=(ApproachingSpeed.GetTheta()!=Ball->Pos.GetTheta()); //booleano che mi dice se il robot si avvicina o allontana con quella velocita'
	    
	  IN_RANGE=(((Ball->Pos.GetRo() - APPROACHING*ApproachingSpeed.GetRo()) < MINDIST )); //mi dice se lo devo considerare, valutando la sua velocita'
          if (IN_SIGHT&&IN_RANGE)
	   {
	    IZone[ObstacleNum].Pos=Ball->Pos;
	    IZone[ObstacleNum].ApproachingSpeed=Ball->Speed;
	    IZone[ObstacleNum].R=OBJRADIUS;
	    ObstacleNum++;
	   }
	  }     
            

	// Riempiamo il campo CollisionPoint

	GVector CollisionSpeed;
	float Gamma1,Gamma2,CollisionDist;


	for(int i=0;i<ObstacleNum;i++)
	  {

	        CollisionSpeed = Interceptor(IZone[i].Pos, IZone[i].ApproachingSpeed); //vettore che mi da' l'angolo di intercettazione dell'osatcolo ci-esimo
		Gamma1 = fabs(CollisionSpeed.GetTheta() - IZone[i].Pos.GetTheta()); //angolo
		Gamma2 = M_PI - fabs(IZone[i].Pos.GetTheta() - IZone[i].ApproachingSpeed.GetTheta()); //altro angolo
		CollisionDist = IZone[i].Pos.GetRo() * sin(Gamma2) / sin(Gamma1+Gamma2); //Teor. dei seni           
                IZone[i].CollisionPoint=GVector(CollisionDist,CollisionSpeed.GetTheta());
          }
	  
//a questo punto ho desiredspeed e lo modifico per non urtare gli avversari, scansando o a dx o a sx



	Ordinator();  // ordino l'array di Izone da sx a dx


	SelectPivot();	//calcola l'elemento piu' prossimo a Desiredspeed (in base al quale devo 
			//scegliere da che parte deviare)


	DetourDX=DesiredSpeed;
	DetourSX=DesiredSpeed;

        UltimoRompiballeDx=-1;
	UltimoRompiballeSx=-1;
    
	

	if(ObstacleNum!=0)
	    {
	    float Gamma4;
	    for(int ci=Pivot; ci>=0; ci--)
		{
		// qua sotto forse conviene considerare la vel. max degli avverari in campo invece di IZone[ci]->Speed... forse...
                CollisionDist=IZone[ci].CollisionPoint.GetRo();
		GVector tmp1=DetourSX;
		tmp1.SetRo(CollisionDist);
		tmp1=tmp1-IZone[ci].CollisionPoint;                    
		if (CollisionDist< Target->Pos.GetRo())
		//    if(CollisionDist * sin(fabs(IZone[ci].CollisionPoint.GetTheta()-DetourSX.GetTheta())) < SAFERADIUS) //non posso ignorarlo, devo schivarlo
		      if (tmp1.GetRo()<(1.5*SAFERADIUS))
			{
			UltimoRompiballeSx = ci;
                        GVector tmp2=IZone[ci].CollisionPoint;
			GVector tmp3=tmp2;
			tmp2.SetRo(1.5*SAFERADIUS);
			tmp2.Rotate(M_PI/2);
			 if (CollisionDist>800)
			  {
			   float d=CollisionDist-800;
			   tmp3.SetRo(d);
			  } 
                          else
			  {
			  cout<<"Son qua"<<endl;
			  tmp3=-tmp3;
			  tmp3.SetRo(MAXVEL);
			  tmp2.SetRo(MAXVEL);
	                  cout<<tmp3<<endl; 
			  } 
	                GVector tmp4=tmp2+tmp3;
//			cout<<"tmp2 "<<tmp2<<endl<<"tmp3 "<<tmp3<<endl<<"tmp4 "<<tmp4<<endl;
			DetourSX.SetTheta(tmp4.GetTheta());
//			cout<<"SCHIVO a SX "<<DetourSX<<endl;
			}
		}
	    for(int ci=Pivot; ci < NumObj; ci++)
		{
		CollisionDist =IZone[ci].CollisionPoint.GetRo();
		GVector tmp1=DetourDX;
		tmp1.SetRo(CollisionDist);
		tmp1=tmp1-IZone[ci].CollisionPoint;                    

		if(CollisionDist < Target->Pos.GetRo())
		      if (tmp1.GetRo()<(1.5*SAFERADIUS))
			{
			UltimoRompiballeDx = ci;
                        GVector tmp2=IZone[ci].CollisionPoint;
                        GVector tmp3=tmp2;
			tmp2.SetRo(1.5*SAFERADIUS);
                        tmp2.Rotate(-M_PI/2);
                         if (CollisionDist>800)
			  {
			   float d=CollisionDist-800;
			   tmp3.SetRo(d);
			  } 
                          else
			  {
			  tmp3=-tmp3;
			  tmp3.SetRo(MAXVEL);
			  cout<<"Son qua"<<endl;
                          tmp2.SetRo(MAXVEL); 
			  cout<<tmp3<<endl;
			  } 
			GVector tmp4=tmp2+tmp3;
//			cout<<"tmp2 "<<tmp2<<endl<<"tmp3 "<<tmp3<<endl<<"tmp4 "<<tmp4<<endl;
			DetourDX.SetTheta(tmp4.GetTheta());
//			cout<<"SCHIVO a DX "<<DetourDX<<endl;
			}


		}
	    }
		
GVector absPos;
float absH;
myGolem->GetPos(absPos , absH);
GVector traslation;
GVector ortho;



//so dove voglio andare, vediamo da che parte schivare in base alle preferenze sul target

if (!(DetourDX.GetTheta()==DetourSX.GetTheta())) cout<<"soddiversi"<<endl;

	switch (Target->Style)
    
	
	{
	/*
	case Following:
			    if(sin(Target->Pos.GetTheta() - Target->Speed.GetTheta()) > 0 )
				PseudoSpeed = DetourDX;
			    else
				PseudoSpeed = DetourSX;
			    break;
	case Preceding:
			    if(sin(Target->Pos.GetTheta() - Target->Speed.GetTheta()) > 0 )
				PseudoSpeed = DetourSX;
			    else
				PseudoSpeed = DetourDX;
			    break;
	*/
	case BestGuess:
			    float diff1, diff2, Ro1,Ro2 ;
                            Ro1=DetourDX.GetRo();
			    Ro2=DetourSX.GetRo();
			    if (Ro1>2*Ro2) PseudoSpeed=DetourDX; 
			     else if
			       (Ro2>2*Ro1) PseudoSpeed=DetourSX;
			       else
			        {
			         diff1 = fabs(DesiredSpeed.GetTheta()-DetourSX.GetTheta());
			         diff2 = fabs(DesiredSpeed.GetTheta()-DetourDX.GetTheta());
			         PseudoSpeed = (diff1 < diff2) ? DetourSX : DetourDX;
			        }

			    break;
	default:
			    cerr << "So' cazzi!! puttanate nello switch case" << endl ;


        }
 
// trroncare la componente verso il muro ev.
      

	if((Target->Pos.GetRo() < SAFESTOP))        
	    {
	    GVector anonimo = PseudoSpeed.Proiezione(Target->Pos.GetTheta());
            GVector corrente= Target->Speed.Proiezione(Target->Pos.GetTheta());
	    float eheh=corrente.GetRo();
	    eheh *= (corrente.GetTheta()==Target->Pos.GetTheta()) ? +1 : 0 ;
            GVector geronimo;
	    if((anonimo.GetTheta() == Target->Pos.GetTheta()) && (anonimo.GetRo() > (MAXVEL/SAFESTOP*Target->Pos.GetRo()+eheh)))
		geronimo = PseudoSpeed - anonimo;
		anonimo.SetRo(MAXVEL/SAFESTOP*Target->Pos.GetRo()+eheh);
		PseudoSpeed = geronimo + anonimo;
	    }

         else if (PseudoSpeed.GetRo()<MAXVEL) cout<<"STRANO"<<endl;
    

        
       
        if (Target->Pos.GetRo()< RUOTDIST) Jog=Target->Heading;
        
        LastSpeed=PseudoSpeed;    	   

          

 // ecco il frutto di tanta speme.... 		  

       
       
       GVector tmp1=absPos;
       GVector tmp2=PseudoSpeed;
       DrawVector(tmp1, tmp2, 150.0, true); //DEBUG
       
       myGolem->SetSpeed(PseudoSpeed,Jog);       
    //   myGolem->SetSpeed(Target->Pos,Jog);       

    
}



GVector G3dof::Interceptor(GVector TargetPos, GVector TargetSpeed)
{
        GVector rot=TargetPos;
	rot.Rotate(M_PI/2); 
	GVector AbsTargetVel = myActualSpeed + TargetSpeed + myActualJog * rot;

	float MyMaxVel=MAXVEL;

	float TempAlpha=0,TempsinAlpha;
	float Alpha=0, sinAlpha=0,cosAlpha=0,sinAlphaGamma=0;
	float DesiredAngle=0; //angolo scelto attualmente
	float Vu0; //componente nella dir. desiderata
	//angolo per intercettare il target nel caso di velocita' massima in dir. di MagicAngle
         
	double Gamma = M_PI-fabs(TargetPos.GetTheta()-TargetSpeed.GetTheta());
	
	if (Gamma>2*M_PI) Gamma-=(2*M_PI);

        GVector somma;
	GVector copia;
        somma=TargetPos+TargetSpeed;         
        somma.Rotate(-TargetPos.GetTheta()); 

        Gamma*= (somma.GetTheta()>0) ? 1 : -1;    

	sinAlpha = AbsTargetVel.GetRo() / MyMaxVel * sin(Gamma); 
            
	if (fabs(sinAlpha)>1)  Alpha=TargetSpeed.GetTheta()-TargetPos.GetTheta();
	  else Alpha=asin(sinAlpha); 
	    
        float tmpAlpha=Alpha;

	Vu0 = (myActualSpeed.Proiezione(DesiredAngle)).GetRo();
	//    Vu0 = tmp.GetRo();

        
	// CONSIDERO CHE NON HO ACCELERAZIONE INFINITA
	
	int contatore=0;
	float c= TargetPos.GetRo();
	float RealMaxSpeed=MAXVEL;
	
	do
	{
	contatore++;
	if (sinAlpha==0) contatore=101;
	 else
	  {
	    Alpha=tmpAlpha;
	    float a= c*sinAlpha/sin(Gamma);  
	    float b= c*sin(M_PI-Gamma-Alpha)/sin(Gamma);
	    GVector componente=myActualSpeed.Proiezione(Alpha);
	    float myvel=componente.GetRo();
	    
	    if (b<SAFESTOP)  RealMaxSpeed=(myvel/2);
	      else
	       { 
	         float daragg=MAXVEL-myvel;
	         float tmp1= daragg/MAXACC;
	         float dist1=tmp1*(myvel+MAXVEL)/2;
		 if (dist1<(b-SAFESTOP)) RealMaxSpeed=((dist1)*(myvel+MAXVEL)/2+SAFESTOP*MAXVEL/2+(b-SAFESTOP-dist1))/b;
		  else 
		   {
		   float vfinale=myvel+((-myvel+sqrt(myvel*myvel+2*MAXACC*(b-SAFESTOP)))/(2*MAXACC));
		   RealMaxSpeed= (SAFESTOP*vfinale/2 + (b-SAFESTOP)*(myvel+vfinale)/2)/b;
		   }        
	       }      
	  }
        if (contatore==99) cout<<"Fine contatore"<<endl;
	sinAlpha = AbsTargetVel.GetRo() / MyMaxVel * sin(Gamma); 
            
	if (fabs(sinAlpha)>1)  tmpAlpha=TargetSpeed.GetTheta()-TargetPos.GetTheta();
	  else tmpAlpha=asin(sinAlpha); 

	}
        while ((fabs((tmpAlpha-Alpha)/Alpha)>0.01)||(contatore<100));
        // FINITO         

	DesiredAngle = Alpha+TargetPos.GetTheta();
 
	return GVector(MAXVEL, Alpha+TargetPos.GetTheta());	
}


void G3dof::Ordinator()  // SelectionSort (GASP)
{
if(ObstacleNum==0)
    return;
float tmp;
ObjZone temp;
int indice=0;
float min=IZone[0].CollisionPoint.GetTheta()-DesiredSpeed.GetTheta();
for(int i=0; i < ObstacleNum; i++)
    {
    for(int j=i+1; j <= ObstacleNum; j++)
	{
	tmp = IZone[j].CollisionPoint.GetTheta()-DesiredSpeed.GetTheta();
        if(tmp < min)
	    {
	    min = tmp;
	    indice = j;
	    }
	}
    temp = IZone[i];
    IZone[i] = IZone[indice];
    IZone[indice] = temp;
    }
}


void G3dof::SelectPivot()
{
if(ObstacleNum==0)
    return;
float tmp;
float min = fabs(IZone[0].CollisionPoint.GetTheta()-DesiredSpeed.GetTheta());
for(int i=1; i < ObstacleNum; i++)
    {
    tmp = fabs(IZone[i].CollisionPoint.GetTheta()-DesiredSpeed.GetTheta());
    if(tmp<min)
	{
	min=tmp;
	Pivot=i;
	}
    }
return;
}


/*
if ((Walls->FW_Seen)&&((traslation=(absPos-Walls->FW_PointOfView)).GetRo()<MAXODOMETRIA)) 
  {
  
  GVector actualPos1=Walls->FW.Pos1+traslation;
  GVector actualPos2=Walls->FW.Pos2+traslation;
  GVector muro=actualPos2-actualPos1;
  muro.Rotate(M_PI/2);
   if (((muro+actualPos1).GetRo())>(actualPos1.GetRo()))  ortho=muro; else {muro.Rotate(M_PI);ortho=muro;}
  
  double angolaccio=ortho.GetTheta();
  GVector distance=actualPos1.Proiezione(angolaccio);
  if ((distance.GetRo()<MINWALLDIST)&&((DetourDX.Proiezione(angolaccio)).GetTheta()==angolaccio))
    {
    GVector componente=DetourDX.Proiezione(angolaccio);
    DetourDX=DetourDX-componente;
    double nuovoro=distance.GetRo()-SAFERADIUS;
    if (nuovoro>0)  componente.SetRo((distance.GetRo()-SAFERADIUS)); else { componente.SetRo(SAFERADIUS);componente.Rotate(M_PI);}
    DetourDX=DetourDX+componente;
    }
  if ((distance.GetRo()<MINWALLDIST)&&((DetourSX.Proiezione(angolaccio)).GetTheta()==angolaccio))
    {
   GVector componente=DetourSX.Proiezione(angolaccio);
    DetourSX=DetourSX-componente;
    double nuovoro=distance.GetRo()-SAFERADIUS;
    if (nuovoro>0)  componente.SetRo((distance.GetRo()-SAFERADIUS)); else { componente.SetRo(SAFERADIUS);componente.Rotate(M_PI);}
    DetourSX=DetourSX+componente;    
    }
     
  }

if ((Walls->BW_Seen)&&((traslation=(absPos-Walls->BW_PointOfView)).GetRo()<MAXODOMETRIA)) 
  {
  
  GVector actualPos1=Walls->BW.Pos1+traslation;
  GVector actualPos2=Walls->BW.Pos2+traslation;
  GVector muro=actualPos2-actualPos1;
  muro.Rotate(M_PI/2);
   if (((muro+actualPos1).GetRo())>(actualPos1.GetRo()))  ortho=muro; else {muro.Rotate(M_PI);ortho=muro;}
  
  double angolaccio=ortho.GetTheta();
  GVector distance=actualPos1.Proiezione(angolaccio);
  if ((distance.GetRo()<MINWALLDIST)&&((DetourDX.Proiezione(angolaccio)).GetTheta()==angolaccio))
    {
    GVector componente=DetourDX.Proiezione(angolaccio);
    DetourDX=DetourDX-componente;
    double nuovoro=distance.GetRo()-SAFERADIUS;
    if (nuovoro>0)  componente.SetRo((distance.GetRo()-SAFERADIUS)); else { componente.SetRo(SAFERADIUS);componente.Rotate(M_PI);}
    DetourDX=DetourDX+componente;
    }
  if ((distance.GetRo()<MINWALLDIST)&&((DetourSX.Proiezione(angolaccio)).GetTheta()==angolaccio))
    {
   GVector componente=DetourSX.Proiezione(angolaccio);
    DetourSX=DetourSX-componente;
    double nuovoro=distance.GetRo()-SAFERADIUS;
    if (nuovoro>0)  componente.SetRo((distance.GetRo()-SAFERADIUS)); else { componente.SetRo(SAFERADIUS);componente.Rotate(M_PI);}
    DetourSX=DetourSX+componente;    
    }
     
  }
 
if ((Walls->LW_Seen)&&((traslation=(absPos-Walls->LW_PointOfView)).GetRo()<MAXODOMETRIA)) 
  {
  
  GVector actualPos1=Walls->LW.Pos1+traslation;
  GVector actualPos2=Walls->LW.Pos2+traslation;
  GVector muro=actualPos2-actualPos1;
  muro.Rotate(M_PI/2);
   if (((muro+actualPos1).GetRo())>(actualPos1.GetRo()))  ortho=muro; else {muro.Rotate(M_PI);ortho=muro;}
  
  double angolaccio=ortho.GetTheta();
  GVector distance=actualPos1.Proiezione(angolaccio);
  if ((distance.GetRo()<MINWALLDIST)&&((DetourDX.Proiezione(angolaccio)).GetTheta()==angolaccio))
    {
    GVector componente=DetourDX.Proiezione(angolaccio);
    DetourDX=DetourDX-componente;
    double nuovoro=distance.GetRo()-SAFERADIUS;
    if (nuovoro>0)  componente.SetRo((distance.GetRo()-SAFERADIUS)); else { componente.SetRo(SAFERADIUS);componente.Rotate(M_PI);}
    DetourDX=DetourDX+componente;
    }
  if ((distance.GetRo()<MINWALLDIST)&&((DetourSX.Proiezione(angolaccio)).GetTheta()==angolaccio))
    {
   GVector componente=DetourSX.Proiezione(angolaccio);
    DetourSX=DetourSX-componente;
    double nuovoro=distance.GetRo()-SAFERADIUS;
    if (nuovoro>0)  componente.SetRo((distance.GetRo()-SAFERADIUS)); else { componente.SetRo(SAFERADIUS);componente.Rotate(M_PI);}
    DetourSX=DetourSX+componente;    
    }
     
  }
     
if ((Walls->RW_Seen)&&((traslation=(absPos-Walls->RW_PointOfView)).GetRo()<MAXODOMETRIA)) 
  {
  
  GVector actualPos1=Walls->RW.Pos1+traslation;
  GVector actualPos2=Walls->RW.Pos2+traslation;
  GVector muro=actualPos2-actualPos1;
  muro.Rotate(M_PI/2);
   if (((muro+actualPos1).GetRo())>(actualPos1.GetRo()))  ortho=muro; else {muro.Rotate(M_PI);ortho=muro;}
  
  double angolaccio=ortho.GetTheta();
  GVector distance=actualPos1.Proiezione(angolaccio);
  if ((distance.GetRo()<MINWALLDIST)&&((DetourDX.Proiezione(angolaccio)).GetTheta()==angolaccio))
    {
    GVector componente=DetourDX.Proiezione(angolaccio);
    DetourDX=DetourDX-componente;
    double nuovoro=distance.GetRo()-SAFERADIUS;
    if (nuovoro>0)  componente.SetRo((distance.GetRo()-SAFERADIUS)); else { componente.SetRo(SAFERADIUS);componente.Rotate(M_PI);}
    DetourDX=DetourDX+componente;
    }
  if ((distance.GetRo()<MINWALLDIST)&&((DetourSX.Proiezione(angolaccio)).GetTheta()==angolaccio))
    {
   GVector componente=DetourSX.Proiezione(angolaccio);
    DetourSX=DetourSX-componente;
    double nuovoro=distance.GetRo()-SAFERADIUS;
    if (nuovoro>0)  componente.SetRo((distance.GetRo()-SAFERADIUS)); else { componente.SetRo(SAFERADIUS);componente.Rotate(M_PI);}
    DetourSX=DetourSX+componente;    
    }
     
  }

//	printf("\nG3dof: passo10 ");
//        fflush(stdout);
  

*/



