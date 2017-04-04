uniform sampler2D texture;

void main()
{
    vec4 sum = vec4(0);
	vec2 texcoord = vec2(gl_TexCoord[0]);
    vec4 pixel = texture2D(texture, texcoord);
	int j;
	int i;

	for( i= -4; i < 4; i++)
	{
        for (j = -3; j < 3; j++)
        {
			sum += texture2D(texture, texcoord + vec2(j, i)*0.004) * 0.25;
		}
	}

	if (pixel.r < 0.3)
	{
		gl_FragColor = gl_Color * (sum * sum * 0.012 + pixel);
	}
	else
	{
		if (pixel.r < 0.5)
		{
			gl_FragColor = gl_Color * (sum * sum * 0.009 + pixel);
		}
		else
		{
			gl_FragColor = gl_Color * (sum * sum * 0.0075 + pixel);
		}
	}
}