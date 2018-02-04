#ifndef VFRAME_NO_3D

#define GLEW_STATIC
#include "depend/glew.h"

#include "V3DLightShader.h"
#include "V3DCamera.h"

V3DLightShader::V3DLightShader(V3DCamera* camera) : cam(camera)
{
	const std::string vertexShader =
		"#version 330\n"\
		"attribute vec3 position;\n" \
		"attribute vec3 normal;\n" \
		"attribute vec4 color;\n" \
		"attribute vec2 texCoord;\n" \
		"uniform mat4 pvm;\n" \

		"varying vec3 pos;\n" \
		"varying vec3 norm;\n" \
		"varying vec4 diffuse;\n" \
		"varying vec2 uv;\n" \

		"void main() {\n" \
		"	gl_Position = pvm * vec4(position, 1.0);\n" \

		"	pos = position;\n" \
		"	norm = normal;\n" \
		"	diffuse = color;\n" \
		"	uv = texCoord;\n" \
		"}";

	const std::string fragShader =
		"#version 330\n" \
		"uniform sampler2D texture;\n" \
		"uniform vec3 camPos;\n" \

		"varying vec3 pos;\n" \
		"varying vec3 norm;\n" \
		"varying vec4 diffuse;\n" \
		"varying vec2 uv;\n" \

		"uniform mat4 transform;\n" \

		"struct Material {\n" \
		"vec3 specular;\n" \
		"float shininess;\n" \
		"};\n" \
		"uniform Material material;\n" \

		"#define MAX_LIGHTS " + std::to_string(LIGHT_COUNT) + "\n" \
		"#define DIRLIGHT " + std::to_string(static_cast<int>(LightType::DIRECTION)) + "\n" \
		"#define POINTLIGHT " + std::to_string(static_cast<int>(LightType::POINT)) + "\n" \
		"#define SPOTLIGHT " + std::to_string(static_cast<int>(LightType::SPOT)) + "\n" \
		"struct LightData {\n" \
		"vec3 pos;\n" \
		"vec3 dir;\n" \
		"vec3 tint;\n" \
		"float cutOff;\n" \
		"float att;\n" \
		"float ambientCo;\n" \
		"int type;\n" \
		"int enabled;\n" \
		"};\n" \
		"uniform LightData lights[MAX_LIGHTS];\n" \

		"vec3 ApplyLight(LightData light, vec3 surface, vec3 position, vec3 normal, vec3 toCamera) {\n" \
		"	vec3 toLight;\n" \
		"	float attenuation = 1.0;\n" \
		"	if (light.type == POINTLIGHT) {\n" \
		"		toLight = normalize(light.pos.xyz - position);\n" \
		"		float dist = length(light.pos.xyz - position);\n" \
		"		attenuation = 1.0 / (1.0 + light.att * pow(dist, 2));\n" \
		"	}\n" \
		"	else if (light.type == SPOTLIGHT) {\n" \
		"		toLight = normalize(light.pos.xyz - position);\n" \
		"		float dist = length(light.pos.xyz - position);\n" \
		"		attenuation = 1.0 / (1.0 + light.att * pow(dist, 2));\n" \

		"		float angle = degrees(acos(dot(-toLight, normalize(light.dir))));\n" \
		"		if (angle > light.cutOff)\n" \
		"			attenuation = 0.0;\n" \
		"	}\n" \
		"	else { //DIRLIGHT\n" \
		"		toLight = normalize(light.pos.xyz);\n" \
		"	}\n" \

		"   vec3 amb = light.ambientCo * surface.rgb * light.tint;\n" \
		
		"	float diffuseCo = max(0.0, dot(normal, toLight));\n" \
		"	vec3 diff = diffuseCo * surface.rgb * light.tint;\n" \

		"	float specularCo = 0.0;\n" \
		"	if (diffuseCo > 0.0)\n" \
		"		specularCo = pow(max(0.0, dot(toCamera, reflect(-toLight, normal))), material.shininess);\n" \
		"	vec3 spec = specularCo * material.specular * light.tint;\n" \

		"	return amb + attenuation * (diff + spec);\n" \
		"}\n" \

		"void main() {\n" \
		"	vec3 position = vec3(transform * vec4(pos, 1));\n" \
		"	vec3 normal = normalize(transpose(inverse(mat3(transform))) * norm);\n" \
		"	vec3 toCam = normalize(camPos - position);\n" \
		"	vec4 surfaceColor = texture2D(texture, uv) * diffuse;\n" \

		"	vec3 linearColor = vec3(0);\n" \
		"	for (int i = 0; i < MAX_LIGHTS; i++) {\n" \
		"		if (lights[i].enabled > 0)\n" \
		"			linearColor += ApplyLight(lights[i], surfaceColor.rgb, position, normal, toCam);\n" \
		"	}\n" \

		"	vec3 gamma = vec3(1.0 / 2.2);" \
		"	gl_FragColor = vec4(pow(linearColor, gamma), surfaceColor.a);\n" \
		"}";

	LoadFromMemory(vertexShader, fragShader);
}

void V3DLightShader::SetCamera(V3DCamera* camera)
{
	cam = camera;
}

void V3DLightShader::Update()
{
	for (unsigned int i = 0; i < LIGHT_COUNT; i++)
	{
		std::string index = std::to_string(i);
		GLint loc = glGetUniformLocation(GetProgram(), ("lights[" + index + "].enabled").c_str());

		if (loc >= 0)
		{
			if (Lights[i].get() == nullptr)
			{
				glUniform1i(loc, 0);
				continue;
			}

			glUniform1i(loc, Lights[i]->Enabled ? 1 : 0);
			glUniform1i(glGetUniformLocation(GetProgram(), ("lights[" + index + "].type").c_str()),
				static_cast<GLint>(Lights[i]->Type));
			glUniform3f(glGetUniformLocation(GetProgram(), ("lights[" + index + "].pos").c_str()),
				Lights[i]->Position.x, Lights[i]->Position.y, Lights[i]->Position.z);
			glUniform3f(glGetUniformLocation(GetProgram(), ("lights[" + index + "].dir").c_str()),
				Lights[i]->Dir.x, Lights[i]->Dir.y, Lights[i]->Dir.z);
			glUniform3f(glGetUniformLocation(GetProgram(), ("lights[" + index + "].tint").c_str()),
				Lights[i]->Tint.x, Lights[i]->Tint.y, Lights[i]->Tint.z);
			glUniform1f(glGetUniformLocation(GetProgram(), ("lights[" + index + "].att").c_str()),
				Lights[i]->Attenuation);
			glUniform1f(glGetUniformLocation(GetProgram(), ("lights[" + index + "].cutOff").c_str()),
				Lights[i]->CutOff);
			glUniform1f(glGetUniformLocation(GetProgram(), ("lights[" + index + "].ambientCo").c_str()),
				Lights[i]->Coefficient);
		}
	}

	GLint loc = glGetUniformLocation(GetProgram(), "camPos");
	if (loc >= 0)
		glUniform3f(loc, cam->Position.x, cam->Position.y, cam->Position.z);
}
#endif