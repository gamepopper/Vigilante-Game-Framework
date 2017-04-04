#version 130
uniform sampler2D texture;
uniform float Factor = 4.0;

void main()
{
	vec4 textureFragment = texture2D(texture, gl_TexCoord[0].xy);
	//float luminance = textureFragment.r * 0.2126 + textureFragment.g * 0.7152 + textureFragment.b * 0.0722;
	textureFragment *= Factor;
	gl_FragColor = textureFragment;
}