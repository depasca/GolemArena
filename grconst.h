#ifndef __GRCONST_H__
#define __GRCONST_H__

#include "geometry.h"

/* parametri di rete e indirizzi di sottorete dei robots */
#define OWNADDRESS "127.0.0.0"
#define ADDRESS "192.168.77.255"
#define DEFAULTPORT 1800
#define ARCHIMEDE_ID 2
#define ERONE_ID 1
#define EUCLIDE_ID 3
#define EULERO_ID 4
#define GOLEM_1 "ERONE"
#define GOLEM_2 "ARCHIMEDE"
#define GOLEM_3 "EUCLIDE"
#define GOLEM_4 "EULERO"

/* che cosa disegnamo oggi ? */
#define GR_DRAW_SPEED_VECTORS true
#define GR_DRAW_BGCIRCLE true
#define GR_MIN_SPEED_DRAWED 50.0

/* parametri del main.c */
#define MILLI_PERIOD 50
#define MICRO_PERIOD MILLI_PERIOD*1000 
#define NANO_PERIOD MICRO_PERIOD*1000
#define FORCE_REDRAW_PERIOD false
#define WINDOW_W 350
#define WINDOW_H 350
#define SUBWIN_X 1.18
#define SUBWIN_Y 0.17
#define USE_DEPTH_TEST true

#define GRRADAR_RADIUS 10000.0

/* parametri del Parser */
#define GR_BCMEMBERS 6
#define CONFIRM_OUTGOING_MESSAGES true
#define BATTERY_CHARGE_MIN 22.0
#define BATTERY_CHARGE_MAX 28.0
#define BATTERY_CHARGE_RANGE  (BATTERY_CHARGE_MAX-BATTERY_CHARGE_MIN)
#define GAS_CHARGE_MIN 0.0
#define GAS_CHARGE_MAX 50.0
#define GAS_CHARGE_RANGE  (GAS_CHARGE_MAX-GAS_CHARGE_MIN)

/* parametri per il BackGround */
#define GRZ_UP 10.0
#define GRTACCA 300.0
#define GRRADAR_SLICES 50

/* parametri per gli ogggetti */
#define GROBJECT_RADIUS 300.0
#define GRTARGET_RADIUS 300.0
#define GROBJ_SLICES 6
#define GRTARGET_SLICES 5

/* parametri per la palla */
#define GRBALL_RADIUS 150.0

/* parametri per i pali */
#define GRGOALPOST_WIDTH 300.0


#endif

