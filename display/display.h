#ifndef DISPLAY_H
#define DISPLAY_H


/**
 * A library for controlling a ByVac BV4511 display. Connects to the display via
 * pins 13 (to display RX) and pin 12 for flow control.
 */
class Display : public Serial
{
	public:
		static const int WIDTH = 128;
		static const int HEIGHT = 64;
		static const int CHWIDTH = 21;
		static const int CHHEIGHT = 8;
		
	public:
		Display(void);
		virtual ~Display(void);
		
		/**
		 * Reset the screen to a known state (but don't clear it). This state is:
		 *  - Normal Font
		 *  - Normal Colour/Inversion
		 *  - Cursor at the top-left.
		 *  - Backlight On
		 */
		void reset(void);
		
		/**
		 * Clear the screen and place the cursor in the home position. The screen is
		 * cleared with the inverse of the foreground colour.
		 */
		void clear(void);
		
		/** Enable/disable the backlight. */
		void backlight(bool state);
		
		/** Enable/disable the bold font. */
		void bold(bool state);
		
		/** Invert the screen (applies to future commands only). */
		void invert(bool state);
		
		/**
		 * Set the colour to draw. When the screen is not inverted, false means
		 * black, true means white.
		 */
		void colour(bool colour);
		
		/** Set the pixel cursor to the given pixel coordinates. */
		void xy(int x, int y);
		
		/** Set the character cursor to the given row and column. */
		void rc(int r, int c);
		
		/** Draw a pixel at the specified position */
		void pixel(int x, int y);
		
		/** Draw a line between the specified points. */
		void line(int x1, int y1, int x2, int y2);
		
		/**
		 * Draw a rectangle, optionally filled, with the corners at the given
		 * points.
		 */
		void rect(bool filled, int x1, int y1, int x2, int y2);
		
		/** Draw a circle with the given center and radius. */
		void circle(int x, int y, int radius);
		
	private:
		bool is_bold;
		bool is_inverted;
		bool is_colour;
		bool is_backlight;
};


#endif
