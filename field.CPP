#include "field.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>
#include <math.h>


void Field::Draw()
{
	GLdouble h_wall = 0.5;

	glPushMatrix();
  	glShadeModel(GL_FLAT);
	glNormal3f(0.0, 0.0, 1.0);
	glMaterialf(GL_FRONT, GL_AMBIENT, 1.0);
	glBegin(GL_POLYGON);  // fondo campo
 			glColor3f(0.0, 0.9, 0.0);
	    glVertex3f(6.0 , 3.5, 0.0);
	    glVertex3f(6.0, -3.5, 0.0);
	    glVertex3f(-6.0, -3.5, 0.0);
	    glVertex3f(-6.0, 3.5, 0.0);
	glEnd();
	
	glBegin(GL_QUAD_STRIP);  // Bordo verde
	    glVertex3f(6.0 , 3.5, -0.5);
	    glVertex3f(6.0, 3.5, 0.0);
	    glVertex3f(6.0, -3.5, -0.5);
	    glVertex3f(6.0, -3.5, 0.0);
	    glVertex3f(-6.0, -3.5, -0.5);
			glVertex3f(-6.0, -3.5, 0.0);
			glVertex3f(-6.0, 3.5, -0.5);
			glVertex3f(-6.0, 3.5, 0.0);
	    glVertex3f(6.0 , 3.5, -0.5);
	    glVertex3f(6.0, 3.5, 0.0);
	glEnd();
/*	
	glBegin(GL_QUAD_STRIP);  // Rampa grigia
			glColor3f(0.5, 0.5, 0.5);
	    glVertex3f(8.0 , XDIM, -1.0);
	    glVertex3f(6.0, 3.5, 0.0);
	    glVertex3f(8.0, -XDIM, -1.0);
	    glVertex3f(6.0, -3.5, 0.0);
	    glVertex3f(-8.0, -XDIM, -1.0);
			glVertex3f(-6.0, -3.5, 0.0);
			glVertex3f(-8.0, XDIM, -1.0);
			glVertex3f(-6.0, 3.5, 0.0);
	    glVertex3f(8.0 , XDIM, -1.0);
	    glVertex3f(6.0, 3.5, 0.0);
	glEnd(); */
// ********* linee verticali agli angoli ********
// Imposto il verde  	
	glColor3f(0.0, 0.8, 0.0);
  	
  	// sx in basso
	glBegin(GL_POLYGON);	
	    glVertex3f(-4.8, -2.49, 0.0);
	    glVertex3f(-4.8, -2.49, h_wall);
	    glVertex3f(-4.8, -2.47, h_wall);
	    glVertex3f(-4.8, -2.47, 0.0);
	glEnd();	
	glBegin(GL_POLYGON);
	    glVertex3f(-4.99, -2.3, 0.0);
	    glVertex3f(-4.99, -2.3, h_wall);
	    glVertex3f(-4.99, -2.28, h_wall);
	    glVertex3f(-4.99, -2.28, 0.0);
	glEnd();		
  	// sx in alto
	glBegin(GL_POLYGON);	
	    glVertex3f(-4.8, 2.49, 0.0);
	    glVertex3f(-4.8, 2.49, h_wall);
	    glVertex3f(-4.8, 2.46, h_wall);
	    glVertex3f(-4.8, 2.46, 0.0);
	glEnd();	
	glBegin(GL_POLYGON);
	    glVertex3f(-4.99, 2.3, 0.0);
	    glVertex3f(-4.99, 2.3, h_wall);
	    glVertex3f(-4.99, 2.27, h_wall);
	    glVertex3f(-4.99, 2.27, 0.0);
	glEnd();			
  	// dx in alto
	glBegin(GL_POLYGON);	
	    glVertex3f(4.8, 2.49, 0.0);
	    glVertex3f(4.8, 2.49, h_wall);
	    glVertex3f(4.8, 2.46, h_wall);
	    glVertex3f(4.8, 2.46, 0.0);
	glEnd();	
	glBegin(GL_POLYGON);
	    glVertex3f(4.99, 2.3, 0.0);
	    glVertex3f(4.99, 2.3, h_wall);
	    glVertex3f(4.99, 2.27, h_wall);
	    glVertex3f(4.99, 2.27, 0.0);
	glEnd();			
  	// dx in basso
	glBegin(GL_POLYGON);	
	    glVertex3f(4.8, -2.49, 0.0);
	    glVertex3f(4.8, -2.49, h_wall);
	    glVertex3f(4.8, -2.46, h_wall);
	    glVertex3f(4.8, -2.46, 0.0);
	glEnd();	
	glBegin(GL_POLYGON);
	    glVertex3f(4.99, -2.3, 0.0);
	    glVertex3f(4.99, -2.3, h_wall);
	    glVertex3f(4.99, -2.27, h_wall);
	    glVertex3f(4.99, -2.27, 0.0);
	glEnd();		
	
// fine linee verticali agli angoli	
	
       glLineWidth(5.0);		
	glBegin(GL_LINE_STRIP);  //asse Y
	    glColor3f(1.0, 1.0, 1.0);
	    glVertex3f(0.0, -YDIM, 0.01);
	    glVertex3f(0.0, YDIM, 0.01);
	glEnd(); 

	
	
	// Imposto il bianco
	glColor4f(1.0, 1.0, 1.0, 0.0);
	
	glBegin(GL_LINE_LOOP); // area porta SX
//	    glColor3f(1.0, 1.0, 1.0);
	    glVertex3f(-XDIM, -1.5, 0.01);
	    glVertex3f(-4.0, -1.5, 0.01);
	    glVertex3f(-4.0, 1.5, 0.01);
	    glVertex3f(-XDIM, 1.5, 0.01);
	glEnd();

	glBegin(GL_LINE_LOOP); // area porta DX
//	    glColor3f(1.0, 1.0, 1.0);
	    glVertex3f(XDIM, -1.5, 0.01);
	    glVertex3f(4.0, -1.5, 0.01);
	    glVertex3f(4.0, 1.5, 0.01);
	    glVertex3f(XDIM, 1.5, 0.01);
	glEnd();
	
	glBegin(GL_LINE_LOOP);  // centro campo
//	    glColor3f(1.0, 1.0, 1.0);
	    double r = 1.0;
	    for (double alfa = 0.0; alfa < 6.28; alfa += 0.0535)
		glVertex3f(r*cos(alfa), r*sin(alfa), 0.01);
	glEnd();	
	
       glLineWidth(1.0);	    	

	// ********* muro lungo 1 *********
	glBegin(GL_POLYGON); // Intreno
	    glVertex3f(XDIM , YDIM, 0.0);
	    glVertex3f(XDIM, YDIM, h_wall);
	    glVertex3f(-XDIM, YDIM, h_wall);
	    glVertex3f(-XDIM, YDIM, 0.0);
	glEnd();
	glBegin(GL_POLYGON);	// Esterno
	    glVertex3f(5.1 , 2.6, 0.0);
	    glVertex3f(5.1, 2.6, h_wall);
	    glVertex3f(-5.1, 2.6, h_wall);
	    glVertex3f(-5.1, 2.6, 0.0);	
	glEnd(); 	
	glBegin(GL_POLYGON);	// Sopra
	    glVertex3f(XDIM , YDIM, h_wall);
	    glVertex3f(5.1, 2.6, h_wall);
	    glVertex3f(-5.1, 2.6, h_wall);
	    glVertex3f(-XDIM, YDIM, h_wall);	
	glEnd();

	// ********* muro lungo 2 *********
	glBegin(GL_POLYGON);	// Interno
	    glVertex3f(XDIM , -YDIM, 0.0);
	    glVertex3f(XDIM, -YDIM, h_wall);
	    glVertex3f(-XDIM, -YDIM, h_wall);
	    glVertex3f(-XDIM, -YDIM, 0.0);
	glEnd(); 
	glBegin(GL_POLYGON);	// Esterno
	    glVertex3f(5.1 , -2.6, 0.0);
	    glVertex3f(5.1, -2.6, h_wall);
	    glVertex3f(-5.1, -2.6, h_wall);
	    glVertex3f(-5.1, -2.6, 0.0);	
	glEnd(); 	
	glBegin(GL_POLYGON);	// Sopra
	    glVertex3f(XDIM , -YDIM, h_wall);
	    glVertex3f(5.1, -2.6, h_wall);
	    glVertex3f(-5.1, -2.6, h_wall);
	    glVertex3f(-XDIM, -YDIM, h_wall);	
	glEnd();
	
	// ********* muro corto SX 1 *********
	glBegin(GL_POLYGON);  // Interno
	    glVertex3f(-XDIM , -YDIM, 0.0);
	    glVertex3f(-XDIM, -1.0, 0.0);
	    glVertex3f(-XDIM, -1.0, h_wall);
	    glVertex3f(-XDIM, -YDIM, h_wall);
	glEnd();
	glBegin(GL_POLYGON);	// Esterno
	    glVertex3f(-5.1 , -2.6, 0.0);
	    glVertex3f(-5.1, -1.0, 0.0);
	    glVertex3f(-5.1, -1.0, h_wall);
	    glVertex3f(-5.1, -2.6, h_wall);
	glEnd(); 	
	glBegin(GL_POLYGON);  //Sopra
	    glVertex3f(-XDIM , -YDIM, h_wall);
	    glVertex3f(-5.1, -2.6, h_wall);
	    glVertex3f(-5.1, -1.0, h_wall);
	    glVertex3f(-XDIM, -1.0, h_wall);
	glEnd();

	 // ********* muro corto SX 2 *********
	glBegin(GL_POLYGON); // Interno
	    glVertex3f(-XDIM , 1.0, 0.0);
	    glVertex3f(-XDIM, YDIM, 0.0);
	    glVertex3f(-XDIM, YDIM, h_wall);
	    glVertex3f(-XDIM, 1.0, h_wall);
	glEnd();
	glBegin(GL_POLYGON);  //Esterno
	    glVertex3f(-5.1 , 2.6, 0.0);
	    glVertex3f(-5.1, 1.0, 0.0);
	    glVertex3f(-5.1, 1.0, h_wall);
	    glVertex3f(-5.1, 2.6, h_wall);
	glEnd(); 	
	glBegin(GL_POLYGON);  //Sopra
	    glVertex3f(-XDIM , YDIM, h_wall);
	    glVertex3f(-5.1, 2.6, h_wall);
	    glVertex3f(-5.1, 1.0, h_wall);
	    glVertex3f(-XDIM, 1.0, h_wall);
	glEnd(); 	
 	
	// ******** muro corto DX 1	**********	
	glBegin(GL_POLYGON);	// Interno
	    glVertex3f(XDIM , -YDIM, 0.0);
	    glVertex3f(XDIM, -1.0, 0.0);
	    glVertex3f(XDIM, -1.0, h_wall);
	    glVertex3f(XDIM, -YDIM, h_wall);
	glEnd(); 
	glBegin(GL_POLYGON);	// Esterno
	    glVertex3f(5.1 , -2.6, 0.0);
	    glVertex3f(5.1, -1.0, 0.0);
	    glVertex3f(5.1, -1.0, h_wall);
	    glVertex3f(5.1, -2.6, h_wall);
	glEnd(); 	
	glBegin(GL_POLYGON);  //Sopra
	    glVertex3f(XDIM , -YDIM, h_wall);
	    glVertex3f(5.1, -2.6, h_wall);
	    glVertex3f(5.1, -1.0, h_wall);
	    glVertex3f(XDIM, -1.0, h_wall);
	glEnd();
	
	// ******* muro corto DX 2 ********
	glBegin(GL_POLYGON);	//Interno
	    glVertex3f(XDIM , 1.0, 0.0);
	    glVertex3f(XDIM, YDIM, 0.0);
	    glVertex3f(XDIM, YDIM, h_wall);
	    glVertex3f(XDIM, 1.0, h_wall);
	glEnd();
	glBegin(GL_POLYGON);  //Esterno
	    glVertex3f(5.1 , 2.6, 0.0);
	    glVertex3f(5.1, 1.0, 0.0);
	    glVertex3f(5.1, 1.0, h_wall);
	    glVertex3f(5.1, 2.6, h_wall);
	glEnd(); 	
	glBegin(GL_POLYGON);  //Sopra
	    glVertex3f(XDIM , YDIM, h_wall);
	    glVertex3f(5.1, 2.6, h_wall);
	    glVertex3f(5.1, 1.0, h_wall);
	    glVertex3f(XDIM, 1.0, h_wall);
	glEnd();
	
	// ********* Angoli del campo  *********
	glBegin(GL_POLYGON);	// dx in alto
	    glVertex3f(4.8 , YDIM, h_wall);
	    glVertex3f(XDIM, 2.3, h_wall);
	    glVertex3f(XDIM, 2.3, 0.0);
	    glVertex3f(4.8, YDIM, 0.0);		
	glEnd();

	glBegin(GL_POLYGON);	// dx in basso
	    glVertex3f(4.8 , -YDIM, h_wall);
	    glVertex3f(XDIM, -2.3, h_wall);
	    glVertex3f(XDIM, -2.3, 0.0);
	    glVertex3f(4.8, -YDIM, 0.0);		
	glEnd();
	

	glBegin(GL_POLYGON);	// sx in basso
	    glVertex3f(-4.8 ,-YDIM, h_wall);
	    glVertex3f(-XDIM, -2.3, h_wall);
	    glVertex3f(-XDIM, -2.3, 0.0);
	    glVertex3f(-4.8, -YDIM, 0.0);		
	glEnd();

	glBegin(GL_POLYGON);	// sx in alto
	    glVertex3f(-4.8 , YDIM, h_wall);
	    glVertex3f(-XDIM, 2.3, h_wall);
	    glVertex3f(-XDIM, 2.3, 0.0);
	    glVertex3f(-4.8, YDIM, 0.0);		
	glEnd();
	//********* Fine angoli **********								

	
	//****** porta SX ******  	
	
	// Inposto il blu
	glColor4f(0.0, 0.0, 1.0, 0.0);
	
	glBegin(GL_POLYGON); // fondo
	    glVertex3f(-6.0 , -1.0, 0.0);
	    glVertex3f(-6.0, 1.0, 0.0);
	    glVertex3f(-6.0, 1.0, 1.0);
	    glVertex3f(-6.0, -1.0, 1.0);
	glEnd();

	glBegin(GL_POLYGON);  // lato 1
	    glVertex3f(-6.0 , -1.0, 0.0);
	    glVertex3f(-XDIM, -1.0, 0.0);
	    glVertex3f(-XDIM, -1.0, 1.0);
	    glVertex3f(-6.0, -1.0, 1.0);
	glEnd();

	glBegin(GL_POLYGON);  // lato 2
	    glVertex3f(-6.0 , 1.0, 0.0);
	    glVertex3f(-XDIM, 1.0, 0.0);
	    glVertex3f(-XDIM, 1.0, 1.0);
	    glVertex3f(-6.0, 1.0, 1.0);
	glEnd();

	
	//****** porta DX ******
	
	// Inposto il giallo
	glColor4f(1.0, 1.0, 0.0, 0.0);
	
	glBegin(GL_POLYGON);  // fondo
	    glVertex3f(6.0 , -1.0, 0.0);
	    glVertex3f(6.0, 1.0, 0.0);
	    glVertex3f(6.0, 1.0, 1.0);
	    glVertex3f(6.0, -1.0, 1.0);
	glEnd();

	glBegin(GL_POLYGON);  // lato 1
	    glVertex3f(6.0 , -1.0, 0.0);
	    glVertex3f(XDIM, -1.0, 0.0);
	    glVertex3f(XDIM, -1.0, 1.0);
	    glVertex3f(6.0, -1.0, 1.0);
	glEnd();

	glBegin(GL_POLYGON);  // lato 2
	    glVertex3f(6.0 , 1.0, 0.0);
	    glVertex3f(XDIM, 1.0, 0.0);
	    glVertex3f(XDIM, 1.0, 1.0);
	    glVertex3f(6.0, 1.0, 1.0);
	glEnd();

    glPopMatrix();


}
