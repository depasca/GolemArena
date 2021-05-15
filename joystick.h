#ifndef __JOYSTICK_H__
#define __JOYSTICK_H__

#include "joycontrol.h"

GJcoyControl gj = new GJoyControl("dev/js0");

void readjoystick()
{
	ReadAndParse();
	float jog;
	vector speed;
	ReadJog(jog);
	ReadSpeed(speed);