#define POINT_LIGHT_NUM 64

struct Material
{
    float ambient;
    float diffuse;
    float specular;
    float shiness;
};
struct DirectionalLight
{
    float4 direction;//xyz - direction, w - ambient
    float4 color;//rgb - color, a - specular
};
struct PointLight
{
    float4 position;//w - specular strength
    float4 color;
    float4 params; //r - const, g - linear, b - quadratic, a - m_ambientStrength ( intensity * 1 / (c + b * distance + a * distance^2))
};
struct SpotLight
{
    float4 position;//xyz - position, w - cutStart
    float4 color;//rgb - color, a - cutEnd
    float4 direction;
};
    
cbuffer ConstBuf0 : register(b0)
{
    float4 BUF0_view_position;//xyz - view position, w - pow factor 
    float4 BUF0_view_direction;//xyz - view_direction, w - cutof cosine
    PointLight BUF0_pointLight[POINT_LIGHT_NUM];//PointLight BUF0_pointLight[POINT_LIGHT_NUM];
    unsigned int BUF0_pointLigthNum;
    float tmp1;
    float tmp2;
    float tmp3;
};


Texture2D tex : register(t0);
Texture2D tex2 : register(t1);
Texture2D flashlightTexture : register(t2);

SamplerState splr;

float3 CalculateDirectionalLight(const DirectionalLight directLight, const float3 normNormalWS, const float3 pixelPosWS);
float3 CalculatePointLight(const PointLight pointLight, const float3 normNormalWS, const float3 pixelPosWS);
float3 CalculateSpotLight(const SpotLight spotLight, const float3 normNormalWS, const float3 pixelPosWS);

float4 main(float3 pin_normal : Normal, float2 pin_UV : UV, float4 pin_color : Colory, float3 pin_pixel_pos_worldspace : PixelPosWS, float3 pin_pixel_pos_viewspace : PixelPosWS, float3 pin_pixel_pos_screenspace : PixelPosSS) : SV_Target //uint FaceID : SV_PrimitiveID
{
    //return float4(1.0f, 0.0f, 0.0f, 1.0f);
    //return float4(BUF0_PointLights[0].position.x, 0.0f, 0.0f, 1.0f);
    //return facesColors[FaceID / 2];
    //return p_color;
    /*
    float3 normal_normalized = normalize(pin_normal);
    float3 to_light_dir = normalize(BUF0_pointLight.position.rgb - pin_pixel_pos_worldspace);
    float distance = length(BUF0_pointLight.position.rgb - pin_pixel_pos_worldspace);
    float attenuation = 1.0f / (BUF0_pointLight.attenuation.r +
    (BUF0_pointLight.attenuation.g * distance) +
    (BUF0_pointLight.attenuation.b * distance * distance));
    
    float3 to_view_dir = normalize(BUF0_view_position.rgb - pin_pixel_pos_worldspace);
    float3 reflect_from_view_dir = reflect(-to_light_dir, normal_normalized);
    
    float diffuse = max(dot(normal_normalized, to_light_dir), 0.0f);
    float specular = pow(max(dot(to_view_dir, reflect_from_view_dir), 0.0f), BUF0_view_position.w) * BUF0_pointLight.position.w;
    
    float3 light = (attenuation * (BUF0_pointLight.lightColor.a + diffuse + specular)) * BUF0_pointLight.lightColor.rgb;
    
    float theta = max(dot(normalize(BUF0_view_direction.xyz), normalize(-to_view_dir)), 0.0f);
    float flashlight = clamp((theta - 0.939f) / 
    (0.965f - 0.939f),
    0.0f, 1.0f);
    float flashlight2 = flashlightTexture.Sample(splr, float2((pin_pixel_pos_screenspace.x + 2.0f) / 4.0f, (pin_pixel_pos_screenspace.y + 2.0f) / 4.0f));
    
    flashlight2 = clamp(flashlight2 * 1.2f, 0.0f, 1.0f);
    return tex.Sample(splr, pin_UV) * float4(light + float3(flashlight2, flashlight2, flashlight2), 1.0f);*/
    
    
    float3 normalNormalized = normalize(pin_normal);
    
    DirectionalLight testDirLight;
    testDirLight.color = float4(1.0f, 1.0f, 1.0f, 0.5f);
    testDirLight.direction = float4(0.0f, -1.0f, 0.0f, 0.87f);

    float3 light = 0.0f;
    //light += CalculateDirectionalLight(testDirLight, normalNormalized, pin_pixel_pos_worldspace);
    
    for (int i = 0; i < POINT_LIGHT_NUM; i++)
    {
        //light += CalculatePointLight(BUF0_pointLight[i], normalNormalized, pin_pixel_pos_worldspace);
    }
    light += CalculatePointLight(BUF0_pointLight[1], normalNormalized, pin_pixel_pos_worldspace);
    //light = clamp(light, float3(0.0f, 0.0f, 0.0f), float3(1.0f, 1.0f, 1.0f));
    //return tex.Sample(splr, pin_UV);
    return tex.Sample(splr, pin_UV) * float4(light, 1.0f);
}

float3 CalculateDirectionalLight(const DirectionalLight directLight, const float3 normNormalWS, const float3 pixelPosWS)
{
    float3 toLightDir = normalize(-directLight.direction.xyz);
    float3 reflectFromLightDir = reflect(directLight.direction.xyz, normNormalWS);
    float diffuse = max(dot(normNormalWS, toLightDir), 0.0f);
    float specular = pow(max(dot(toLightDir, reflectFromLightDir), 0.0f), BUF0_view_position.w) * directLight.color.a;
    
    return (directLight.direction.w + (diffuse * (1.0f - directLight.direction.a)) + specular) * directLight.color.rgb;

}
float3 CalculatePointLight(const PointLight pointLight, const float3 normNormalWS, const float3 pixelPosWS)
{
    float3 toLightDir = normalize(pointLight.position.xyz - pixelPosWS);
    float3 reflectFromLightDir = reflect(-toLightDir, normNormalWS);
    
    float distance = length(pointLight.position.xyz - pixelPosWS);
    float attenuation = 1.0f / (pointLight.params.r +
    (pointLight.params.g * distance) +
    (pointLight.params.b * distance * distance));

    float diffuse = max(dot(normNormalWS, toLightDir), 0.0f);
    //float specular = pow(max(dot(toLightDir, reflectFromLightDir), 0.0f), BUF0_view_position.w) * pointLight.position.w;
    
    return (attenuation * (diffuse)) * pointLight.color.rgb; //pointLight.color.a +
}