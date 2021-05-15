#include "robot.h"
  	
void Robot::make_gkernel(int id)
{  	
  	
/* crea gli esperti */	
	gTimer=new ETTimer();
	gDis=new ETDispatch(id);
	Gol=new GControl(gDis);
	Gol->SetId(id);
	gMem=new GMemHandler(gDis,5);

	int target_area_handle=gMem->CreateMemArea(sizeof(Target_struct));
	int wall_area_handle=gMem->CreateMemArea(sizeof(Walls_struct));
	int obstacle_area_handle=gMem->CreateMemArea(sizeof(All_Robots_struct));
	int ball_area_handle=gMem->CreateMemArea(sizeof(Balloon_struct));
	int goals_area_handle=gMem->CreateMemArea(sizeof(Goals_struct));

	printf ("ba = %d",ball_area_handle);
	printf ("ta = %d",target_area_handle);

	gVision = new GVision(gDis, Gol, gTimer, gMem, ball_area_handle,
	                                               wall_area_handle,
	                                               obstacle_area_handle,
	                                               goals_area_handle);	
	
	
	gMove = new G3dof(gDis, Gol, gMem,
	                  target_area_handle,
	                  wall_area_handle,
	                  obstacle_area_handle,ball_area_handle);
	
    gG=new GGoalie(Gol, gMem, target_area_handle);
	
	gF=new GFake(gDis, Gol, gMem, gG, ball_area_handle, target_area_handle, goals_area_handle);

/* li aggiunge al dispatcher */	
	int a1=gDis->AddExpert(Gol);
	int a2=gDis->AddExpert(gMem);
	int a3=gDis->AddExpert(gVision);
	int a4=gDis->AddExpert(gMove);
	int a5=gDis->AddExpert(gF);
	
/* li attiva */	
	gDis->ActivateExpert(a1);
	gDis->ActivateExpert(a2);
	gDis->ActivateExpert(a3);
	gDis->ActivateExpert(a4);
	gDis->ActivateExpert(a5);
}
