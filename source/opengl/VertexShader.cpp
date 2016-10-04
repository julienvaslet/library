#include <opengl/VertexShader.h>

#include <tools/logger/Logger.h>
using namespace tools::logger;

namespace opengl
{
	VertexShader::VertexShader() : Shader( Shader::Vertex )
	{
	}
	
	VertexShader::~VertexShader()
	{
	}
	
	Shader::Type VertexShader::getShaderType() const
	{
		return Shader::Vertex;
	}
}

