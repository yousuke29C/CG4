#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class PostEffect
{
private:    //エイリアス
    //Microsoft::WRL::を省略
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
    //DirectX::を省略
    using XMFLOAT2 = DirectX::XMFLOAT2;
    using XMFLOAT3 = DirectX::XMFLOAT3;
    using XMFLOAT4 = DirectX::XMFLOAT4;
    using XMMATRIX = DirectX::XMMATRIX;

public: // サブクラス
    /// <summary>
    /// 頂点データ構造体
    /// </summary>
    struct VertexPosUv
    {
        XMFLOAT3 pos; // xyz座標
        XMFLOAT2 uv;  // uv座標
    };

    /// <summary>
    /// 定数バッファ用データ構造体
    /// </summary>
    struct ConstBufferData
    {
        XMFLOAT4 color;	// 色 (RGBA)
        XMMATRIX mat;	// ３Ｄ変換行列
    };

protected: // 静的メンバ変数
    // テクスチャの最大枚数
    static const int srvCount = 512;
    // 頂点数
    static const int vertNum = 4;
    // デバイス
    static ID3D12Device* device;
    // デスクリプタサイズ
    static UINT descriptorHandleIncrementSize;
    // コマンドリスト
    static ID3D12GraphicsCommandList* cmdList;
    // ルートシグネチャ
    static ComPtr<ID3D12RootSignature> rootSignature;
    // パイプラインステートオブジェクト
    static ComPtr<ID3D12PipelineState> pipelineState;
    // 射影行列
    static XMMATRIX matProjection;
    // デスクリプタヒープ
    static ComPtr<ID3D12DescriptorHeap> descHeap;

public:
    /// <summary>
    /// コンストラクタ
    /// </summary>
    PostEffect(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint);

    /// <summary>
    /// 初期化
    /// </summary>
    void Initialize(ID3D12Device* device);

    /// <summary>
    /// 描画コマンドの発行
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void Draw(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// シーン描画前処理
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// シーン描画後処理
    /// </summary>
    /// <param name="cmdList">コマンドリスト</param>
    void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// パイプライン生成
    /// </summary>
    void CreateGraphicsPipelineState();

private:
    //画面クリアカラー
    static const float clearColor[4];

private:
    //テクスチャバッファ
    ComPtr<ID3D12Resource> texBuff;
    //SRV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //深度バッファ
    ComPtr<ID3D12Resource> depthBuff;
    //RTV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV用デスクリプタヒープ
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;
    // 頂点バッファ
    ComPtr<ID3D12Resource> vertBuff;
    // 定数バッファ
    ComPtr<ID3D12Resource> constBuff;
    // 頂点バッファビュー
    D3D12_VERTEX_BUFFER_VIEW vbView{};
    // テクスチャ番号
    UINT texNumber = 0;
    // Z軸回りの回転角
    float rotation = 0.0f;
    // 座標
    XMFLOAT2 position{};
    // スプライト幅、高さ
    XMFLOAT2 size = { 100.0f, 100.0f };
    // アンカーポイント
    XMFLOAT2 anchorpoint = { 0, 0 };
    // ワールド行列
    XMMATRIX matWorld{};
    // 色
    XMFLOAT4 color = { 1, 1, 1, 1 };
};