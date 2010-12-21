#include <mbed.h>
#include <math.h>
#include "display.h"
#include "compass_widget.h"

#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#define MAX(x,y) (((x) > (y)) ? (x) : (y))


CompassWidget::CompassWidget(Display *display, int x, int y, int size)
	: Widget(display)
	, x(x)
	, y(y)
	, size(size)
	, heading(0)
	, bearing(0)
	, distance(0)
	, accuracy(0)
{
	// Do nothing...
}


CompassWidget::~CompassWidget(void)
{
	// Do nothing
}


void
CompassWidget::set_heading(int heading)
{
	this->heading = heading;
}


void
CompassWidget::set_bearing(int bearing)
{
	this->bearing = bearing;
}


void
CompassWidget::set_distance(int distance)
{
	this->distance = distance;
}


void
CompassWidget::set_accuracy(int accuracy)
{
	this->accuracy = accuracy;
}


void
CompassWidget::redraw(void)
{
	draw_all(bearing, heading, distance, accuracy);
	
	last_heading = heading;
	last_bearing = bearing;
	last_distance = distance;
	last_accuracy = accuracy;
	last_redraw  = time(NULL);
}


void
CompassWidget::update(void)
{
	if (abs(bearing - last_bearing) >= MIDI_UPDATE_BEARING_THRESHOLD
	    || abs(heading - last_heading) >= MIDI_UPDATE_HEADING_THRESHOLD
	    || abs(distance - last_distance) >= MIDI_UPDATE_DISTANCE_THRESHOLD
	    || abs(accuracy - last_accuracy) >= MIDI_UPDATE_ACCURACY_THRESHOLD
	    || ((time(NULL) - last_redraw >= MIDI_UPDATE_TIMEOUT)
	        && (abs(bearing - last_bearing) > MINI_UPDATE_BEARING_THRESHOLD
	            || abs(heading - last_heading) > MINI_UPDATE_HEADING_THRESHOLD
	            || abs(distance - last_distance) > MINI_UPDATE_DISTANCE_THRESHOLD
	            || abs(accuracy - last_accuracy) > MINI_UPDATE_ACCURACY_THRESHOLD)
	       )) {
		// Draw over the old compass in white
		display->invert(true);
		draw_all(last_bearing, last_heading, last_distance, last_accuracy);
		display->invert(false);
		
		redraw();
	}
}


void
CompassWidget::get_xy_at_angle_radius(double a, int r, int *x, int *y)
{
	(*x) = (int)(((double)r) * cos((PI/2.0) - a));
	(*y) = (int)(-((double)r) * sin((PI/2.0) - a));
}


void
CompassWidget::draw_radial_line(double a, int r, int length)
{
	int x1, x2, y1, y2;
	
	get_xy_at_angle_radius(a, r, &x1, &y1);
	get_xy_at_angle_radius(a, r + length, &x2, &y2);
	
	x1 += x + (size/2);
	x2 += x + (size/2);
	y1 += y + (size/2);
	y2 += y + (size/2);
	
	display->line(x1,y1, x2,y2);
}


double
CompassWidget::deg2rad(int deg)
{
	return (((double)deg) / 180.0) * PI;
}


void
CompassWidget::draw_heading(int deg_angle)
{
	double heading = deg2rad(deg_angle);
	
	// North
	draw_radial_line(heading - (COMPASS_NDASH_SEP/2.0),
	                 (size/2) - COMPASS_DASH_SIZE, COMPASS_DASH_SIZE);
	draw_radial_line(heading + (COMPASS_NDASH_SEP/2.0),
	                 (size/2) - COMPASS_DASH_SIZE, COMPASS_DASH_SIZE);
	
	// South
	draw_radial_line(heading + PI,
	                 (size/2) - COMPASS_DASH_SIZE, COMPASS_DASH_SIZE);
	
	// West
	draw_radial_line(heading - (PI/2.0),
	                 (size/2) - COMPASS_DASH_SIZE, COMPASS_DASH_SIZE);
	
	// East
	draw_radial_line(heading + (PI/2.0),
	                 (size/2) - COMPASS_DASH_SIZE, COMPASS_DASH_SIZE);
}


void
CompassWidget::draw_bearing(int deg_angle)
{
	double bearing = deg2rad(deg_angle);
	
	// Tip of the arrow
	int tipx, tipy;
	get_xy_at_angle_radius(bearing, (size/2) * COMPASS_ARROW_SIZE, &tipx, &tipy);
	
	// Recessed bottom of the arrow
	int btmx, btmy;
	get_xy_at_angle_radius(bearing + PI, (size/2) * COMPASS_ARROW_DEPTH,
	                       &btmx, &btmy);
	
	// Left bottom point of arrow
	int leftx, lefty;
	get_xy_at_angle_radius(bearing + PI - (COMPASS_ARROW_SEP/2.0),
	                       (size/2) * COMPASS_ARROW_SIZE,
	                       &leftx, &lefty);
	
	// Right bottom point of arrow
	int rightx, righty;
	get_xy_at_angle_radius(bearing + PI + (COMPASS_ARROW_SEP/2.0),
	                       (size/2) * COMPASS_ARROW_SIZE,
	                       &rightx, &righty);
	
	leftx += x + (size/2);
	rightx += x + (size/2);
	tipx += x + (size/2);
	btmx += x + (size/2);
	
	lefty += y + (size/2);
	righty += y + (size/2);
	tipy += y + (size/2);
	btmy += y + (size/2);
	
	display->line(leftx, lefty, tipx, tipy);
	display->line(rightx, righty, tipx, tipy);
	
	display->line(leftx, lefty, btmx, btmy);
	display->line(rightx, righty, btmx, btmy);
}


void
CompassWidget::draw_all(int bearing, int heading, int distance, int accuracy)
{
	display->colour(false);
	
	// Main outer circle
	display->circle(x + (size/2), y + (size/2), (size/2) - (COMPASS_DASH_SIZE/2));
	
	// Dashes round the edge
	draw_heading(-heading);
	
	// Arrow
	draw_bearing(-(heading + bearing));
	
	// Inner ring for distance/accuracy
	double ratio;
	if (distance != 0)
		ratio = ((double)accuracy) / ((double)distance);
	else
		ratio = 1.0;
	
	display->circle(x + (size/2), y + (size/2),
	                ((double)((size/2) - COMPASS_DASH_SIZE)) * MAX(MIN(ratio, 1.0),0.1));
}
