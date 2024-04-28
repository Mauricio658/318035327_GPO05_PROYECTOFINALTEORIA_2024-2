#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

const float amplitude_x = 0.05; // Amplitud del movimiento en el eje x
const float frequency_x = 7.0; // Frecuencia del movimiento en el eje x
const float amplitude_z = 0.09; // Amplitud del movimiento en el eje z
const float frequency_z = 1.5; // Frecuencia del movimiento en el eje z
const float amplitude_y = 0.09; // Amplitud del movimiento en el eje y (levantar)
const float frequency_y = 1.0; // Frecuencia del movimiento en el eje y (levantar)
const float suavisado = 110.0f;// Suavisar el movimiento del eje x
const float PI = 3.14159;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

void main()
{
    float x_offset = amplitude_x * cos(PI * frequency_x * time/suavisado);
    float z_offset = amplitude_z * sin(PI * frequency_z * time);
    float y_offset = amplitude_y * sin(PI * frequency_y * time);
    
    vec3 offset = vec3(x_offset, y_offset, z_offset);
    
    gl_Position = projection * view * model * vec4(aPos + offset, 1.0);
    TexCoords = vec2(aTexCoords.x,aTexCoords.y);
}