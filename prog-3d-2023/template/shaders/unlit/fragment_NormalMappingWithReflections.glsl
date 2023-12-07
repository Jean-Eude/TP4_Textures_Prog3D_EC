#version 330 core
#define GLSLIFY 1

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec2 o_uv0;
out vec4 FragColor;

uniform sampler2D colorTexture1;
uniform sampler2D colorTexture2; 
uniform samplerCube skybox; 

in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

void main() {
    vec3 lightColor = vec3(1.);

    float NormalStrength = 1.0f;
    
    vec3 normal = texture(colorTexture2, o_uv0).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal.x *= NormalStrength;
    normal.y *= NormalStrength;

    float ambientStrength = 0.2f; 
    vec3 ambient = ambientStrength * lightColor;

    float diffuseStrength = 1.0f;
    vec3 lightDir = normalize(TangentLightPos - TangentFragPos);
    float diff = max(dot(normal, lightDir), 0.0);
    vec3 diffuse = diffuseStrength * diff * lightColor;
    
    float specularStrength = 0.5f; 
    vec3 viewDir = normalize(TangentViewPos - TangentFragPos);
    vec3 reflectDir = reflect(-lightDir, normal);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor; 

    //vec3 res = (ambient + diffuse + specular) * texture(colorTexture1, vec2(o_uv0.x, 1 - o_uv0.y)).rgb;

    vec3 CouleurR = texture(skybox, reflect(-viewDir, normal)).rgb;
    //result = mix(result, CouleurR, 1.0); 

    FragColor = vec4(CouleurR, 1.0f);
}
