#version 410

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position[2];
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms

uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

float point_light_contribution(vec3 light_position, vec3 light_direction);

void main()
{
    float ambient_light = material_kd;
    // TODO(student): Write pixel out color
     float light = ambient_light + point_light_contribution(light_position[0], light_direction) + point_light_contribution(light_position[1], light_direction);
     out_color = vec4(object_color * light, 1);

}

float point_light_contribution(vec3 light_position, vec3 light_direction)
{
	// TODO(student): Define ambient, diffuse and specular light components
    vec3 L = normalize( light_position - world_position );
    vec3 V = normalize( eye_position - world_position );
    vec3 H = normalize( L + V );

    float ambient_light = material_kd;
    float diffuse_light = material_kd * max (dot(world_normal, L), 0);
    float specular_light = 0.0;

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!
    
    vec3 R = reflect (-L, world_normal);
    float d = length(light_position - world_position);
    float atenuare = 1/(d*d);

    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.

    float cut_off = radians(30);
    float spot_light = dot(-L, light_direction);
    float spot_light_limit = cos(cut_off);
    float linear_att;
    float light_att_factor;
    if (spot_light > cos(cut_off)){
        linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
        light_att_factor = pow(linear_att, 2);
    }

    if(dot(light_direction, light_direction) == 0){
        light_att_factor = atenuare;
    }
    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.
    float light = light_att_factor * (diffuse_light + specular_light);
	
    return light;
}