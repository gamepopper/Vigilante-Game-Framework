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

void VPostEffectBase::passThrough(const sf::RenderTexture& input, sf::RenderTarget& output)
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
		shader.setUniform("texture", input.getTexture());

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

void VPostEffect::Apply(const sf::RenderTexture& input, sf::RenderTarget& output)
{
	shader.setUniform(sourceTextureName, input.getTexture());
	applyShader(shader, output);
}

VPostEffectMultipass::VPostEffectMultipass(int MaxSize)
{
	maxSize = MaxSize;

	unsigned int amount = maxSize - 1 > 0 ? maxSize - 1 : 0;
	renderTextures.reserve(amount);
	for (unsigned int i = 0; i < amount; i++)
	{
		sf::RenderTexture* renderTex = new sf::RenderTexture();
		renderTex->create(VGlobal::p()->Width, VGlobal::p()->Height);
		renderTextures.push_back(renderTex);
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
		delete renderTextures[i];
		renderTextures[i] = nullptr;
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

void VPostEffectMultipass::Apply(const sf::RenderTexture& input, sf::RenderTarget& output)
{
	int inputRenderId = -1;
	int outputRenderId = 0;

	for (unsigned int i = 0; i < postEffects.size(); i++)
	{
		sf::RenderTarget& renderOutput = outputRenderId < (int)postEffects.size() - 1 ? *renderTextures[outputRenderId] : output;

		if (enabled[i])
		{
			if (inputRenderId < 0)
				postEffects[i]->Apply(input, renderOutput);
			else
				postEffects[i]->Apply(*renderTextures[inputRenderId], renderOutput);
		}
		else
		{
			if (inputRenderId < 0)
				passThrough(input, renderOutput);
			else
				passThrough(*renderTextures[inputRenderId], renderOutput);
		}

		inputRenderId++;
		outputRenderId++;
	}
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
