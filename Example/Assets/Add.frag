uniform sampler2D texture;
uniform sampler2D other;

void main()
{
    vec4 textureFragment = texture2D(texture, gl_TexCoord[0].xy);
    vec4 otherFragment = texture2D(other, gl_TexCoord[0].xy);
    gl_FragColor = textureFragment + otherFragment;
}