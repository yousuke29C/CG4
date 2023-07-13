#include "PostEffectTest.hlsli"

// 0番スロットに設定されたテクスチャ
Texture2D<float4> tex0 : register(t0);
// 1番スロットに設定されたテクスチャ
Texture2D<float4> tex1 : register(t1);
// 0番スロットに設定されたサンプラー
SamplerState smp : register(s0);

float4 main(VSOutput input) : SV_TARGET
{
	float4 colortex0 = tex0.Sample(smp, input.uv);
	float4 colortex1 = tex1.Sample(smp, input.uv);
	float4 blur = 0.1f;

	//テクスチャのサイズを指定
	int uWidth = 512;  // テクスチャの幅
	int vHeight = 512; // テクスチャの高さ

	// サンプリング範囲の設定
	float2 texelSize = float2(1.0 / uWidth, 1.0 / vHeight);

	// カーネルサイズ
	int kernelSize = 3;

	// サンプリング範囲内のピクセルを合計
	for (int x = -kernelSize; x <= kernelSize; x++)
	{
		for (int y = -kernelSize; y <= kernelSize; y++)
		{
			float2 offset = float2(x, y) * texelSize;
			blur += tex1.Sample(smp, input.uv + offset);
		}
	}

	// ピクセル数で割って平均化
	colortex1.rgb = blur / 9.0f;

	float4 color = 1.0f - colortex0;

	if (fmod(input.uv.y, 0.1f) < 0.05f)
	{
		color = lerp(colortex0, colortex1, 0.4f);
	}

	return float4(color.rgb, 1);
}