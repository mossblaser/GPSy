#include <mbed.h>
#include "display.h"
#include "widget.h"


Widget::Widget(Display *display)
	: display(display)
	, parent_focus(NULL)
	, focused(false)
{
	// Do nothing...
}


Widget::~Widget(void)
{
	// Do nothing...
}


void
Widget::give_focus(Widget *widget)
{
	// Unfocus this window
	focused = false;
	on_unfocus();
	
	// Pass focus to the child
	widget->focus(this);
}


void
Widget::release_focus(void)
{
	if (parent_focus != NULL) {
		// Unfocus this window
		focused = false;
		on_unfocus();
		
		// Pass focus back to the parent
		parent_focus->refocus();
	}
}


void
Widget::focus(Widget *parent)
{
	parent_focus = parent;
	refocus();
}


void
Widget::refocus(void)
{
	// Focus this window
	focused = true;
	on_focus();
}

void Widget::on_focus(void) { }
void Widget::on_unfocus(void) { }


bool
Widget::has_focus(void)
{
	return focused;
}
