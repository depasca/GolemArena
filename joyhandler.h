	
 // ------------------ joyhandler.h ------------------------	

 /*joystick interface for Golem control
 *	 2000 Nikita Scattolin
 *
 * needs recent joystick.h in /usr/include/linux
 *
 * device defaults to /dev/js0
 */

#ifndef _JOY_HANDLER_H_
#define _JOY_HANDLER_H_

#include <sys/ioctl.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <iostream.h>
#include <errno.h>
#include <string.h>

#include <linux/joystick.h>

#define NAME_LENGTH 128
#define JS_SELECT_UDELAY 25000


class JoyHandler {

public:

	JoyHandler( char* device);

	~JoyHandler();
	int read_data();			// read data from device
	int get_type();			// input type (axix or button)
	int get_number();			// input number
	int get_value();			// input valule
	unsigned int get_time();	// time of event
	
	char num_buttons();		// Number of button aviable
	char num_axis();			// Number of axes aviable
	
	void set_nblock();


private:

	char devname[NAME_LENGTH];
	struct js_event	js;
	struct timeval tv;
	int fd;
	int version;
	char axis;
	char buttons;
	char name[NAME_LENGTH];
	bool option_plain;
};	
#endif