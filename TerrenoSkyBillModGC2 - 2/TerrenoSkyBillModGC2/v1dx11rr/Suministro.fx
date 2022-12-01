Texture2D colormap : register(t0);
SamplerState colorSampler : register(s0);

cbuffer cbChangesEveryFrame : register(b0)
{
    matrix worldMatrix;
};

cbuffer cbNeverChanges : register(b1)
{
    matrix viewMatrix;
};

cbuffer cbChangeOnResize : register(b2)
{
    matrix projMatrix;
};

struct VS_Input
{
    float4 pos : POSITION;
    float2 tex0 : TEXCOORD0;
    float3 norma : NORMAL;
};

struct PS_Input
{
    float4 pos : SV_POSITION;
    float2 tex0 : TEXCOORD0;
    float3 norma : NORMAL;
    float3 lightVec : TEXCOORD1;
    float3 cameraVec : TEXCOORD2;
};

PS_Input VS_Main(VS_Input vertex)
{
    PS_Input vsOut = (PS_Input) 0;
    
    float4 worldPos = mul(vertex.pos, worldMatrix);
    vsOut.pos = mul(worldPos, viewMatrix);
    vsOut.pos = mul(vsOut.pos, projMatrix);
    vsOut.tex0 = vertex.tex0;
    
    vsOut.norma = mul(vertex.norma, (float3x3) worldMatrix);
    vsOut.norma = normalize(vsOut.norma);
    
    float3 lightPos = float3(10.0f, 100.0f, -100.0f);
    
    vsOut.lightVec = normalize(lightPos - worldPos);
    
    
    
    return vsOut;
}

float4 PS_Main(PS_Input frag): SV_TARGET
{
    
    float3 Ambiental = float3(0.0f, 1.0f, 0.0f);
    float3 Difusa = float3(0.8f, 0.5f, 0.5f);
    
    float3 normal = normalize(frag.norma);
    float3 lightVec = normalize(frag.lightVec);
    
    float intensidadD = saturate(dot(normal, lightVec));
    
    float3 final = Ambiental + (Difusa * intensidadD);
    
    float4 textura = colormap.Sample(colorSampler, frag.tex0);
    
    return colormap.Sample(colorSampler, frag.tex0) * float4(final, 1.0f);
}


