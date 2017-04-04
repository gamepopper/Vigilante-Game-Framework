#version 130

uniform sampler2D texture;
uniform sampler2D height;

uniform vec3 light;
uniform float intensity;

void main()
{
	vec2 ts = gl_TexCoord[0].xy;
	vec4 p = texture2D(texture, ts);
	vec4 h = texture2D(height, ts);

	vec3 normal = 2.0 * h.rgb - 1.0;
	vec3 light = normalize(vec3(light.x - ts.x, light.y - ts.y, light.z));
	float diffuse = clamp(dot(normal, light), 0.0, 1.0); 

	gl_FragColor = (intensity * p * diffuse);
}