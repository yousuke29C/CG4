#pragma once

#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <DirectXMath.h>

class PostEffect
{
private:    //�G�C���A�X
    //Microsoft::WRL::���ȗ�
    template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
    //DirectX::���ȗ�
    using XMFLOAT2 = DirectX::XMFLOAT2;
    using XMFLOAT3 = DirectX::XMFLOAT3;
    using XMFLOAT4 = DirectX::XMFLOAT4;
    using XMMATRIX = DirectX::XMMATRIX;

public: // �T�u�N���X
    /// <summary>
    /// ���_�f�[�^�\����
    /// </summary>
    struct VertexPosUv
    {
        XMFLOAT3 pos; // xyz���W
        XMFLOAT2 uv;  // uv���W
    };

    /// <summary>
    /// �萔�o�b�t�@�p�f�[�^�\����
    /// </summary>
    struct ConstBufferData
    {
        XMFLOAT4 color;	// �F (RGBA)
        XMMATRIX mat;	// �R�c�ϊ��s��
    };

protected: // �ÓI�����o�ϐ�
    // �e�N�X�`���̍ő喇��
    static const int srvCount = 512;
    // ���_��
    static const int vertNum = 4;
    // �f�o�C�X
    static ID3D12Device* device;
    // �f�X�N���v�^�T�C�Y
    static UINT descriptorHandleIncrementSize;
    // �R�}���h���X�g
    static ID3D12GraphicsCommandList* cmdList;
    // ���[�g�V�O�l�`��
    static ComPtr<ID3D12RootSignature> rootSignature;
    // �p�C�v���C���X�e�[�g�I�u�W�F�N�g
    static ComPtr<ID3D12PipelineState> pipelineState;
    // �ˉe�s��
    static XMMATRIX matProjection;
    // �f�X�N���v�^�q�[�v
    static ComPtr<ID3D12DescriptorHeap> descHeap;

public:
    /// <summary>
    /// �R���X�g���N�^
    /// </summary>
    PostEffect(UINT texNumber, XMFLOAT2 position, XMFLOAT2 size, XMFLOAT4 color, XMFLOAT2 anchorpoint);

    /// <summary>
    /// ������
    /// </summary>
    void Initialize(ID3D12Device* device);

    /// <summary>
    /// �`��R�}���h�̔��s
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void Draw(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// �V�[���`��O����
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void PreDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// �V�[���`��㏈��
    /// </summary>
    /// <param name="cmdList">�R�}���h���X�g</param>
    void PostDrawScene(ID3D12GraphicsCommandList* cmdList);

    /// <summary>
    /// �p�C�v���C������
    /// </summary>
    void CreateGraphicsPipelineState();

private:
    //��ʃN���A�J���[
    static const float clearColor[4];

private:
    //�e�N�X�`���o�b�t�@
    ComPtr<ID3D12Resource> texBuff;
    //SRV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapSRV;
    //�[�x�o�b�t�@
    ComPtr<ID3D12Resource> depthBuff;
    //RTV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapRTV;
    //DSV�p�f�X�N���v�^�q�[�v
    ComPtr<ID3D12DescriptorHeap> descHeapDSV;
    // ���_�o�b�t�@
    ComPtr<ID3D12Resource> vertBuff;
    // �萔�o�b�t�@
    ComPtr<ID3D12Resource> constBuff;
    // ���_�o�b�t�@�r���[
    D3D12_VERTEX_BUFFER_VIEW vbView{};
    // �e�N�X�`���ԍ�
    UINT texNumber = 0;
    // Z�����̉�]�p
    float rotation = 0.0f;
    // ���W
    XMFLOAT2 position{};
    // �X�v���C�g���A����
    XMFLOAT2 size = { 100.0f, 100.0f };
    // �A���J�[�|�C���g
    XMFLOAT2 anchorpoint = { 0, 0 };
    // ���[���h�s��
    XMMATRIX matWorld{};
    // �F
    XMFLOAT4 color = { 1, 1, 1, 1 };
};