uniform sampler2D 	texture;
uniform vec2 		textureSize;

void main()
{
	vec2 pixelSize = vec2(1.0 / textureSize.x, 1.0 / textureSize.y);
	vec2 textureCoordinates = gl_TexCoord[0].xy;
	vec4 color = texture2D(texture, textureCoordinates);
	color     += texture2D(texture, textureCoordinates + vec2( 1.0,  0.0) * pixelSize);
	color     += texture2D(texture, textureCoordinates + vec2(-1.0,  0.0) * pixelSize);
	color     += texture2D(texture, textureCoordinates + vec2( 0.0,  1.0) * pixelSize);
	color     += texture2D(texture, textureCoordinates + vec2( 0.0, -1.0) * pixelSize);
	color     += texture2D(texture, textureCoordinates + vec2( 1.0,  1.0) * pixelSize);
	color     += texture2D(texture, textureCoordinates + vec2(-1.0, -1.0) * pixelSize);
	color     += texture2D(texture, textureCoordinates + vec2( 1.0, -1.0) * pixelSize);
	color     += texture2D(texture, textureCoordinates + vec2(-1.0,  1.0) * pixelSize);
	gl_FragColor = color / 9.0;
}