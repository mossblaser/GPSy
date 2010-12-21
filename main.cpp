#include <mbed.h>
#include <stdlib.h>

#include "compass_widget.h"
#include "HMC6352.h"

#include "display.h"


int
main()
{
	Display display = Display();
	HMC6352 compass = HMC6352(p9, p10);
	CompassWidget compass_widget = CompassWidget(&display, 0,0,64);
	CompassWidget compass_widget_s = CompassWidget(&display, 64,32,32);
	CompassWidget compass_widget_ss = CompassWidget(&display, 64+32,32+16,16);
	
	display.clear();
	compass_widget.redraw();
	compass_widget_s.redraw();
	compass_widget_ss.redraw();
	
	int heading = 0;
	int bearing = 0;
	int distance = 0;
	int accuracy = 0;
	time_t last_update = time(NULL);
	
	while(1) {
		heading = compass.sample()/10;
		//bearing = (bearing+5)%360;
		compass_widget.set_heading(heading);
		compass_widget.set_bearing(bearing);
		compass_widget.set_distance(distance);
		compass_widget.set_accuracy(accuracy);
		compass_widget_s.set_heading(heading);
		compass_widget_s.set_bearing(bearing);
		compass_widget_s.set_distance(distance);
		compass_widget_s.set_accuracy(accuracy);
		compass_widget_ss.set_heading(heading);
		compass_widget_ss.set_bearing(bearing);
		compass_widget_ss.set_distance(distance);
		compass_widget_ss.set_accuracy(accuracy);
		
		compass_widget.update();
		compass_widget_s.update();
		compass_widget_ss.update();
		
		if (time(NULL) - last_update > 3) {
			distance = rand() % 10;
			distance *= distance;
			accuracy = rand() % 20;
			last_update = time(NULL);
			display.rc(0,11);
			display.printf("D: %d    ", distance);
			display.rc(1,11);
			display.printf("A: %d    ", accuracy);
		}
	}
}


