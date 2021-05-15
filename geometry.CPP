#include "geometry.h"

Point::Point(){ x = y = h = 0.0;}
Point::Point(double setx, double sety){ x = setx; y = sety; h = 0.0; }
Point::Point(double setx, double sety, double seth){ x = setx; y = sety; h = seth; }
void Point::Draw(GColor pcolor)
{
	glPushMatrix();
		glTranslatef(x, y, 0.15);
		switch(pcolor)
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
				glColor3f(1.0, 0.0, 1.0);
				break;
       		case Gred:
				glColor3f(1.0, 0.0, 0.0);
				break;
       		case Gcyan:
				glColor3f(0.0, 1.0, 1.0);
				break;
       		case Gmagenta:
				glColor3f(1.0, 0.0, 1.0);
				break;
       		default:
				glColor3f(1.0, 1.0, 1.0);
				break;
       	}
      	glutSolidSphere(0.04, 5, 5);
    glPopMatrix();
}   		

Segment::Segment(Point _P1, Point _P2)
{
	P1=_P1;
	P2=_P2;
}
Point Segment::GetP1() { return P1;}	
Point Segment::GetP2() { return P2;}
float Segment::GetRo() { return sqrt(((P1.x-P2.x)*(P1.x-P2.x))+((P1.y-P2.y)*(P1.y-P2.y)));}
float Segment::GetTheta() {return atan2(P2.y-P1.y, P2.x-P1.x);}
Point Segment::middle()
{
	return Point(P1.x+(P2.x-P1.x)/2.0, P1.y+(P2.y-P1.y)/2.0);
}

/********************Non Funziona Bene**********************************/		 	
bool Segment::Has_on_it(Point p)
{
	Segment tmp(P1, p);
	if(tmp.GetRo()<=GetRo() && tmp.GetTheta()==GetTheta())
		return true;
	else
		return false;
}
void Segment::Draw(GColor scolor=Gblack)
{
	glPushMatrix();
		glTranslatef(0.0, 0.0, 0.15);
		switch(scolor)
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
				glColor3f(1.0, 0.0, 1.0);
				break;
       		case Gred:
				glColor3f(1.0, 0.0, 0.0);
				break;
       		case Gcyan:
				glColor3f(0.0, 1.0, 1.0);
				break;
       		case Gmagenta:
				glColor3f(1.0, 0.0, 1.0);
				break;
       		default:
				glColor3f(1.0, 1.0, 1.0);
				break;
   		}
		glLineWidth(2.0);
		glBegin(GL_LINE_STRIP);
			glVertex3f(P1.x, P1.y, 0.0);
			glVertex3f(P2.x, P2.y, 0.0);
		glEnd();
	glPopMatrix();
}


Line::Line(float _m=0.0, float _q=0.0)
{
	m=_m;
	q=_q;
}	  		
Line::Line(Point P1, Point P2)
{
	m=(P2.y-P1.y)/(P2.x-P1.x);
	q=P1.y-P1.x*m;
}
Line::Line(Segment S)
{
	m=(S.GetP2().y-S.GetP1().y)/(S.GetP2().x-S.GetP1().x);
	q=S.GetP1().y-S.GetP1().x*m;
}
Line::Line(Segment *S)
{
	m=(S->GetP2().y-S->GetP1().y)/(S->GetP2().x-S->GetP1().x);
	q=S->GetP1().y-S->GetP1().x*m;
}
Line::Line(Point P, GVector *V)
{
	m=V->GetY()/V->GetX();
	q=P.y-P.x*m;
}
Line Line::ortho(Point P)
{
	Line L;
	if(m!=0.0)
		L.m=-1.0/m;
	else
		L.m=99999.0;
	L.q=P.y-P.x*L.m;
	return L;
}
Line Line:: parallel(Point P)
{
	Line L;
	L.m=m;
	L.q=P.y-P.x*L.m;
	return L;
}
float Line::Get_m() { return m; }
float Line::Get_q() { return q; }
void Line::Draw(GColor lcolor=Gblack)
{
	Point P1(-10.0, -10.0*m+q);
	Point P2(10.0, 10.0*m+q);
	glPushMatrix();
		glTranslatef(0.0, 0.0, 0.15);
		switch(lcolor)
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
				glColor3f(1.0, 0.0, 1.0);
				break;
       		case Gred:
				glColor3f(1.0, 0.0, 0.0);
				break;
        	case Gcyan:
				glColor3f(0.0, 1.0, 1.0);
				break;
        	case Gmagenta:
				glColor3f(1.0, 0.0, 1.0);
				break;
        	default:
				glColor3f(1.0, 1.0, 1.0);
				break;
       	}
		glLineWidth(2.0);
		glBegin(GL_LINE_STRIP);
			glVertex3f(P1.x, P1.y, 0.0);
			glVertex3f(P2.x, P2.y, 0.0);
		glEnd();
	glPopMatrix();
}

Point interception (Line L1, Line L2)
{
	if(L1.Get_m()==L2.Get_m())
		return Point(FORA, FORA);
	else
	{
		float x=(L2.Get_q()-L1.Get_q())/(L1.Get_m()-L2.Get_m());
		float y=L1.Get_m()*x+L1.Get_q();
		return Point(x, y);
	}
}

Point interception (Line *L1, Line *L2)
{
	if(L1->Get_m()==L2->Get_m())
		return Point(FORA, FORA);
	else
	{
		float x=(L2->Get_q()-L1->Get_q())/(L1->Get_m()-L2->Get_m());
		float y=L1->Get_m()*x+L1->Get_q();
		return Point(x, y);
	}
}
			
/*************** funzione appartenenza di un punto a un segmento ****************/
bool point_on_segment(Point p, Segment s)
{
 	/* retta contenente il segmento */
	Line segline(s);
	
	/* controlla se il punto appartiene alla retta contenente il segmento */
 	if(p.y=segline.Get_m()*p.x+segline.Get_q())
 		
 		/* in tal caso controlla che il punto non abbia distanza dal centro
 		del segmento maggiore di metà modulo del segmento stesso */
		if(sqrt((p.x-s.middle().x)*(p.x-s.middle().x)+(p.y-s.middle().y)*(p.y-s.middle().y))<s.GetRo()/2.0)
			return true;
	
	return false;					
}

/*************** funzione distanza punto-segmento *****************/
float Dist(Point p, Segment s)
{
	/* retta contenente il segmento */
	Line segline(s);
	
	/* retta ortogonale alla precedente passante per p */
	Line ortholine=segline.ortho(p);
	
	/* punto di intersezione fra le rette precedenti */
	Point cross=interception(segline, ortholine);
	
	/* se il punto di intersezione sta sul segmento ritorna la distanza di tale punto da p */
	if(point_on_segment(cross, s))
		return sqrt((p.x-cross.x)*(p.x-cross.x)+(p.y-cross.y)*(p.y-cross.y));
		
	/* altrimenti ritorna la distanza fra p e l'estremo del segmento più vicino a p */
	else
	{
		float d1=sqrt((p.x-s.GetP1().x)*(p.x-s.GetP1().x)+(p.y-s.GetP1().y)*(p.y-s.GetP1().y));
		float d2=sqrt((p.x-s.GetP2().x)*(p.x-s.GetP2().x)+(p.y-s.GetP2().y)*(p.y-s.GetP2().y));			
	    return d1<d2 ? d1 : d2;
	}
}	

