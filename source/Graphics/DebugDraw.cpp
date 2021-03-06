#include <Nephilim/Graphics/DebugDraw.h>
#include <Nephilim/Graphics/GraphicsDevice.h>
#include <Nephilim/Graphics/GL/GLHelpers.h>
#include <Nephilim/Foundation/Logging.h>
#include <Nephilim/Graphics/Geometry.h>
#include <Nephilim/Foundation/Math.h>

NEPHILIM_NS_BEGIN

DebugDraw::DebugDraw(GraphicsDevice* graphics)
: _graphics(graphics)
{

}

/// Draw a colored box
void DebugDraw::drawBox(float x, float y, float z, float size, Color a)
{
	_graphics->setDefaultTexture();

	GeometryObject box;
	box.addBox(size, size, size);
	box.setAllColors(a);
	box.generateNormals();

	_graphics->setModelMatrix(mat4::translate(x, y, z));
	_graphics->draw(box);

	/*box.setAllColors(Color::Black);
	box.m_primitive = Render::Primitive::LineLoop;
	_graphics->draw(box);*/
}

NEPHILIM_NS_END