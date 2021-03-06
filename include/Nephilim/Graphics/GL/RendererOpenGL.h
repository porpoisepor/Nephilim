#ifndef RendererOpenGL_h__
#define RendererOpenGL_h__

#include <Nephilim/Platform.h>
#include <Nephilim/Graphics/GraphicsDevice.h>
#include <Nephilim/Graphics/GL/GLShader.h>

NEPHILIM_NS_BEGIN

/**
	\class RendererOpenGL
	\brief Renderer specialization for desktop platform's OpenGL
*/
class NEPHILIM_API RendererOpenGL : public GraphicsDevice
{
public:

	/// All functional shader programs are kept here for safe destruction
	std::vector<GDI_ShaderProgram*> _shaderPrograms;

public:

	/// Initialize OpenGL renderer
	RendererOpenGL();

	/// This function will create a new shader program from the raw code
	/// It runs a preprocessing step to identify what portions belong to what shader
	virtual GDI_ShaderProgram* createShader(const String& code);

	/// Push client-side geometry to the GPU
	/// This is usually slower than using a VBO because the data is uploaded to the GPU every time
	/// Note: This will eventually be the only way to render client-side data
	virtual void draw(const VertexArray& vertexData);

	/// Draw a vertex array
	virtual void draw(const VertexArray2D& varray, const RenderState& state);

	/// This will cancel all shader-related settings and activate the default shader/fixed pipeline
	virtual void setDefaultShader();

	/// Activates the shader for the next drawing calls
	virtual void setShader(Shader& shader);

	/// Activate a projection matrix
	virtual void setProjectionMatrix(const mat4& projection);

	/// Activate a view matrix
	virtual void setViewMatrix(const mat4& view);

	/// Activate a model matrix
	virtual void setModelMatrix(const mat4& model);

private:
	GLShader m_defaultShader;
};

NEPHILIM_NS_END
#endif // RendererOpenGL_h__