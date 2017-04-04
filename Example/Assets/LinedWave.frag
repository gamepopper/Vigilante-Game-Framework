#version 130
uniform sampler2D texture;
uniform vec2 textureSize;
uniform float lineHeight = 1.0;
uniform float amplitude = 0.0;
uniform float time = 0.0;
uniform float waveMultiply = 1.0;

const float pi = 3.14159265;

void main()
{
	//Convert amplitude from pixel to UV
	float width = (amplitude / textureSize.x) * 0.5;

	float ts = gl_TexCoord[0].y;

	//Get exact pixel Y position
	float pixelHeight = ts * textureSize.y;

	//Alternate between positive and negative sine
	vec2 mod;
	if (int(pixelHeight / lineHeight) % 2 == 0)
	{
		mod = gl_TexCoord[0].xy + vec2(width * sin((time + 1.5*ts*pi) * waveMultiply), 0);
	}
	else
	{
		mod = gl_TexCoord[0].xy - vec2(width * sin((time + 1.5*ts*pi) * waveMultiply), 0);
	}

	if (mod.x >= 0 && mod.x <= 1)
	{
		// lookup the pixel in the texture
		vec4 pixel = texture2D(texture, mod);
		
		// multiply it by the color
		gl_FragColor = gl_Color * pixel;
	}
	else
	{
		gl_FragColor = vec4(0,0,0,0);
	}
}