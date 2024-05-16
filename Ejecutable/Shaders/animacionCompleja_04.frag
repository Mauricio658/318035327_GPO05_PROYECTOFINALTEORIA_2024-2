#version 330 core
out vec4 FragColor;
in vec2 TexCoords;
uniform float activaTransparencia;
uniform vec4 colorAlpha;
uniform sampler2D texture1;

void main()
{
 
  vec4 texColor= colorAlpha * texture(texture1,TexCoords);
    if(texColor.a < 0.1 && activaTransparencia > 0.0)
        discard;
    FragColor = texColor;
}