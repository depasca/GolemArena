
//--------------- gjoycontrol.cpp --------------------

/** Class for the control of Golem robots via joystick
 *     2000  Nikita Scattolin, Golem Team
 *
 *  v. 1.0.2 needs
 *
 *  needs recent joystick.h in /usr/include/linux
 **/

#include <stdlib.h>

#include "gjoycontrol.h"

//------------------   COSTRUTTORI   ---------------------------

GJoyControl::GJoyControl(char* DevName = "/dev/js0")
{
	joystick = new JoyHandler(DevName);
	
	speed.SetCartesian(0.0, 0.0);
	stopped = true;
		
	_kjog = MAX_JOG/MAX_VALUE; //0.000031;		// 1/32767
	_kv = MAX_SPEED/MAX_VALUE; //0.061;			// 2000/32767
	
	signX = signY = signJog = -1;
	
	x = y = jog = 0;
	
	joystick->read_data();
	
	numAx = numAxis();
	numBut = numButtons();
		
	SetAxesX(1);
	SetAxesY(0);
	
	// Passatemi la bruttura, serve per evitare conflitti
	// usando joystick con pochi assi
	SetAxesJog(-1);
	SetButtonKickDX(-1);
	SetButtonKickSX(-1);
	SetButtonKickFWD(-1);
		
	if(numAx > 2)
		SetAxesJog(2);

	SetButtonBrake(0);
	
	if(numBut > 1)
		SetButtonKickFWD(1);
	if(numBut > 3){
     	SetButtonKickDX(2);
     	SetButtonKickSX(3);		
	}
	// Su misura pre MS SideWinder FFB	
   	if(numBut > 6){
     	SetButtonKickDX(4);
     	SetButtonKickSX(7);
     	SetButtonKickFWD(5);
   	}
}



//------------------   DISTRUTTORE   ---------------------------

GJoyControl::~GJoyControl()
{
	delete joystick;
}

//----------   Acquisizione ed interpretazione dati   ----------

void GJoyControl::ReadAndParse()
{
	joystick->read_data();
	type = joystick->get_type();
	type &= ~JS_EVENT_INIT;
	number = joystick->get_number();
	value = joystick->get_value();
	
	if(type == JS_EVENT_AXIS ){
		if(number == _xAxes){
			x = (float)(signX * value);
		}else
		if(number == _yAxes){
			y = (float)(signY * value);
		}else
		if(number == _jogAxes){
			jog = (float)(signJog * value);
		}
		#ifdef _DEBUG_GJOYCONTROL_
			cout << "\n Asse: " << number << " = " << value << endl;
		#endif
	}else
	if(type == JS_EVENT_BUTTON){
	
		if(number == _kickDX){
			kDX = (bool)value;
		}else
		
		if(number == _kickSX){
			kSX = (bool)value;
		}else
		
		if(number == _kickFWD){
			kFWD = (bool)value;
		}else
		
		if(number == _brake){
			if(!value)
				stopped = true;
			else
				stopped = false;
		}
		#ifdef _DEBUG_GJOYCONTROL_
			cout << "\n Pulsante: " << number << " = " << value << endl;
		#endif
	}
	
	return;
	
}

//-------------------   METODI DI LETTURA   --------------------

char GJoyControl::numAxis() {return joystick->num_axis();}
char GJoyControl::numButtons() {return joystick->num_buttons();}

void GJoyControl::ReadSpeed(GVector &v)
{
	if(stopped)
		v.SetPolar( 0.0, 0.0 );
	else{
     	float tmp;
     	
     	speed.SetCartesian(x, y);
     	
     	tmp=speed.GetRo();
     	
     	#ifdef _DEBUG_GJOYCONTROL_
     	cout << "--- tmp= " << tmp << endl;
     	#endif
     	
     	if(tmp < MAX_VALUE)
     		speed.SetRo( tmp * _kv );
     	else
     		speed.SetRo( (float)MAX_SPEED );
     		
     	v.SetPolar( speed.GetRo(), speed.GetTheta() );
	}
	
	return;
}

float GJoyControl::ReadJog()
{
	return ( _kjog * jog );
}
bool GJoyControl::ReadKickDX(){ return kDX; }
bool GJoyControl::ReadKickSX(){ return kSX; }
bool GJoyControl::ReadKickFWD(){ return kFWD; }

int GJoyControl::AxesX() {return _xAxes;}
int GJoyControl::AxesY() {return _yAxes;}
int GJoyControl::AxesJog() {return _jogAxes;}
int GJoyControl::ButtonBrake() {return _brake;}
int GJoyControl::ButtonKickDX() {return _kickDX;}
int GJoyControl::ButtonKickSX() {return _kickSX;}
int GJoyControl::ButtonKickFWD() {return _kickFWD;}







//-------------------   METODI PER L' IMPOSTAZIONE   ---------------

void GJoyControl::SetAxesX(int i) { _xAxes = i; }

void GJoyControl::SetAxesY(int i) { _yAxes = i; }

void GJoyControl::SetAxesJog(int i) { _jogAxes = i; }

void GJoyControl::SetButtonKickDX(int i) { _kickDX = i; }

void GJoyControl::SetButtonKickSX(int i) { _kickSX = i; }

void GJoyControl::SetButtonKickFWD(int i) { _kickFWD = i; }

void GJoyControl::SetButtonBrake(int i) { _brake = i; }

void GJoyControl::SetKV(float k) { _kv = k; }
void GJoyControl::SetKJog(float k){ _kjog = k; }

void inline GJoyControl::InvertAxesX() { signX *= -1; }
void inline GJoyControl::InvertAxesY() { signY *= -1;}
void inline GJoyControl::InvertAxesJog() { signJog *= -1;}

void GJoyControl::Setup()
{
	bool end = false;
	int tmp;
	int treshold = 20000;
	int step = 0;
	signX = signY = signJog = 1;
	
	printf("\033[2J");		        /* Pulisce lo schermo */
 	printf("\033[2;0f");		/* Mette il cursore nell angolo in alto a sx */
   	cout << "\n\n" << "************************************" << endl
   	               << "********  GJOYSTICK SETUP   ********" << endl
   	               << "************************************" <<endl << endl;
   	/*cout <<           "************************************" <<endl
   	     <<           "*  Posiziona al centro la MANETTA  *" << endl
   	     <<           "*    e premi un pulsante.          *" << endl
   	     <<           "************************************" << endl;*/
   	
   	cout <<           "************************************" << endl
   	               << "*  Sono stati rilevati: " << (int)numAx << " assi     *" << endl
   	               << "*                       " << (int)numBut << " pulsanti *" << endl
   	               << "*                                  *" << endl
   	               << "* Premi un pulsante per continuare *" << endl
   	               << "************************************" << endl;
   	end = false;
   	while(!end)
   	{
   		joystick->read_data();
 		type = joystick->get_type();
		type &= ~JS_EVENT_INIT;
		value = joystick->get_value();
   		if(type == JS_EVENT_BUTTON){
   			if(value)
   				end = true;
   			else{
   				;
   			}
   		}
   	}
	step++;
	
	printf("\033[2J");		        /* Pulisce lo schermo */
 	printf("\033[8;0f");		/* Posiziono il cursore */
   	cout <<           "*************    <"<<step<<">    ************" <<endl
   	     <<           "*    Spingi in AVANTI la cloche    *" << endl
   	     <<           "*    e premi un pulsante.          *" << endl
   	     <<           "************************************" << endl;
   	end = false;
   	tmp = -1;
   	while(!end)
   	{
   		joystick->read_data();
		type = joystick->get_type();
		type &= ~JS_EVENT_INIT;
		number = joystick->get_number();
		value = joystick->get_value();
   		
		if(type == JS_EVENT_AXIS){
			if(value < 0)
				signX = -1;
			else signX = 1;
			if(signX*value > treshold)
   				tmp = number;
   		}else
   		if(type == JS_EVENT_BUTTON & tmp != -1){
   			if(value)
   				SetAxesX(tmp);
   			else{
   				end = true;
   			}
   		}
   	}
   	step++;
   	
	printf("\033[2J");		        /* Pulisce lo schermo */
 	printf("\033[8;0f");		/* Posiziono il cursore */
   	cout <<           "*************    <"<<step<<">    ************" << endl
   	     <<           "*    Spingi a SINISTRA la cloche   *" << endl
   	     <<           "*    e premi un pulsante.          *" << endl
   	     <<           "************************************" << endl;
   	end = false;
   	tmp = -1;
   	while(!end)
   	{
   		joystick->read_data();
		type = joystick->get_type();
		type &= ~JS_EVENT_INIT;
		number = joystick->get_number();
		value = joystick->get_value();
   		
		if(type == JS_EVENT_AXIS){
			if(value < 0)
				signY = -1;
			else signY = 1;
			if(signY*value > treshold)
   				tmp = number;
   		}else
   		if(type == JS_EVENT_BUTTON & tmp != -1 & tmp != _xAxes){
   			if(value)
   			 SetAxesY(tmp);
   			else{
   				end = true;
   			}
   		}
   	}
   	step++;
   	
   	if(numAx>2) {   	
     	printf("\033[2J");		        /* Pulisce lo schermo */
      	printf("\033[8;0f");		/* Posiziono il cursore */
        	cout <<           "*************    <"<<step<<">    ************" << endl
        	     <<           "*    RUOTA a sinistra la cloche    *" << endl
        	     <<           "*    e premi un pulsante           *" << endl
        	     <<           "************************************" << endl;
        	end = false;
        	tmp = -1;
        	while(!end)
        	{
        		joystick->read_data();
     		type = joystick->get_type();
     		type &= ~JS_EVENT_INIT;
     		number = joystick->get_number();
     		value = joystick->get_value();
        		
     		if(type == JS_EVENT_AXIS){
     			if(value < 0)
     				signJog = -1;
     			else signJog = 1;
     			if(signJog*value > treshold)
        				tmp = number;
        		}else
        		if(type==JS_EVENT_BUTTON & tmp!=-1 & tmp!=_xAxes & tmp!=_yAxes){
        			if(value)
        				SetAxesJog(tmp);
        			else{
        				end = true;
        			}
        		}
        	}
     	step++;
	}
	
	printf("\033[2J");		        /* Pulisce lo schermo */
 	printf("\033[8;0f");		/* Posiziono il cursore */
   	cout <<           "*************    <"<<step<<">    ************" << endl
   	     <<           "*     Premi il pulsante per lo     *" << endl
   	     <<           "*               START              *" << endl
   	     <<           "************************************" << endl;
	end =false;
	while(!end)
	{
		joystick->read_data();
		type = joystick->get_type();
		type &= ~JS_EVENT_INIT;
		number = joystick->get_number();
		value = joystick->get_value();
		
		if(type==JS_EVENT_BUTTON) {
			if(value)
				SetButtonBrake(number);
			else{
				end = true;
			}
		}
	}
	step++;
	
	if(numBut > 3) {	
     	printf("\033[2J");		        /* Pulisce lo schermo */
      	printf("\033[8;0f");		/* Posiziono il cursore */

        	cout <<           "*************    <"<<step<<">    ************" << endl
        	     <<           "*     Premi il pulsante per il     *" << endl
        	     <<           "*         KICKER SINISTRO.         *" << endl
        	     <<           "************************************" << endl;
     	end =false;
     	while(!end)
     	{
     		joystick->read_data();
     		type = joystick->get_type();
     		type &= ~JS_EVENT_INIT;
     		number = joystick->get_number();
     		value = joystick->get_value();
     		
     		if(type == JS_EVENT_BUTTON & number != _brake){
     			if(value)
     				SetButtonKickSX(number);
     			else{
     				end = true;
     			}
     		}
     	}
        	step++;
        	
     	printf("\033[2J");		        /* Pulisce lo schermo */
      	printf("\033[8;0f");		/* Posiziono il cursore */
        	cout <<           "*************    <"<<step<<">    ************" << endl
        	     <<           "*     Premi il pulsante per il     *" << endl
        	     <<           "*          KICKER DESTRO.          *" << endl
        	     <<           "************************************" << endl;
     	end =false;
     	while(!end)
     	{
     		joystick->read_data();
     		type = joystick->get_type();
     		type &= ~JS_EVENT_INIT;
     		number = joystick->get_number();
     		value = joystick->get_value();
     		
     		if(type==JS_EVENT_BUTTON & number!=_kickSX & number != _brake){
     			if(value)
     				SetButtonKickDX(number);
     			else{
     				end = true;
     			}
     		}
     	}
     	step++;
	}
	
	if(numBut > 1) {
     	printf("\033[2J");		        /* Pulisce lo schermo */
      	printf("\033[8;0f");		/* Posiziono il cursore */
        	cout <<           "*************    <"<<step<<">    ************" << endl
        	     <<           "*     Premi il pulsante per il     *" << endl
        	     <<           "*          TIRO IN AVANTI          *" << endl
        	     <<           "************************************" << endl;
     	end =false;
     	while(!end)
     	{
     		joystick->read_data();
     		type = joystick->get_type();
     		type &= ~JS_EVENT_INIT;
     		number = joystick->get_number();
     		value = joystick->get_value();
     		
     		if(type==JS_EVENT_BUTTON & number!=_kickSX & number!=_kickDX & number != _brake) {
     			if(value)
     				SetButtonKickFWD(number);
     			else{
     				end = true;
     			}
     		}
     	}
	}

	
	printf("\033[2J");		        /* Pulisce lo schermo */
 	printf("\033[8;0f");		/* Posiziono il cursore */
   	cout	 << "************************************" << endl
 	     << "********  SETUP TERMINATO  *********" << endl
 	     << "************************************" << endl;
   	cout << "************************************" <<endl
   	     << "*        Premi un pulsante         *" << endl
   	     << "*         per continuare           *" << endl
   	     << "************************************" << endl;
   	while(!end)
   	{
   		joystick->read_data();
 		type = joystick->get_type();
		type &= ~JS_EVENT_INIT;
		value = joystick->get_value();
   		if(type == JS_EVENT_BUTTON){
   			if(value)
   				;
   			else{
   				end = true;
   			}
   		}
   	}			
}