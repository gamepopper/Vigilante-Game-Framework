#version 130
uniform sampler2D texture;
uniform vec2 textureSize;
uniform float amplitude;
uniform float time;
uniform float waveMultiply = 1.0;

const float pi = 3.14159265;

void main()
{
	//Convert amplitude from pixel to UV
	float height = (amplitude / textureSize.y) * 0.5;

	float ts = gl_TexCoord[0].x;

	//Get exact pixel Y position
	float pixelWidth = ts * textureSize.x;

	//Alternate between positive and negative sine
	vec2 mod = gl_TexCoord[0].xy - vec2(0, height * sin((time + 1.5*ts*pi) * waveMultiply));

	if (mod.y >= 0 && mod.y <= 1)
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