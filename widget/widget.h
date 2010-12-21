#ifndef WIDGET_H
#define WIDGET_H

#include "display.h"

/**
 * A pure-virtual class which defines the interface for GUI widgets.
 */
class Widget {
	public:
		Widget(Display *display);
		virtual ~Widget(void);
		
		/**
		 * Redraw assuming that the target canvas has been cleared with the
		 * expected background colour.
		 */
		virtual void redraw(void) = NULL;
		
		/**
		 * Redraw assuming that the target canvas contains the image from the last
		 * time update or redraw was called (this can be used to optimise out
		 * repeated drawing calls or minimize changes by only adding changed info).
		 */
		virtual void update(void) = NULL;
		
		/**
		 * Give focus to the given Widget.
		 */
		virtual void give_focus(Widget *widget);
		
		/**
		 * Return focus to the parent window.
		 */
		virtual void release_focus(void);
		
		/**
		 * Called whenver this Widget recieves focus.
		 */
		virtual void on_focus(void);
		
		/**
		 * Called whenever the Widget looses focus.
		 */
		virtual void on_unfocus(void);
		
		/**
		 * Does this Widget currently have focus?
		 */
		virtual bool has_focus(void);
	
	protected:
		/** The display to draw onto. */
		Display *display;
	
	private:
		/**
		 * The parent Widget has given this Widget focus, a NULL parent means this
		 * Widget is the root.
		 */
		virtual void focus(Widget *parent);
		
		/**
		 * A child has released the focus.
		 */
		virtual void refocus(void);
		
		Widget *parent_focus;
		
		bool focused;
};


#endif
