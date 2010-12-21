#include <mbed.h>
#include "display.h"

#define ESC "\033"

#define MIN(x,y) (((x) < (y)) ? (x) : (y))
#define MAX(x,y) (((x) > (y)) ? (x) : (y))

#define CROP_WIDTH(x) MIN(MAX(x, 0), WIDTH - 1)
#define CROP_HEIGHT(y) MIN(MAX(y, 0), HEIGHT - 1)
#define CROP_CHWIDTH(x) MIN(MAX(x, 0), CHWIDTH - 1)
#define CROP_CHHEIGHT(x) MIN(MAX(x, 0), CHHEIGHT - 1)

Display::Display(void)
	: Serial(p13, NC)
	, is_bold(NULL)
	, is_inverted(NULL)
	, is_colour(NULL)
	, is_backlight(NULL)
{
	// Set CTSen to 1 to enable CTS for UART1
	LPC_UART1->MCR |= 1<<7;
	// Make pin 12 into CTS pin for UART1
	LPC_PINCON->PINSEL1 = ((LPC_PINCON->PINSEL1) & ~(3<<2)) | 1<<2;	
	
	baud(115200);
	putc('\r');
	wait(0.5);
	
	reset();
	clear();
}


Display::~Display(void)
{
	// Do nothing.
}


void
Display::reset(void)
{
	xy(0, 0);
	
	// Force bold off
	is_bold = true;
	bold(false);
	
	// Normal, Black Colour
	is_colour = false;
	is_inverted = true;
	invert(false);
	colour(false);
	
	// Backlight on
	is_backlight = false;
	backlight(true);
}


void
Display::clear(void)
{
	printf(ESC"[2J");
}


void
Display::backlight(bool state)
{
	if (is_backlight != state) {
		printf(ESC"[?26%c", state ? 'h' : 'I');
		is_backlight = state;
	}
}


void
Display::bold(bool state)
{
	if (is_bold != state) {
		printf(ESC"(%c", state ? '2' : '1');
		is_bold = state;
	}
}


void
Display::invert(bool state)
{
	if (is_inverted != state) {
		// Invert the colour
		colour(!is_colour);
		is_inverted = state;
	}
}


void
Display::colour(bool state)
{
	if (is_colour != state) {
		printf(ESC"[%cf", (is_inverted ^ state) ? '0' : '1');
		is_colour = state;
	}
}


void
Display::xy(int x, int y)
{
	printf(ESC"{%d;%dp", CROP_HEIGHT(y), CROP_WIDTH(x));
}


void
Display::rc(int r, int c)
{
	printf(ESC"[%d;%dH", CROP_CHHEIGHT(r), CROP_CHWIDTH(c));
}


void
Display::pixel(int x, int y)
{
	printf(ESC"{%d;%dP", CROP_HEIGHT(y), CROP_WIDTH(x));
}


void
Display::line(int x1, int y1, int x2, int y2)
{
	if (x1 != x2 || y1 != y2)
		printf(ESC"{%d;%d;%d;%dL", CROP_HEIGHT(y1), CROP_WIDTH(x1),
		                           CROP_HEIGHT(y2), CROP_WIDTH(x2));
}


void
Display::rect(bool filled, int _x1, int _y1, int _x2, int _y2)
{
	int x1, x2, y1, y2;
	x1 = MIN(_x1, _x2);
	x2 = MAX(_x1, _x2);
	y1 = MIN(_y1, _y2);
	y2 = MAX(_y1, _y2);
	
	printf(ESC"{%d %d %d %d%c", CROP_HEIGHT(y1), CROP_WIDTH(x1),
	                            CROP_HEIGHT(y2), CROP_WIDTH(x2),
	                            filled ? 'F' : 'B');
}


void
Display::circle(int x, int y, int r)
{
	y = CROP_HEIGHT(y);
	x = CROP_WIDTH(x);
	
	if (r + x > WIDTH)
		r = WIDTH - x;
	
	if (r + y > HEIGHT)
		r = HEIGHT - y;
	
	if (x - r < 0)
		r = x;
	
	if (y - r < 0)
		r = y;
	
	if (r <= 0)
		r = 1;
	
	printf(ESC"{%d;%d;%dC", y, x, r);
}
