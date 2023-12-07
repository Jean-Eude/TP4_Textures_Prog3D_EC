#version 330 core
#define GLSLIFY 1
#define GLSLIFY 1

in vec3 o_positionWorld;
in vec3 o_normalWorld;
in vec2 o_uv0;
out vec4 FragColor;

uniform sampler2D colorTexture1;
uniform sampler2D colorTexture2;
uniform sampler2D roughness;
uniform sampler2D ao;
uniform samplerCube skybox;

in vec3 TangentLightPos;
in vec3 TangentViewPos;
in vec3 TangentFragPos;

uniform vec3 lightPos;
uniform vec3 viewPos; 

const float PI = 3.14159265359;

// Functions
// ----------------------------------------------------------------------------
float DistributionGGX(vec3 N, vec3 H, float roughness)
{
    float a = roughness*roughness;
    float a2 = a*a;
    float NdotH = max(dot(N, H), 0.0);
    float NdotH2 = NdotH*NdotH;

    float nom   = a2;
    float denom = (NdotH2 * (a2 - 1.0) + 1.0);
    denom = PI * denom * denom;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySchlickGGX(float NdotV, float roughness)
{
    float r = (roughness + 1.0);
    float k = (r*r) / 8.0;

    float nom   = NdotV;
    float denom = NdotV * (1.0 - k) + k;

    return nom / denom;
}
// ----------------------------------------------------------------------------
float GeometrySmith(vec3 N, vec3 V, vec3 L, float roughness)
{
    float NdotV = max(dot(N, V), 0.0);
    float NdotL = max(dot(N, L), 0.0);
    float ggx2 = GeometrySchlickGGX(NdotV, roughness);
    float ggx1 = GeometrySchlickGGX(NdotL, roughness);

    return ggx1 * ggx2;
}
// ----------------------------------------------------------------------------
vec3 fresnelSchlick(float cosTheta, vec3 F0)
{
    return F0 + (1.0 - F0) * pow(clamp(1.0 - cosTheta, 0.0, 1.0), 5.0);
}
// ----------------------------------------------------------------------------

void main()
{
    float NormalStrength = 1.0f;
    vec3 normal = texture(colorTexture2, o_uv0).rgb;
    normal = normalize(normal * 2.0 - 1.0);
    normal.x *= NormalStrength;
    normal.y *= NormalStrength;

    vec3 albedo     = pow(texture(colorTexture1, o_uv0).rgb, vec3(2.2));
    float roughness = texture(roughness, o_uv0).r;  
    float ao        = texture(ao, o_uv0).r;
    float metallic  = 1.0f;

    vec3 N = normalize(normal);
    vec3 V = normalize(TangentViewPos - TangentFragPos);
    vec3 F0 = vec3(0.04); 
    F0 = mix(F0, albedo, metallic);

    vec3 Lo = vec3(0.0);

    vec3 L = normalize(TangentViewPos - TangentFragPos);
    vec3 H = normalize(V + L);

    vec3 envColor = texture(skybox, reflect(-V, N)).rgb;

    float NDF = DistributionGGX(N, H, roughness);
    float G = GeometrySmith(N, V, L, roughness);
    vec3 F = fresnelSchlick(clamp(dot(H, V), 0.0, 1.0), F0);

    vec3 numerator = NDF * G * F;
    float denominator = 4.0 * max(dot(N, V), 0.0) * max(dot(N, L), 0.0) + 0.0001;
    vec3 specular = numerator / denominator;

    vec3 kS = F;
    vec3 kD = (vec3(1.0) - kS);
    kD *= 1.0 - metallic;

    float NdotL = max(dot(N, L), 0.0);

    Lo += (kD * albedo / PI + specular) * envColor * NdotL;

    vec3 ambient = vec3(0.03) * albedo * ao;
    vec3 color = ambient + Lo;

    color = pow(color, vec3(1.0/2.2)); 

    FragColor = vec4(color, 1.0);
}