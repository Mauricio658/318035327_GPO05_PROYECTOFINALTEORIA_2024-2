//Animación Compleja para el fuego de la estufa
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNormal;
layout (location = 2) in vec2 aTexCoords;

out vec2 TexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform float time;

const float amplitude_y = 0.07; // Amplitud del movimiento en el eje y
const float frequency_y = 10.0; // Frecuencia del movimiento en el eje y
const float amplitude_xz = 0.01; // Amplitud del movimiento en los ejes x y z
const float frequency_xz = 10.0; // Frecuencia del movimiento en los ejes x y z

void main()
{
    // Obtener la distancia del vértice al centro del objeto
    float distance = length(aPos);
    
    // Deformación del vértice en los ejes x y z (simulando una leve oscilación)
    float x_offset = amplitude_xz * sin(time * frequency_xz * distance);
    float z_offset = amplitude_xz * cos(time * frequency_xz * distance);
    
    // Solo aplicar el shader a los vértices de la mitad del objeto hacia arriba
    if (aPos.y > 0.0) {
        // Deformación del vértice en el eje y (simulando el movimiento de una llama)
        float y_offset = amplitude_y * sin(time * frequency_y * distance);
        
        // Aplicar la deformación a la posición del vértice
        vec3 deformed_position = vec3(aPos.x + x_offset, aPos.y + y_offset, aPos.z + z_offset);
        
        // Calcular la posición final del vértice en el espacio de vista y proyección
        gl_Position = projection * view * model * vec4(deformed_position, 1.0);
    } else {
        // Si el vértice está debajo de la mitad del objeto, mantener su posición original
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
    TexCoords = aTexCoords;
}
