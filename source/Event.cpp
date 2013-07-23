#include <Nephilim/Event.h>

NEPHILIM_NS_BEGIN

/// Returns true if the event refers to a pointer being pressed, like a touch or the mouse button
bool Event::isPointerPressed() const
{
	return (type == MouseButtonPressed || type == TouchPressed);
}

bool Event::isPointerType()
{
	return (type == MouseButtonPressed || type == TouchPressed || type == MouseButtonReleased || type == TouchReleased || type == MouseMoved || type == TouchMoved); 
}

vec2i Event::getPointerPosition() const
{
	if(type == MouseButtonPressed)
	{
		return Vec2i(mouseButton.x, mouseButton.y);
	}
	else if(type == MouseButtonReleased)
	{
		return Vec2i(mouseButton.x, mouseButton.y);
	}
	else if(type == MouseMoved)
	{
		return Vec2i(mouseMove.x, mouseMove.y);
	}
	else if(type == TouchPressed || type == TouchReleased || type == TouchMoved)
	{
		return Vec2i((int)touch.x, (int)touch.y);
	}
	else return Vec2i(0,0);
}

NEPHILIM_NS_END
