cbuffer constBuf0 : register(b0)
{
    float4 lightColorBUF0;
}

float4 main(float3 p_normal : Normal, float2 p_UV : UV, float4 p_color : Colory) : SV_Target //uint FaceID : SV_PrimitiveID
{
    //return float4(1.0f, 0.0f, 0.0f, 1.0f);
    //return float4(BUF0_PointLights[0].position.x, 0.0f, 0.0f, 1.0f);
    //return facesColors[FaceID / 2];
    //return p_color;
    return lightColorBUF0;
    return float4(1.0f, 1.0f, 1.0f, 1.0f);
}