cbuffer MatrixBuffer : register(b0) {
    float4x4 model_view_projection;
};

struct VSInput {
    float3 position : POSITION;
};

struct VSOutput {
    float4 position : SV_Position;
};

VSOutput main(VSInput input) {
    VSOutput output;
    output.position = mul(model_view_projection, float4(input.position, 1.0));
    return output;
}
