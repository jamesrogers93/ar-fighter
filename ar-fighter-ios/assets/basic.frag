
#version 300 es

// Data structures
struct Light
{
    // Position
    mediump vec3 position;

    // Colours
    mediump vec4 ambient;
    mediump vec4 diffuse;
    mediump vec4 specular;

    // Attenuation
    mediump float constant;
    mediump float linear;
    mediump float quadratic; 
};

struct Material
{
    // colours
    mediump vec4 diffuse;
    mediump vec4 specular;

    mediump float shininess;
};

// In variables
in mediump vec3 Normal;
in mediump vec3 Frag_Position;

// Out variables
out mediump vec4 colour;

// Uniform Variables
uniform mediump vec3 camera_position;
uniform Material material;
uniform Light lights[4];
uniform int num_lights;

void main()
{
    // Initalise colour
    colour = vec4(vec3(0.0), 1.0);

    mediump vec3 viewDir  = normalize(camera_position - Frag_Position);
    
    //mediump vec3 normal = vec3(0.0, 0.0, 1.0);
    mediump vec3 normal = Normal;
    
    for(int i = 0; i < num_lights; i++)
    {
        
        mediump vec3 lightDir = normalize(lights[i].position - Frag_Position);
        
        // Ambient
        mediump float ambientStrength = 1.0;
        
        // Diffuse
        mediump float diffuseStrength = max(dot(Normal, lightDir), 0.0);
        
        // Specular
        mediump vec3 halfwayDir = normalize(lightDir + viewDir);
        mediump float specularStrength = pow(max(dot(Normal, halfwayDir), 0.0), material.shininess);
        
        // Attenuation
        mediump float distance = length(lights[i].position - Frag_Position);
        mediump float attenuation = 1.0 / (lights[i].constant + lights[i].linear * distance + lights[i].quadratic * (distance * distance));
        
        // Calculate colours
        mediump vec4 ambient = lights[i].ambient * material.diffuse * ambientStrength;
        mediump vec4 diffuse = lights[i].diffuse * material.diffuse * diffuseStrength;
        mediump vec4 specular = lights[i].specular * material.specular * specularStrength;
        
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
        
        colour += ambient + diffuse + specular;
    }
}
