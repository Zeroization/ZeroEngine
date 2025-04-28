module test;

// 顶点输入结构
struct VertexInput
{
    float3 position : POSITION;
};

// 顶点输出结构（传递给片段着色器）
struct VertexOutput
{
    float4 position : SV_Position;
};

// 顶点着色器
[shader("vertex")]
VertexOutput vertexMain(VertexInput input)
{
    VertexOutput output;

    output.position = float4(input.position, 1.0);

    return output;
}

// 片段着色器
[shader("fragment")]
float4 fragmentMain(VertexOutput input)
    : SV_Target
{
    return float4(1.0, 0.5, 0.2, 1.0); // 强制红色
}
