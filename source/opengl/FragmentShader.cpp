#include <opengl/FragmentShader.h>

#include <tools/logger/Logger.h>
using namespace tools::logger;

namespace opengl
{
	FragmentShader::FragmentShader() : Shader( Shader::Fragment )
	{
	}
	
	FragmentShader::~FragmentShader()
	{
	}
	
	Shader::Type FragmentShader::getShaderType() const
	{
		return Shader::Fragment;
	}
}

