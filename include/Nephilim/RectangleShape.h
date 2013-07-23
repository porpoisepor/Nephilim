#ifndef NephilimRectangleShape_h__
#define NephilimRectangleShape_h__

#include "Platform.h"
#include "Drawable.h"
#include "Color.h"
#include "Transformable.h"
#include "VertexArray2D.h"

NEPHILIM_NS_BEGIN

class Texture;

/**
	\ingroup Graphics
	\class RectangleShape
	\brief A drawable rectangle
*/
class NEPHILIM_API RectangleShape : public Drawable, public Transformable
{
public:
	RectangleShape();

	/// Check if a given point lies inside the shape
	bool contains(vec2 point);

	/// Set the rect of this rectangle
	void setRect(FloatRect rect);

	/// Get the size of the rectangle
	vec2 getSize();

	/// Set the size of the rectangle
	void setSize(const vec2& size);

	void setSize(float width, float height);

	void setOutlineColor(const Color& outlineColor);

	void setOutlineThickness(float thickness);

	void setColor(const Color& color);

	void setColors(const Color& topleft, const Color& topright, const Color& bottomright, const Color& bottomleft);

	void setTexture(Texture* texture);

	/// Set the sub region of the texture to display. These coordinates are in pixels and are normalized internally
	void setTextureRect(float left, float top, float width, float height);

	/// Invert the vertical coordinates of the texture - hacky
	void invertTextureCoordinates();

protected:

	float m_width;
	float m_height;

	/// Draw with the renderer.draw(object) syntax
	virtual void onDraw(Renderer* renderer);

	VertexArray2D m_geometry;
	VertexArray2D m_outline;

	float m_outlineThickness;

	Texture* m_texture;

private:
	void updateInternalOutline();
};

NEPHILIM_NS_END
#endif // NephilimRectangleShape_h__
