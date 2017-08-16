#version 300 es

// Data Structures
struct Material
{
    // colours
    mediump vec4 diffuse;
};

// Out varaibles
out mediump vec4 colour;

// Uniform variables
uniform Material material;

void main()
{
    colour = material.diffuse;
}