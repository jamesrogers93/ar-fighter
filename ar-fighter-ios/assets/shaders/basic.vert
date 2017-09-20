#version 300 es

// In variables
in vec3 position;

// Uniform varaibles
uniform mat4 camera_projection;
uniform mat4 camera_view;
uniform mat4 model;

void main()
{
    gl_Position = camera_projection * camera_view * model * vec4(position, 1.0);
}


