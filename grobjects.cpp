#include "grobjects.h"


void DrawVector(GVector pos, GVector draw, float z, bool go_to_meters, GColor c)
{
	glPushMatrix();
	
	switch(c)
	{
		case Gwhite:
			glColor3f(1.0, 1.0, 1.0);
			break;
		case Gblack:
			glColor3f(0.0, 0.0, 0.0);
			break;
   		case Gblue:
			glColor3f(0.0, 0.0, 1.0);
			break;
   		case Gyellow:
			glColor3f(1.0, 1.0, 0.0);
			break;
	  	case Gred:
			glColor3f(1.0, 0.0, 0.0);
			break;
		case Glowred:
			glColor3f(0.5, 0.0, 0.0);
			break;
   		case Gcyan:
			glColor3f(0.0, 1.0, 1.0);
			break;
	    case Gmagenta:
			glColor3f(1.0, 0.0, 1.0);
			break;
		case Ggreen:
			glColor3f(0.0, 0.1, 0.0);
			break;
	    default:
			glColor3f(1.0, 1.0, 1.0);
			break;
       	}
	
       	if(go_to_meters)
       		glScalef(0.001, 0.001, 0.001);
       	
       	glLineWidth(1.0);	
		glTranslatef(pos.GetX(), pos.GetY(), z);
                glBegin(GL_LINE_STRIP);
                	glVertex3f(0.0, 0.0, 0.0);
                	glVertex3f(draw.GetX(), draw.GetY(), 0.0);
                glEnd();

	        glPushMatrix();

                glTranslatef(draw.GetX(), draw.GetY(), 0.0);
				glRotatef(90.0+draw.GetTheta()*180.0/M_PI, 0.0, 0.0, 1.0);
		    	glRotatef(-90.0, 0.0, 0.0, 1.0);
		    	glScalef(1.0, 0.5, 1.0);
				GRCircle C(GRTACCA);		
		    	C.Draw(Point(0.0, 0.0), 3, c, 0);	
		    	
		    glPopMatrix();	
			    	
	glPopMatrix();
	
}

void DrawVector(Point p, GVector v, float z, bool go_to_meters, GColor c)
{
	DrawVector(GVector(p.x, p.y, Cartesian), v, z, go_to_meters, c);
}

void DrawVector(GVector p, GVector v, GColor c=Gblack)
{
	DrawVector(p, v, GRZ_UP, false, c);
}			

void gaDrawVector(GVector p, GVector v, GColor c=Gblack)
{
	DrawVector(p, v, 150.0, true, c);
}	

void DrawVector(Point p, GVector v, GColor c=Gblack)
{
	DrawVector(p, v, GRZ_UP, false, c);
}	

GRCircle::GRCircle(float r)
{
 	Radius=r;
}
  	
void GRCircle::Draw(GVector p, int slices, GColor c, bool full)
{
	glPushMatrix();
		
     	glTranslatef(p.GetX(), p.GetY(), 0.0);
		
		switch(c)
		{
			case Gwhite:
				glColor3f(1.0, 1.0, 1.0);
				break;
			case Gblack:
				glColor3f(0.0, 0.0, 0.0);
				break;
    		case Gblue:
				glColor3f(0.0, 0.0, 1.0);
				break;
       		case Gyellow:
				glColor3f(1.0, 1.0, 0.0);
				break;
	   		case Gred:
				glColor3f(1.0, 0.0, 0.0);
				break;
			case Glowred:
				glColor3f(0.1, 0.0, 0.0);
				break;
    		case Gcyan:
				glColor3f(0.0, 1.0, 1.0);
				break;
	   		case Gmagenta:
				glColor3f(1.0, 0.0, 1.0);
				break;
			case Ggreen:
				glColor3f(0.0, 0.1, 0.0);
				break;
	        default:
				glColor3f(1.0, 1.0, 1.0);
				break;
       	}
       	if(full)
       	{	
       		glBegin(GL_POLYGON);
	   		for (double alfa = 0.0; alfa <= 6.28; alfa += 6.28/float(slices))
				glVertex3f(Radius*cos(alfa), Radius*sin(alfa), 0.0);
		glEnd();
		
	}
	else
	{
	        glBegin(GL_LINE_STRIP);
	   		for (double alfa = 0.0; alfa <= 6.28; alfa += 6.28/float(slices))
				glVertex3f(Radius*cos(alfa), Radius*sin(alfa), 0.0);
		glEnd();
	}
	glPopMatrix();
}   			

       		
void GRCircle::Draw(Point p, int slices, GColor c, bool full)
{
	Draw(GVector(p.x, p.y, Cartesian), slices, c, full);
}	

GRBall::GRBall()
{
	C=GRCircle(GRBALL_RADIUS);
}
  	
void GRBall::Draw(GVector pos, GVector speed, GColor c)
{
	glPushMatrix();
 		
		/* cerchio dell'oggetto */
		glTranslatef(0.0, 0.0, GRZ_UP);
		C.Draw(pos, GROBJ_SLICES, c, 1);
		if(GR_DRAW_SPEED_VECTORS && speed.GetRo()>GR_MIN_SPEED_DRAWED)
		    DrawVector(pos, speed, c);
		  			
	glPopMatrix();	
}

GRTarget::GRTarget()
{
	
}
  	
void GRTarget::Draw(GVector p, GVector speed)
{
	glPushMatrix();
		
		if(GR_DRAW_BGCIRCLE)
		  	glColor3f(0.0, 0.0, 0.0);		
		else
		  	glColor3f(0.9, 0.9, 0.9);
  		
		glTranslatef(p.GetX(), p.GetY(), GRZ_UP);
		glPushMatrix();
		
		  glRotatef(45.0, 0.0, 0.0, 1.0);
		  glBegin(GL_LINE_STRIP);
			glVertex3f(GRTACCA, 0.0, 0.0);
			glVertex3f(-GRTACCA, 0.0, 0.0);
		  glEnd();
		  glBegin(GL_LINE_STRIP);	
			glVertex3f(0.0, GRTACCA, 0.0);
			glVertex3f(0.0, -GRTACCA, 0.0);
		  glEnd();
		  
		glPopMatrix();
		
		glBegin(GL_LINE_STRIP);
		  glVertex3f(0.0, 0.0, 0.0);
		  glVertex3f(speed.GetX(), speed.GetY(), 0.0);    
		glEnd();  

		if(GR_DRAW_SPEED_VECTORS && speed.GetRo()>GR_MIN_SPEED_DRAWED)
		{
			if(GR_DRAW_BGCIRCLE)
		    	DrawVector(GVector(0.0, 0.0, Cartesian), speed, Gblack);
            else
		    	DrawVector(GVector(0.0, 0.0, Cartesian), speed, Gwhite);
		}
		
	glPopMatrix();	

}
  	

GRObject::GRObject()
{
	Cext=new GRCircle(GROBJECT_RADIUS);
	Cint=new GRCircle(GROBJECT_RADIUS/3.0);
}
  	
void GRObject::Draw(GVector p, GVector s, GColor c=Gblack)
{
	glPushMatrix();
  		
 		glTranslatef(0.0, 0.0, GRZ_UP);
  		if(GR_DRAW_BGCIRCLE)
  		{
  			/* cerchio dell'oggetto */
  			Cext->Draw(p, GROBJ_SLICES, Gblack, 1);
  		
  			/* cerchio più piccolo colorato per distinguere compagni e avversari */
  			if(c!=Gblack)
  			{
  				glTranslatef(0.0, 0.0, GRZ_UP);
  				Cint->Draw(p, GROBJ_SLICES, c, 1);
  			}	
  	    }
  	    else
  	    {
  	    	/* cerchio dell'oggetto colorato */
  			Cext->Draw(p, GROBJ_SLICES, c, 1);
  		}
  		
		if(GR_DRAW_SPEED_VECTORS)
			DrawVector(p, s, c);
  		
  	glPopMatrix();	
  	    	
}
  	

GRGoalPost::GRGoalPost()
{
  C=GRCircle(GROBJECT_RADIUS);
}
  
void GRGoalPost::Draw(GVector p, int slices, GColor c)
{
  glPushMatrix();
	glTranslatef(0.0, 0.0, GRZ_UP);          
	C.Draw(p, slices, c, 1);       	
 
  glPopMatrix();		
}
  	
GRWall::GRWall()
{
	P1=Point(0.0, 0.0);
	P2=Point(0.0, 0.0);
}

GRWall::GRWall(Point p1, Point p2)
{
	P1=p1;
	P2=p2;
}
  	
GRWall::GRWall(GVector v1, GVector v2)
{
	P1=Point(v1.GetX(), v1.GetY());
	P2=Point(v2.GetX(), v2.GetY());
}

GRWall::GRWall(GVector v)
{
	Point middle(v.GetX(), v.GetY());
	GVector ortho(5000.0, v.GetTheta()+(M_PI/2.0));
	GVector v1=v+ortho;
	GVector v2=v-ortho;
	P1=Point(v1.GetX(), v1.GetY());
	P2=Point(v2.GetX(), v2.GetY());
} 		

void GRWall::Draw()
{
	glPushMatrix();
  		
		glTranslatef(0.0, 0.0, GRZ_UP);
		glColor3f(0.8, 0.8, 0.8);
		glBegin(GL_LINE_STRIP);
			glVertex3f(P1.x, P1.y, 0.0);
			glVertex3f(P2.x, P2.y, 0.0);
		glEnd();	
			  			
	glPopMatrix();	
}

GRBackGround::GRBackGround(float r)
{
	Radius=r;
	C=GRCircle(Radius);
}
  	
void GRBackGround::Draw(Point p)
{
	glPushMatrix();
 		
		/* cerchio del radar */
		if(GR_DRAW_BGCIRCLE)
			C.Draw(p, GRRADAR_SLICES, Ggreen, 1);
  		
		/*seleziono il colore degli assi */
		if(GR_DRAW_BGCIRCLE)
			glColor3f(0.0, 0.0, 0.0);
		else
			glColor3f(0.9, 0.9, 0.9);
		
		/* asse verticale */
		glBegin(GL_LINE_STRIP);
			glLineWidth(5.0);
			glVertex3f(Radius, 0.0, GRZ_UP);
			glVertex3f(-Radius, 0.0, GRZ_UP);
		glEnd();
		
		/* asse orizzontale */
		glBegin(GL_LINE_STRIP);
			glLineWidth(5.0);
			glVertex3f(0.0, Radius, GRZ_UP);
			glVertex3f(0.0, -Radius, GRZ_UP);
		glEnd();	
 		
		/* tacche sull'asse verticale */
		for(float x=1000.0; x<Radius; x+=1000.0)
		{
			glBegin(GL_LINE_STRIP);
				glLineWidth(5.0);
				glVertex3f(x, -GRTACCA, GRZ_UP);
				glVertex3f(x, GRTACCA, GRZ_UP);
			glEnd();
			glBegin(GL_LINE_STRIP);
				glLineWidth(5.0);
				glVertex3f(-x, -GRTACCA, GRZ_UP);
				glVertex3f(-x, GRTACCA, GRZ_UP);
			glEnd();
		}	
		
		/* tacche sull'asse orizzontale */
		for(float y=1000.0; y<Radius; y+=1000.0)
		{
			glBegin(GL_LINE_STRIP);
				glLineWidth(5.0);
				glVertex3f(-GRTACCA, y, GRZ_UP);
				glVertex3f(GRTACCA, y, GRZ_UP);
			glEnd();
			glBegin(GL_LINE_STRIP);
				glLineWidth(5.0);
				glVertex3f(-GRTACCA, -y, GRZ_UP);
				glVertex3f(GRTACCA, -y, GRZ_UP);
			glEnd();
		}
  			
	glPopMatrix();
}
