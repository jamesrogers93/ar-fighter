
#version 300 es

// In variables
in vec3 position;
in vec3 normal;

// Out variables
out mediump vec3 Normal;
out mediump vec3 Frag_Position;

// Uniform variables
uniform mat4 camera_projection;
uniform mat4 camera_view;
uniform mat4 model;
uniform mat3 normal_matrix;

void main()
{
    Normal = normalize(normal_matrix * normal);
    //Normal = normal;
    
    Frag_Position = vec3(model * vec4(position, 1.0));

    gl_Position = camera_projection * camera_view * model * vec4(position, 1.0);
}
