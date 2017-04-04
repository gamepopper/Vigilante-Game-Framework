#version 130

uniform sampler2D texture;
uniform vec2 textureSize;

uniform float sectionWidth;

uniform float time;
uniform float seed;

uniform float minAcc;
uniform float maxAcc;

uniform vec4 bgColor;

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
	vec4 pixel = texture2D(texture, gl_TexCoord[0].xy);

	if (pixel.w > 0)
	{
		//convert section width to UV
		float sec = (sectionWidth / textureSize.x);
		float x = gl_TexCoord[0].x;

		float accRange = maxAcc - minAcc;
		float acceleration = abs(rand(vec2(floor(x/sec), seed))) * accRange;
		acceleration += minAcc;

		float u = 0;
		float a = acceleration / textureSize.y;
		float s = (u * time) + 0.5f * (a * time * time);

		vec2 mod = gl_TexCoord[0].xy + vec2(0, s);
		if (mod.y >= 0 && mod.y <= 1)
		{
			// lookup the pixel in the texture
			vec4 pixel = texture2D(texture, mod);
		
			// multiply it by the color
			gl_FragColor = gl_Color * pixel;
		}
		else
		{
			float alpha = 1 - clamp(mod.y - 1, 0, 1);
			gl_FragColor = bgColor * alpha;
		}
	}
	else
	{
		gl_FragColor = vec4(0,0,0,0);
	}
}