#include "V3DModel.h"

void V3DModel::updateTransform()
{
	//S * R * T
	glTranslatef(Position.x, Position.y, Position.z);
	glRotatef(Rotation.z, 0.0f, 0.0f, 1.0f);
	glRotatef(Rotation.x, 1.0f, 0.0f, 0.0f);
	glRotatef(Rotation.y, 0.0f, 1.0f, 0.0f);
	glScalef(Scale.x, Scale.y, Scale.z);
}

bool V3DModel::LoadModelData(const std::vector<GLfloat>& data, int vertexPos, int normalPos, int texturePos, int colourPos)
{
	modelData = data;
	vertexArrayOffset = vertexPos;
	normalArrayOffset = normalPos;
	textureArrayOffset = texturePos;
	colourArrayOffset = colourPos;

	dataLineLength = 0;
	if (vertexArrayOffset >= 0)
		dataLineLength += 3;

	if (textureArrayOffset >= 0)
		dataLineLength += 2;

	if (normalArrayOffset >= 0)
		dataLineLength += 3;

	if (colourArrayOffset >= 0)
		dataLineLength += 4;

	return true;
}

bool V3DModel::LoadTexture(const sf::String& filename, bool mipmap)
{
	if (mipmap)
	{
		if (texture.loadFromFile(filename))
		{
			texture.generateMipmap();
			return true;
		}

		return false;
	}
	else
	{
		return texture.loadFromFile(filename);
	}
}

bool V3DModel::LoadTexture(const sf::Texture& texture, bool mipmap)
{
	this->texture = texture;

	if (mipmap)
		this->texture.generateMipmap();

	return true;
}

void V3DModel::SetMaterial(sf::Color Colour, sf::Color Specular, float Shininess)
{
	material->Colour[0] = Colour.r / 255.0f;	material->Colour[1] = Colour.g / 255.0f;	material->Colour[2] = Colour.b / 255.0f;	material->Colour[3] = Colour.a / 255.0f;
	material->Specular[0] = Specular.r/255.0f;	material->Specular[1] = Specular.g/255.0f;	material->Specular[2] = Specular.b/255.0f;	material->Specular[3] = Specular.a/255.0f;
	material->Shininess = Shininess;
}

void V3DModel::Destroy()
{
	VSUPERCLASS::Destroy();
	modelData.clear();
	modelData.shrink_to_fit();
}

void V3DModel::Draw(sf::RenderTarget& RenderTarget)
{
	// Enable position and texture coordinates vertex components
	if (vertexArrayOffset >= 0)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glVertexPointer(3, GL_FLOAT, dataLineLength * sizeof(GLfloat), modelData.data() + vertexArrayOffset);
	}
	else
	{
		glDisableClientState(GL_VERTEX_ARRAY);
	}

	if (textureArrayOffset >= 0)
	{
		glEnableClientState(GL_TEXTURE_COORD_ARRAY);
		glTexCoordPointer(2, GL_FLOAT, dataLineLength * sizeof(GLfloat), modelData.data() + textureArrayOffset);
	}
	else
	{
		glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	}

	if (normalArrayOffset >= 0)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glNormalPointer(GL_FLOAT, dataLineLength * sizeof(GLfloat), modelData.data() + normalArrayOffset);
	}
	else
	{
		glDisableClientState(GL_NORMAL_ARRAY);
	}

	if (colourArrayOffset >= 0)
	{
		glEnableClientState(GL_NORMAL_ARRAY);
		glColorPointer(4, GL_FLOAT, dataLineLength * sizeof(GLfloat), modelData.data() + colourArrayOffset);
	}
	else
	{
		glDisableClientState(GL_COLOR_ARRAY);
	}

	glEnable(GL_TEXTURE_2D);
	sf::Texture::bind(&texture);

	if (material)
	{
		glEnable(GL_COLOR_MATERIAL);
		glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);

		glColor4f(material->Colour[0], material->Colour[1], material->Colour[2], material->Colour[3]);
		glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material->Specular);
		glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->Shininess);

		if (material->Colour[3] < 1.0f)
		{
			glEnable(GL_BLEND);
			glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		}
		else
		{
			glDisable(GL_BLEND);
		}
	}
	else
	{
		glDisable(GL_COLOR_MATERIAL);
	}

	glPushMatrix();
	VSUPERCLASS::Draw(RenderTarget);

	// Draw the model
	glDrawArrays(GL_TRIANGLES, 0, modelData.size() / dataLineLength);

	glPopMatrix();

	sf::Texture::bind(nullptr);
}
