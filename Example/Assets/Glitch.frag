#version 130
uniform sampler2D texture;
uniform vec2 textureSize;
uniform float amplitude;
uniform float lineHeight;
uniform float time;

highp float rand(vec2 co){
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}

void main()
{
	//Convert amplitude from pixel to UV
	float width = (amplitude / textureSize.x) * 0.5;

	//Get exact pixel Y position
	float ts = gl_TexCoord[0].y;
	float pixelHeight = ts * textureSize.y;

	//Random multiplier between -0.5 and 0.5
	highp float mult = rand(vec2(time, int(pixelHeight / lineHeight)));
	if (mult < 0.001)
		mult = 0;

	vec2 mod = gl_TexCoord[0].xy + vec2(width * mult, 0);
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