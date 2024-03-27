#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_direction2;

uniform vec3 light_position2;
uniform vec3 light_position;

uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

// TODO(student): Declare any other uniforms
uniform vec3 object_color;
uniform int isSpotligth;
uniform float cut_off_angle;

//uniform vec3 object_color;

// Output
layout(location = 0) out vec4 out_color;

void main()
{

    vec3 world_position_new = world_position;
    vec3 world_normal_new  = normalize(world_normal);

    // TODO(student): Define ambient, diffuse and specular light components
    float ambient_light = 0.25;
    float diffuse_light = 0;
    float specular_light = 0;

    //2 object

    float ambient_light2 = 0.25;
    float diffuse_light2 = 0;
    float specular_light2 = 0;

    //vec3 P = normalize(light_direction - world_position_new);
    vec3 L = normalize(light_position - world_position_new);
    vec3 V = normalize(eye_position - world_position_new);
    vec3 R = reflect(-L, world_normal_new);
    vec3 H = normalize( L + V );

    // 2 object

    vec3 L2 = normalize(light_position2 - world_position_new);
    vec3 R2 = reflect(-L2, world_normal_new);

    diffuse_light = material_kd * max(dot(world_normal_new, L), 0);
    diffuse_light2 = material_kd * max(dot(world_normal_new, L2), 0);

    //diffuse_light += diffuse_light2;

    // It's important to distinguish between "reflection model" and
    // "shading method". In this shader, we are experimenting with the Phong
    // (1975) and Blinn-Phong (1977) reflection models, and we are using the
    // Phong (1975) shading method. Don't mix them up!

    if (diffuse_light > 0)
    {
        specular_light = material_ks * pow(max(dot(V, R), 0), material_shininess);
    }

     if (diffuse_light2 > 0)
    {
        specular_light2 = material_ks * pow(max(dot(V, R2), 0), material_shininess);
    }

    // TODO(student): If (and only if) the light is a spotlight, we need to do
    // some additional things.

    float attenuation_factor = 0, attenuation_factor2 = 0;
    float ligth = 0, ligth2 = 0;
    //specular_light += specular_light2;



     if (isSpotligth == 1) {
        float spot_light = dot(-L, light_direction);
        float spot_light_limit = cos(radians(cut_off_angle));

        if (spot_light > spot_light_limit) {
            float linear_att = (spot_light - spot_light_limit) / (1.0f - spot_light_limit);
            float light_att_factor = linear_att * linear_att;
            float d = distance(light_position, world_position_new);

            attenuation_factor = light_att_factor * (1.0f / max(d * d, 1.0f));
            ligth = ambient_light + attenuation_factor * (diffuse_light + specular_light);

        } else {
             ligth = ambient_light;
        }

     } else {
         float d = distance(light_position, world_position_new);
         attenuation_factor = 1.0f / max(d * d, 1.0f);
         ligth = ambient_light + attenuation_factor * (diffuse_light + specular_light);
     }


        //2 object

    if (isSpotligth == 1) {
        float spot_light2 = dot(-L2, light_direction);
        float spot_light_limit = cos(radians(cut_off_angle));

        if (spot_light2 > spot_light_limit) {
                float linear_att2 = (spot_light2 - spot_light_limit) / (1.0f - spot_light_limit);
                float light_att_factor2 = linear_att2 * linear_att2;
                 float d2 = distance(light_position2, world_position_new);

                attenuation_factor2 = light_att_factor2 * (1.0f / max(d2 * d2, 1.0f));

                ligth2 = attenuation_factor2 * (diffuse_light2 + specular_light2);

        } else {
          ligth2 = 0;
        }
      } else {
          float d2 = distance(light_position2, world_position_new);
          attenuation_factor2 = 1.0f / max(d2 * d2, 1.0f);
          ligth2 = attenuation_factor2 * (diffuse_light2 + specular_light2);
      }

    // TODO(student): Compute the total light. You can just add the components
    // together, but if you're feeling extra fancy, you can add individual
    // colors to the light components. To do that, pick some vec3 colors that
    // you like, and multiply them with the respective light components.

    // TODO(student): Write pixel out color
    //out_color = vec4(1);
    //out_color2 = vec4(3);

    vec3 color = object_color * (ligth + ligth2);
    out_color = vec4(color, 1);
    

    //vec3 color2 = object_color * ligth2;
    //out_color2 = vec4(color + color2, 1);
    //out_color2 = vec4(color2, 1);

}
