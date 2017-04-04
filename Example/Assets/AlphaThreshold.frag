uniform sampler2D texture;

uniform float threshold;
uniform float smoothness;
uniform float unpremultiply;

void main(void)
{
	vec4 color = texture2D( texture, gl_TexCoord[0].xy ) ;
	if( unpremultiply > 0.0 ){ color.rgb /= color.a ; }
	
	float range = ( color.a - (1.0 - threshold) - (smoothness * 0.05) ) / (0.0001 + smoothness * 0.1) ;
	color.a = smoothstep( 0.0, 1.0, range ) ;
	color.rgb *= color.a ;
	
	gl_FragColor = gl_Color * color ;
}