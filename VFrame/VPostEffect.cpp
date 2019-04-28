#include "VPostEffect.h"
#include "VGlobal.h"

#include <SFML/Graphics/VertexArray.hpp>

void VPostEffectBase::applyShader(const sf::Shader& shader, sf::RenderTarget& output)
{
	sf::Vector2f outputSize = static_cast<sf::Vector2f>(output.getSize());

	sf::VertexArray vertices(sf::TrianglesStrip, 4);
	vertices[0] = sf::Vertex(sf::Vector2f(0, 0),			sf::Vector2f(0, 1));
	vertices[1] = sf::Vertex(sf::Vector2f(outputSize.x, 0), sf::Vector2f(1, 1));
	vertices[2] = sf::Vertex(sf::Vector2f(0, outputSize.y), sf::Vector2f(0, 0));
	vertices[3] = sf::Vertex(sf::Vector2f(outputSize),		sf::Vector2f(1, 0));

	sf::RenderStates states;
	states.shader = &shader;
	states.blendMode = sf::BlendNone;

	output.draw(vertices, states);
}

void VPostEffectBase::passThrough(const sf::Texture& input, sf::RenderTarget& output)
{
	const std::string fragment = \
		"uniform sampler2D texture;" \
		"void main()" \
		"{" \
		"	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);" \
		"	gl_FragColor = gl_Color * pixel;" \
		"}";

	sf::Shader shader;
	if (shader.loadFromMemory(fragment, sf::Shader::Fragment))
	{
		shader.setUniform("texture", input);

		sf::Vector2f outputSize = static_cast<sf::Vector2f>(output.getSize());

		sf::VertexArray vertices(sf::TrianglesStrip, 4);
		vertices[0] = sf::Vertex(sf::Vector2f(0, 0), sf::Vector2f(0, 1));
		vertices[1] = sf::Vertex(sf::Vector2f(outputSize.x, 0), sf::Vector2f(1, 1));
		vertices[2] = sf::Vertex(sf::Vector2f(0, outputSize.y), sf::Vector2f(0, 0));
		vertices[3] = sf::Vertex(sf::Vector2f(outputSize), sf::Vector2f(1, 0));

		sf::RenderStates states;
		states.shader = &shader;
		states.blendMode = sf::BlendNone;

		output.draw(vertices, states);
	}
}

bool VPostEffectBase::isSupported()
{
	return sf::Shader::isAvailable();
}

VPostEffect::VPostEffect(sf::String filename, sf::Shader::Type type)
{
	shader.loadFromFile(filename, type);
}

VPostEffect::VPostEffect(sf::String vertFile, sf::String fragFile)
{
	shader.loadFromFile(vertFile, fragFile);
}

VPostEffect::VPostEffect(sf::String vertFile, sf::String geomFile, sf::String fragFile)
{
	shader.loadFromFile(vertFile, geomFile, fragFile);
}

void VPostEffect::SetInputTextureName(sf::String name)
{
	sourceTextureName = name;
}

void VPostEffect::SetParameter(sf::String paramName, sf::Shader::CurrentTextureType currentTextureType)
{
	if (paramName != sourceTextureName)
		shader.setUniform(paramName, currentTextureType);
}

void VPostEffect::SetParameter(sf::String paramName, const sf::Texture& texture)
{
	if (paramName != sourceTextureName)
		shader.setUniform(paramName, texture);
}

void VPostEffect::SetParameter(sf::String paramName, const sf::Transform& transform)
{
	if (paramName != sourceTextureName)
		shader.setUniform(paramName, (sf::Glsl::Mat4)transform);
}

void VPostEffect::SetParameter(sf::String paramName, const sf::Color& colour)
{
	if (paramName != sourceTextureName)
		shader.setUniform(paramName, (sf::Glsl::Vec4)colour);
}

void VPostEffect::SetParameter(sf::String paramName, const sf::Vector3f& vec3)
{
	if (paramName != sourceTextureName)
		shader.setUniform(paramName, vec3);
}

void VPostEffect::SetParameter(sf::String paramName, const sf::Vector2f& vec2)
{
	if (paramName != sourceTextureName)
		shader.setUniform(paramName, vec2);
}

void VPostEffect::SetParameter(sf::String paramName, float x, float y, float z, float w)
{
	if (paramName != sourceTextureName)
		shader.setUniform(paramName, sf::Glsl::Vec4(x, y, z, w));
}

void VPostEffect::SetParameter(sf::String paramName, float x, float y, float z)
{
	if (paramName != sourceTextureName)
		shader.setUniform(paramName, sf::Glsl::Vec3(x, y, z));
}

void VPostEffect::SetParameter(sf::String paramName, float x, float y)
{
	if (paramName != sourceTextureName)
		shader.setUniform(paramName, sf::Glsl::Vec2(x, y));
}

void VPostEffect::SetParameter(sf::String paramName, float x)
{
	if (paramName != sourceTextureName)
		shader.setUniform(paramName, x);
}

void VPostEffect::Apply(const sf::Texture& input, sf::RenderTarget& output)
{
	shader.setUniform(sourceTextureName, input);
	applyShader(shader, output);
}

VPostEffectMultipass::VPostEffectMultipass(int MaxSize)
{
	maxSize = MaxSize;
	renderTextures.reserve(MaxSize);
	for (unsigned int i = 0; i < MaxSize; i++)
	{
		renderTextures.push_back(std::make_unique<sf::RenderTexture>());
		renderTextures[i]->create(VGlobal::p()->Width, VGlobal::p()->Height);
	}
}

VPostEffectMultipass::~VPostEffectMultipass()
{
	for (unsigned int i = 0; i < postEffects.size(); i++)
	{
		delete postEffects[i];
		postEffects[i] = nullptr;
	}

	postEffects.clear();
	postEffects.shrink_to_fit();

	enabled.clear();
	enabled.shrink_to_fit();

	for (unsigned int i = 0; i < renderTextures.size(); i++)
	{
		renderTextures[i].reset();
	}

	renderTextures.clear();
	renderTextures.shrink_to_fit();
}

void VPostEffectMultipass::AddPostEffect(VPostEffectBase* post)
{
	if (postEffects.size() >= maxSize)
		return;

	postEffects.push_back(post);
	enabled.push_back(true);
}

void VPostEffectMultipass::RemovePostEffect(unsigned int index)
{
	if (index >= maxSize)
		return;

	delete postEffects[index];
	postEffects.erase(postEffects.begin() + index);
	enabled.erase(enabled.begin() + index);
}

VPostEffectBase* VPostEffectMultipass::GetPostEffect(unsigned int index)
{
	if (index >= postEffects.size())
		return nullptr;

	return postEffects[index];
}

void VPostEffectMultipass::EnableEffect(unsigned int index, bool enable)
{
	VPostEffectBase* post = GetPostEffect(index);

	if (post)
	{
		enabled[index] = enable;
	}
}

void VPostEffectMultipass::Apply(const sf::Texture& input, sf::RenderTarget& output)
{
	int inputRenderId = -1;
	int outputRenderId = 0;

	for (unsigned int i = 0; i < postEffects.size(); i++)
	{
		sf::RenderTarget* renderOutput = renderTextures[outputRenderId].get();

		if (enabled[i])
		{
			if (inputRenderId < 0)
				postEffects[i]->Apply(input, *renderOutput);
			else
				postEffects[i]->Apply(renderTextures[inputRenderId]->getTexture(), *renderOutput);
		}
		else
		{
			if (inputRenderId < 0)
				passThrough(input, *renderOutput);
			else
				passThrough(renderTextures[inputRenderId]->getTexture(), *renderOutput);
		}

		inputRenderId++;
		outputRenderId++;
	}

	passThrough(renderTextures[outputRenderId - 1]->getTexture(), output);
}

void VPostEffectMultipass::Update(float dt)
{
	for (unsigned int i = 0; i < postEffects.size(); i++)
	{
		if (enabled[i])
		{
			postEffects[i]->Update(dt);
		}
	}
}
