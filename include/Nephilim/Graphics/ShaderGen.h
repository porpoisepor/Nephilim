#ifndef ShaderGen_h__
#define ShaderGen_h__

#include <Nephilim/Platform.h>
#include <Nephilim/Foundation/String.h>

NEPHILIM_NS_BEGIN

class GLShader;

/**
	\ingroup Graphics
	\class ShaderGen
	\brief GLSL tools for generating portable source code
*/
class NEPHILIM_API ShaderGen
{
public:

	/// Prepares the default shader
	static bool	prepareDefault(GLShader& shader);

	/// Returns the source code for the default vertex shader
	static String getDefaultVertexShader();

	/// Returns the source code for the default fragment shader
	static String getDefaultFragmentShader();
};

class NEPHILIM_API ShaderComposer
{
public:
	/// Starts a clear source with VertexUnit composing mode
	ShaderComposer();

	enum ShaderType
	{
		VertexUnit,
		FragmentUnit
	};

	/// Change the composer into another shader type
	void setUnitType(ShaderType shaderType);

	/// Clear the source
	void clear();

	/// Declare a vertex attribute array in the shader source
	void addAttribute(const String& name);

private:
	String m_source;
};

NEPHILIM_NS_END
#endif // ShaderGen_h__
