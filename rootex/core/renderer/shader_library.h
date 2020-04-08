#pragma once

#include "common/common.h"
#include "shader.h"

/// does Shader caching
class ShaderLibrary
{
	enum class ShaderType
	{
		Default,
		Diffuse,
		CPUParticles
	};

private:
	/// New shaders are stored in this hash map, existing once are retrieved
	static HashMap<ShaderType, Ptr<Shader>> s_Shaders;

	/// Deals with the hash map
	static Shader* MakeShader(ShaderType shaderType, const LPCWSTR& vertexPath, const LPCWSTR& pixelPath, const BufferFormat& vertexBufferFormat);

public:
	/// Load all shaders
	static void MakeShaders();
	/// Unload all shaders
	static void DestroyShaders();

	static Shader* GetDefaultShader();
	static DiffuseShader* GetDiffuseShader();
	static CPUParticlesShader* GetCPUParticlesShader();
};
