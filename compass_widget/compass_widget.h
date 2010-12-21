#ifndef COMPASS_WIDGET_H
#define COMPASS_WIDGET_H

#include "widget.h"


/**
 * A widget which provides an advanced compass which shows bearing, heading,
 * accuracy and target distance.
 *
 * The outer ring has a double line which points north (the heading). The arrow
 * points towards the next bearing. The inner ring's distance from the outer
 * ring is proportional to the differrence between accuracy and distance. If the
 * two circles meet then the distance to the target is < the GPS accuracy.
 */
class CompassWidget : public Widget {
	private:
		static const double PI = 3.1415926535;
		
		/** Size in pixels of the dashes on the outer ring. */
		static const int COMPASS_DASH_SIZE = 4;
		
		/** Seperation of the north dashes on the outer ring in radians. */
		static const double COMPASS_NDASH_SEP = 2.0*PI / 50.0;
		
		
		/** The seperation between the bottom points of the arrow in radians. */
		static const double COMPASS_ARROW_SEP = 2.0*PI / 8.0;
		
		/** The size of the arrow 0-1 relative to the compass size/2. */
		static const double COMPASS_ARROW_SIZE = 0.6;
		
		/** The size of the bottom dent 0-1 relative to the compass size/2. */
		static const double COMPASS_ARROW_DEPTH = 0.3;
		
		
		/**
		 * The number of seconds the display can remain not-updated if the midi
		 * thresholds have not been passed but the mini thresholds have been met.
		 */
		static const int MIDI_UPDATE_TIMEOUT = 1;
		
		
		/**
		 * A set of thresholds for medium changes. If these thresholds are not
		 * passed then the screen will not be updated until MIDI_UPDATE_TIMEOUT
		 * secconds have passed.
		 */
		static const int MIDI_UPDATE_BEARING_THRESHOLD = 10;
		static const int MIDI_UPDATE_HEADING_THRESHOLD = 10;
		static const int MIDI_UPDATE_DISTANCE_THRESHOLD = 3;
		static const int MIDI_UPDATE_ACCURACY_THRESHOLD = 1;
		
		
		/**
		 * A set of thresholds for mini changes. If these thresholds are not met
		 * then the screen will not update. This serves to prevent screen updates
		 * when the angle changes by an ammount that would not be visible.
		 */
		static const int MINI_UPDATE_BEARING_THRESHOLD = 3;
		static const int MINI_UPDATE_HEADING_THRESHOLD = 3;
		static const int MINI_UPDATE_DISTANCE_THRESHOLD = 0;
		static const int MINI_UPDATE_ACCURACY_THRESHOLD = 0;
	
	
	public:
		CompassWidget(Display *display, int x, int y, int size);
		virtual ~CompassWidget(void);
		
		virtual void redraw(void);
		virtual void update(void);
		
		/** Set the direction we're heading in (i.e. angle from north) degrees. */
		virtual void set_heading(int heading);
		
		/** Set the angle from north of the current target (bearing) degrees. */
		virtual void set_bearing(int bearing);
		
		/** Set the distance of the current target (meters). */
		virtual void set_distance(int distance);
		
		/** Set the accuracy of position data (meters). */
		virtual void set_accuracy(int accuracy);
	
	
	private:
		int x;
		int y;
		int size;
		
		int heading;
		int bearing;
		int distance;
		int accuracy;
		
		/** Old data values used for un-drawing when updating the screen. */
		int last_heading;
		int last_bearing;
		int last_distance;
		int last_accuracy;
		time_t last_redraw;
		
		
		/**
		 * Draw the compass on the screen using the provided bearing, heading,
		 * distance and accuracy.
		 */
		void draw_all(int bearing, int heading, int distance, int accuracy);
		
		/** Draw the outer compass rotated by the given ammount. */
		void draw_heading(int deg_angle);
		
		
		/** Draw the arrow rotated by the given ammount. */
		void draw_bearing(int deg_angle);
		
		
		/** Convert from degrees to radians. */
		double deg2rad(int deg);
		
		
		/**
		 * Get the pixel coordinates of a point at a degrees from vertical and r
		 * pixels from the center of the widget.
		 */
		void get_xy_at_angle_radius(double a, int r, int *x, int *y);
		
		/**
		 * Draw a line at the given angle starting from the given radius and of a
		 * given length.
		 */
		void draw_radial_line(double a, int r, int length);
};


#endif
