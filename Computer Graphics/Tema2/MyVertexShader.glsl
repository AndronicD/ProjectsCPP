#version 330

// Input
// TODO(student): Get vertex attributes from each location
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_normal;
layout(location = 2) in vec2 v_coordinate;
layout(location = 3) in vec3 v_color;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform vec3 v_car;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_position;
out vec3 frag_normal;
out vec2 frag_coordinate;
out vec3 frag_color;

void main()
{
    float scaleFactor = 0.015;
    // TODO(student): Send output to fragment shader
    frag_position = v_position;
    frag_normal = v_normal;
    frag_coordinate = v_coordinate;
    vec4 ve = Model * vec4(v_position, 1);
    frag_color = v_color;
    vec4 ve2 = vec4(v_car, 1);

    ve.y = ve.y - length(ve2 - ve) * length(ve2 - ve) * scaleFactor;
    gl_Position = Projection * View * ve; 
    //gl_Position = Projection * View * Model * vec4(frag_position, 1);
}