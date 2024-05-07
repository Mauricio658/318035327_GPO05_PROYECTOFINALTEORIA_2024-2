//Animaci�n Compleja para el fuego de la estufa
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
    // Obtener la distancia del v�rtice al centro del objeto
    float distance = length(aPos);
    
    // Deformaci�n del v�rtice en los ejes x y z (simulando una leve oscilaci�n)
    float x_offset = amplitude_xz * sin(time * frequency_xz * distance);
    float z_offset = amplitude_xz * cos(time * frequency_xz * distance);
    
    // Solo aplicar el shader a los v�rtices de la mitad del objeto hacia arriba
    if (aPos.y > 0.0) {
        // Deformaci�n del v�rtice en el eje y (simulando el movimiento de una llama)
        float y_offset = amplitude_y * sin(time * frequency_y * distance);
        
        // Aplicar la deformaci�n a la posici�n del v�rtice
        vec3 deformed_position = vec3(aPos.x + x_offset, aPos.y + y_offset, aPos.z + z_offset);
        
        // Calcular la posici�n final del v�rtice en el espacio de vista y proyecci�n
        gl_Position = projection * view * model * vec4(deformed_position, 1.0);
    } else {
        // Si el v�rtice est� debajo de la mitad del objeto, mantener su posici�n original
        gl_Position = projection * view * model * vec4(aPos, 1.0);
    }
    TexCoords = aTexCoords;
}
