#version 130
uniform sampler2D texture;
uniform float circleRadius;
uniform vec2 screenDim;
uniform float multiplier;

void main()
{
	float minDim = min(screenDim.x, screenDim.y);
	vec2 factor = vec2(screenDim.x / minDim, screenDim.y / minDim);
	vec2 texCoord = mod(gl_TexCoord[0].xy * factor * multiplier, 1);

	float l = length(texCoord - vec2(0.5, 0.5));
 
	if (l > circleRadius)
		discard;
 
	gl_FragColor = texture2D(texture, gl_TexCoord[0].xy);
}