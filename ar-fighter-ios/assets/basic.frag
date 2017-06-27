
#version 300 es

// Data structures
struct DirectionalLight
{
    // Direction
    mediump vec3 direction;
    
    // Colours
    mediump vec4 ambient;
    mediump vec4 diffuse;
    mediump vec4 specular;
};

struct PointLight
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

// Uniform variables
uniform mediump vec3 camera_position;
uniform Material material;
uniform DirectionalLight directional_lights[2];
uniform int num_directional_lights;
uniform PointLight point_lights[4];
uniform int num_point_lights;

// Prototype functions
void directionalLightColour(const mediump vec3 viewDir);
void pointLightColour(const mediump vec3 viewDir);

void main()
{
    // Initalise colour
    colour = vec4(vec3(0.0), 1.0);

    mediump vec3 viewDir  = normalize(camera_position - Frag_Position);
    
    directionalLightColour(viewDir);
    pointLightColour(viewDir);
}

void directionalLightColour(const mediump vec3 viewDir)
{
    for(int i = 0; i < num_directional_lights; i++)
    {
        // Ambient
        mediump float ambientStrength = 1.0;
        
        // Diffuse
        mediump float diffuseStrength = max(dot(Normal, directional_lights[i].direction), 0.0);
        
        // Specular
        mediump vec3 halfwayDir = normalize(directional_lights[i].direction + viewDir);
        mediump float specularStrength = pow(max(dot(Normal, halfwayDir), 0.0), material.shininess);
        
        // Calculate colours
        mediump vec4 ambient = directional_lights[i].ambient * material.diffuse * ambientStrength;
        mediump vec4 diffuse = directional_lights[i].diffuse * material.diffuse * diffuseStrength;
        mediump vec4 specular = directional_lights[i].specular * material.specular * specularStrength;
        
        colour += ambient + diffuse + specular;
    }
}

void pointLightColour(const mediump vec3 viewDir)
{
    for(int i = 0; i < num_point_lights; i++)
    {
        
        mediump vec3 lightDir = normalize(point_lights[i].position - Frag_Position);
        
        // Ambient
        mediump float ambientStrength = 1.0;
        
        // Diffuse
        mediump float diffuseStrength = max(dot(Normal, lightDir), 0.0);
        
        // Specular
        mediump vec3 halfwayDir = normalize(lightDir + viewDir);
        mediump float specularStrength = pow(max(dot(Normal, halfwayDir), 0.0), material.shininess);
        
        // Attenuation
        mediump float distance = length(point_lights[i].position - Frag_Position);
        mediump float attenuation = 1.0 / (point_lights[i].constant + point_lights[i].linear * distance + point_lights[i].quadratic * (distance * distance));
        
        // Calculate colours
        mediump vec4 ambient = point_lights[i].ambient * material.diffuse * ambientStrength;
        mediump vec4 diffuse = point_lights[i].diffuse * material.diffuse * diffuseStrength;
        mediump vec4 specular = point_lights[i].specular * material.specular * specularStrength;
        
        // Apply attenuation
        ambient *= attenuation;
        diffuse *= attenuation;
        specular *= attenuation;
        
        colour += ambient + diffuse + specular;
    }
}
