
 //------------------ joyhandler.cpp -------------------
	
 /*joystick interface for Golem control
 *	 2000 Nikita Scattolin
 *
 * needs recent joystick.h in /usr/include/linux
 *
 * device defaults to /dev/js0
 */

#include "joyhandler.h"

JoyHandler::JoyHandler(char* device = "/dev/js0") {
	
	strcpy(name, "Unknown");
	buttons = 2;
	axis = 2;
	version = 0x000800;
	strcpy(devname, device);
	
	if ((fd = open(devname, O_RDONLY)) < 0) {
		char msg[128];
		strcpy(msg, "\nERROR opening ");
		strcat(msg, devname);
		perror(msg);
		exit(1);
	}
	
		
	// Get information from device
	ioctl(fd, JSIOCGVERSION, &version);
	ioctl(fd, JSIOCGAXES, &axis);
	ioctl(fd, JSIOCGBUTTONS, &buttons);
	ioctl(fd, JSIOCGNAME(NAME_LENGTH), name);
	

	cout << "\n** Joystick handler message **\n\n";
	cout << "Driver version is: " << (version >> 16) << "."
								<< ((version >> 8) & 0xff) << "."
								<< (version & 0xff) << endl;

	cout << "Joystick: " << name
		<< ", Axis: " << axis+0
		<< ", Buttons: " << buttons+0 << endl;
	cout << "\n**           **             **\n\n";	
	
}

JoyHandler::~JoyHandler() {
	if(fd != -1)
		close(fd);
}

int JoyHandler::read_data() {
	if (fd != -1){
		int readrc = read(fd, &js, sizeof(struct js_event));
		return readrc;
	}else return 0;
}

int JoyHandler::get_type() {return (int)js.type;}

int JoyHandler::get_number() {return (int)js.number;}

int JoyHandler::get_value() {return (int)js.value;}

unsigned int JoyHandler::get_time() {return (unsigned int)js.time;}

char JoyHandler::num_buttons() {return buttons;}

char JoyHandler::num_axis() {return axis;}

void JoyHandler::set_nblock() {
	fcntl(fd, F_SETFL, O_NONBLOCK);
}

