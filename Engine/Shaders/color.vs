
cbuffer MatrixBuffer {
	matrix WorldMatrix;
	matrix ViewMatrix;
	matrix ProjectionMatrix;
};

struct VertexInputType {
	float4 Position : POSITION;
	float4 Color : COLOR;
};

struct PixelInputType {
	float4 Position : SV_POSITION;
	float4 Color : COLOR;
};

PixelInputType ColorVertexShader(VertexInputType Input) {
	PixelInputType Output;

	Input.Position.w = 1.f;

	Output.Position = mul(Input.Position, WorldMatrix);
	Output.Position = mul(Input.Position, ViewMatrix);
	Output.Position = mul(Output.Position, ProjectionMatrix);

	Output.Color = Input.Color;
	return Output;
}