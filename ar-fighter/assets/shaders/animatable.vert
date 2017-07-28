
#version 300 es

const int MAX_JOINTS = 60;

// In variables
in vec3 position;
in vec3 normal;
in vec2 uv0;
in ivec4 joint_id;
in vec4 joint_weight;

// Out variables
out mediump vec3 Normal;
out mediump vec3 Frag_Position;

// Uniform variables
uniform mat4 camera_projection;
uniform mat4 camera_view;
uniform mat4 model;
uniform mat3 normal_matrix;
uniform mat4 joints[MAX_JOINTS];

void main()
{
    Normal = normalize(normal_matrix * normal);

    Frag_Position = vec3(model * vec4(position, 1.0));

    mat4 joint_transform = joints[joint_id[0]] * joint_weight[0];
    joint_transform += joints[joint_id[1]] * joint_weight[1];
    joint_transform += joints[joint_id[2]] * joint_weight[2];
    joint_transform += joints[joint_id[3]] * joint_weight[3];
    
    gl_Position = camera_projection * camera_view * model * joint_transform * vec4(position, 1.0);
}
