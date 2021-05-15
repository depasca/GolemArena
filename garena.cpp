//---------------------------------------------------------------------------
// Borland C++Builder
// Copyright (c) 1987, 1998 Inprise Corporation. All Rights Reserved.
//---------------------------------------------------------------------------
//---------------------------------------------------------------------------
#include <vcl.h>
#pragma hdrstop

#include "garena.h"
#include "field.h"

//---------------------------------------------------------------------------
#pragma resource "*.dfm"
TOpenGL_Form *OpenGL_Form;
//---------------------------------------------------------------------------
Field *F;




__fastcall TOpenGL_Form::TOpenGL_Form(TComponent* Owner)
	: TForm(Owner)
{
}
//---------------------------------------------------------------------------
void __fastcall TOpenGL_Form::Timer1Timer(TObject *Sender)
{
	Timer1->Enabled = false;
    DrawScene();
    Yield();
    Timer1->Enabled = true;
}

BOOL bSetupPixelFormat(HDC hdc)
{

    PIXELFORMATDESCRIPTOR pfd, *ppfd;
    int pixelformat;

    ppfd = &pfd;

    ppfd->nSize = sizeof(PIXELFORMATDESCRIPTOR);
    ppfd->nVersion = 1;
    ppfd->dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
                        PFD_DOUBLEBUFFER;
    ppfd->dwLayerMask = PFD_MAIN_PLANE;
    ppfd->iPixelType = PFD_TYPE_COLORINDEX;
    ppfd->cColorBits = 8;
    ppfd->cDepthBits = 16;

    ppfd->cAccumBits = 0;
    ppfd->cStencilBits = 0;

    if ( (pixelformat = ChoosePixelFormat(hdc, ppfd)) == 0 )
    {
        MessageBox(NULL, "ChoosePixelFormat failed", "Error", MB_OK);
        return FALSE;
    }

    if (SetPixelFormat(hdc, pixelformat, ppfd) == FALSE)
    {
        MessageBox(NULL, "SetPixelFormat failed", "Error", MB_OK);
        return FALSE;
    }

    return TRUE;

}

void	TOpenGL_Form::DrawScene()
{
    glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
    glClearIndex( (GLfloat)BLACK_INDEX);
    glPushMatrix();

  /*
    latitude += latinc;
    longitude += longinc;

    polarView( radius, 0, latitude, longitude );

        glIndexi(RED_INDEX);
        glCallList(CONE);

        glIndexi(BLUE_INDEX);
        glCallList(GLOBE);

    glIndexi(GREEN_INDEX);
        glPushMatrix();
            glTranslatef(0.8F, -0.65F, 0.0F);
            glRotatef(30.0F, 1.0F, 0.5F, 1.0F);
            glCallList(CYLINDER);
        glPopMatrix();
    */

        F->Draw();

    glPopMatrix();


    SwapBuffers(ghDC);
}

GLvoid TOpenGL_Form::resize( GLsizei width, GLsizei height )
{
        glViewport(0, 0, width, height);
    	glMatrixMode(GL_PROJECTION);
    	glLoadIdentity();
    	gluPerspective(45.0, (GLdouble)width/(GLdouble)height, 0.1, 20.0);
    	glMatrixMode(GL_MODELVIEW);
    	glLoadIdentity();     
    	//GLfloat myangle = 90.0;
    	GLfloat myx = 0.0;
    	GLfloat lookatx = -3.0;//myx+(1.1*cos(myangle*M_PI/180.0));
    	GLfloat myy = 0.0;
    	GLfloat lookaty = 0.0;//myy+(1.1*sin(myangle*M_PI/180.0));
    	GLfloat myz = 1.8;
    	GLfloat lookatz = 0.0;
    	gluLookAt(myx, myy, myz, lookatx, lookaty, lookatz, 0.0, 0.0, 1.0);
}

GLvoid TOpenGL_Form::createObjects()
{
    F=new Field();

    /*

    GLUquadricObj *quadObj;

    glNewList(GLOBE, GL_COMPILE);
        quadObj = gluNewQuadric ();

        gluQuadricDrawStyle (quadObj, GLU_LINE);
        gluSphere (quadObj, 1.5, 16, 16);
    glEndList();

    glNewList(CONE, GL_COMPILE);
        quadObj = gluNewQuadric ();
        gluQuadricDrawStyle (quadObj, GLU_FILL);
        gluQuadricNormals (quadObj, GLU_SMOOTH);
        gluCylinder(quadObj, 0.3, 0.0, 0.6, 15, 10);
    glEndList();

    glNewList(CYLINDER, GL_COMPILE);
        glPushMatrix ();
        glRotatef ((GLfloat)90.0, (GLfloat)1.0, (GLfloat)0.0, (GLfloat)0.0);

        glTranslatef ((GLfloat)0.0, (GLfloat)0.0, (GLfloat)-1.0);
        quadObj = gluNewQuadric ();
        gluQuadricDrawStyle (quadObj, GLU_FILL);
        gluQuadricNormals (quadObj, GLU_SMOOTH);
        gluCylinder (quadObj, 0.3, 0.3, 0.6, 12, 2);
        glPopMatrix ();
    glEndList();

    */
}

GLvoid TOpenGL_Form::initializeGL(GLsizei width, GLsizei height)
{
    /*
    GLfloat     maxObjectSize, aspect;
    GLdouble    near_plane;

    glClearIndex( (GLfloat)BLACK_INDEX);

    glClearDepth( 1.0 );

    glEnable(GL_DEPTH_TEST);

    glMatrixMode( GL_PROJECTION );
    aspect = (GLfloat) width / height;
    gluPerspective( 45.0, aspect, 3.0, 7.0 );
    glMatrixMode( GL_MODELVIEW );

    near_plane = 3.0;
    maxObjectSize = 3.0F;
    radius = near_plane + maxObjectSize/2.0;

    latitude = 0.0F;
    longitude = 0.0F;
    latinc = 6.0F;
    longinc = 2.5F;
    */
    GLfloat a=0.1, aw=8.0,
            d0=0.6, d0w=1.0,
	    d1=0.8, d1w=1.0,
	    p0x=5.0, p0y=0.0, p0z=1.0, p0w=1.0,
	    p1x=6.0, p1y=3.0, p1z=1.0, p1w=0.5;

    glClearColor(0.1, 0.1, 0.1, 0.0);
    glClearIndex( (GLfloat)BLACK_INDEX);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);
    GLfloat amb0[]={a, a, a, aw};
    glLightfv(GL_LIGHT0, GL_AMBIENT, amb0);
    GLfloat dif0[]={d0, d0, d0, d0w};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dif0);
    GLfloat pos0[]={p0x, p0y, p0z, p0w};
    glLightfv(GL_LIGHT0, GL_POSITION, pos0);
    GLfloat dif1[]={d1, d1, d1, d1w};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, dif1);
    GLfloat pos1[]={p1x, p1y, p1z, p1w};
    glLightfv(GL_LIGHT0, GL_POSITION, pos1);


    createObjects();
}

void TOpenGL_Form::polarView(GLdouble radius, GLdouble twist, GLdouble latitude,
           			  GLdouble longitude)
{
    glTranslated(0.0, 0.0, -radius);
    glRotated(-twist, 0.0, 0.0, 1.0);
    glRotated(-latitude, 1.0, 0.0, 0.0);
    glRotated(longitude, 0.0, 0.0, 1.0);

}


//---------------------------------------------------------------------------
void __fastcall TOpenGL_Form::FormCreate(TObject *Sender)
{
        ghDC = GetDC(Handle);
        if (!bSetupPixelFormat(ghDC))
            Close();
        ghRC = wglCreateContext(ghDC);
        wglMakeCurrent(ghDC, ghRC);
        initializeGL(ClientRect.Right, ClientRect.Bottom);
}
//---------------------------------------------------------------------------
void __fastcall TOpenGL_Form::FormResize(TObject *Sender)
{
	resize(ClientRect.Right, ClientRect.Bottom);
}
//---------------------------------------------------------------------------
void __fastcall TOpenGL_Form::FormClose(TObject *Sender, TCloseAction &Action)
{
	    Timer1->Enabled = false;
        if (ghRC)
            wglDeleteContext(ghRC);
        if	(ghDC)
        	ReleaseDC(Handle, ghDC);
}
//---------------------------------------------------------------------------
void __fastcall TOpenGL_Form::FormKeyDown(TObject *Sender, WORD &Key,
	TShiftState Shift)
{
	Close();
}
//---------------------------------------------------------------------------