
//--------------- gjoycontrol.h --------------------

/** Class for the control of Golem robots via joystick
 *     2000  Nikita Scattolin, Golem Team
 *
 *  v. 1.0.1 needs
 *
 * needs recent joystick.h in /usr/include/linux
 */

#ifndef __GJOYCONTROL__
#define __GJOYCONTROL__

#include "joyhandler.h"
#include "GVector.h"

//#define _DEBUG_GJOYCONTROL_

// Valore massimo in ingresso dal joystick
#define MAX_VALUE 32767.0

#define MAX_SPEED 2000.0
#define MAX_JOG 1.0


class GJoyControl
{

public:

/*------------------   Costruttori e Distruttore   ____________________*/
	
	
	GJoyControl(char* DevName);
	
	
	~GJoyControl();	

/*------------------   Acquisizione ed interpretazione dati   ----------

	Il metodo legge l'input del device ed interpreta il suo significato.
	Essendo il fuzionamento on demand va chiamato il un ciclo prima di
	effettuare le leture.

-------------------------------------------------------------------------*/

	void ReadAndParse();
	
/*------------------   Metodi per la lettura dei dati  ----------------------------

	Tarmite joystick e' possibile controllare completamente il robot. Il movimento
	viene impostato tramite un vettore velocia' (modulo, direzione) ed un valore
	di jog (positivo per rotazione in senso antiorario).
	
	Il sistama di tiro puo' venire controllato con i pulasanti.
	
------------------------------------------------------------------------------------*/

	// Ritornano il numero di assi e pulsanti presenti sul joystick
	char numAxis();
	char numButtons();
	
	// Inserisce nel vettore  v(ro, theta) i valori letti dall'hardware
	// Se non viene premuto il pulsante del !freno ritorna v(0, 0)
	void ReadSpeed(GVector &v);
	// Ritorna il valore del jog
	float ReadJog();
	// se TRUE => kicker destro fuori
	bool ReadKickDX();
	// se TRUE => kicker sinistro fuori
	bool ReadKickSX();
	// se TRUE => entrambi i kicker fuori
	bool ReadKickFWD();
	
	// Metodi per controllare a che asse o pulsante sono associati i controlli
	// utili per controllare il setup.
	int AxesX();				// ritorna l'ID dell'asse X
	int AxesY();				// ritorna l'ID dell'asse Y
	int AxesJog();			// ritorna l'ID dell'asse Jog
	int ButtonBrake();		// ritorna l'ID del pulsante di START/BRAKE
	int ButtonKickDX();		// ritotna l'ID del pulsante per tirare di destro
	int ButtonKickSX();		// ritotna l'ID del pulsante per tirare di sinistro
	int ButtonKickFWD();		// ritotna l'ID del pulsante per tirare una bomba al centro
	
/*------------------   Metodi per l'impostazione dei controlli   -------------------

	La classe JoyHandler riconosce il numero di assi e pulsanti e li numera
	a pertire da 0. Tramite i seguenti metodi e' possibile configurare il joystick.
	
	I valori relativi agli assi variano tra -32767 e +32767, vanno poi scalati a
	valori compatibili col controllo motori. Per farlo utilizzo delle semplici
	costanti che possono venire corrette tramite i metodi SetKV e SetJog.
	
	E' previsto l'uso di un pulsante per frenare. Mantenendo premuto il pulsante
	viene disattivato il freno, rilasciando il pulsante sara' possibilie solamente
	ruotare.
	
------------------------------------------------------------------------------------*/
	
	void SetAxesX(int i);				// associa l'asse i alla coordinata X
	void SetAxesY(int i);				// associa l'asse i alla coordinata Y
	void SetAxesJog(int i);			// associa l'asse i al jog
	void SetButtonKickDX(int i);		// associa il pulsante i al kicker destro
	void SetButtonKickSX(int i);		// associa il pulsante i al kicker sinistro
	void SetButtonKickFWD(int i);		// associa il pulsante i al calcio in avanti
	void SetButtonBrake(int i);		// associa il pulsante i al freno
	
	void SetKV(float k);				// modifica la costante di proporzionalita' per la velocita'
	void SetKJog(float k);			// modifica la costante di proporzionalita' per il jog
	
	void inline InvertAxesX();		// Invertono il segno degli assi
	void inline InvertAxesY();
	void inline InvertAxesJog();
	
	void Setup();					// Effettua il Setup interattivo del joystick con output
									// su shell.
	
	
private:
	
	// Numero assi e buttoni
	char numAx, numBut;
	
	// variabili per il parsing
	int type, number, value;
	unsigned int time;
	
	// risultai del parsing
	float x,y,jog;
	bool kDX, kSX, kFWD;
	
	GVector speed;	// velocita'

	
	int _kickDX, _kickSX, _kickFWD;	// flags
	int _xAxes, _yAxes, _jogAxes;
	int _brake;
	
	bool stopped;
	
	char signX, signY, signJog;		// correzione del segno
	
	float _kjog, _kv;					// costanti di scala
	
	JoyHandler *joystick;				// gestore del joystick
	
};

#endif