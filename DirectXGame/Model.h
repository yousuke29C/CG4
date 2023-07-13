#pragma once

#include <string>
#include <DirectXMath.h>
#include <vector>
#include <DirectXTex.h>
#include <Windows.h>
#include <wrl.h>
#include <d3d12.h>
#include <d3dx12.h>
#include <fbxsdk.h>

using namespace DirectX;

struct Node
{
	//���O
	std::string name;
	//���[�J���X�P�[��
	XMVECTOR scaling = { 1,1,1,0 };
	//���[�J����]�p
	XMVECTOR rotation = { 0,0,0,0 };
	//���[�J���ړ�
	XMVECTOR translation = { 0,0,0,1 };
	//���[�J���ό`�s��
	XMMATRIX transform;
	//�O���[�o���ό`�s��
	XMMATRIX gloabalTransform;
	//�e�m�[�h
	Node* parent = nullptr;
};

class Model
{
private:	//�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template <class T>using ComPtr = Microsoft::WRL::ComPtr<T>;
	//DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;
	using TexMetadata = DirectX::TexMetadata;
	using ScratchImage = DirectX::ScratchImage;
	//std::���ȗ�
	using string = std::string;
	template <class T> using vector = std::vector<T>;

public:		//�萔
	//�{�[���C���f�b�N�X�̍ő吔
	static const int MAX_BONE_INDICES = 4;

public:	//�T�u�N���X
	//���_�f�[�^�\����
	struct VertexPosNormalUvSkin
	{
		XMFLOAT3 pos;		//xyz���W
		XMFLOAT3 normal;	//�@���x�N�g��
		XMFLOAT2 uv;		//uv���W
		UINT boneIndex[MAX_BONE_INDICES];	//�{�[���@�ԍ�
		float boneWeight[MAX_BONE_INDICES];	//�{�[���@�d��
	};

	//�{�[���\����
	struct Bone
	{
		//���O
		std::string name;
		//�����p���̋t�s��
		DirectX::XMMATRIX invInitialPose;
		//�N���X�^�[(FBX���̃{�[�����)
		FbxCluster* fbxCluster;
		//�R���X�g���N�^
		Bone(const std::string& name)
		{
			this->name = name;
		}
	};

public:
	//�t�����h�N���X
	friend class FbxLoader;

public:
	//�f�X�g���N�^
	~Model();

	//�o�b�t�@����
	void CreateBuffers(ID3D12Device* device);

	//�`��
	void Draw(ID3D12GraphicsCommandList* cmdList);

public:	//�A�N�Z�b�T
	//���f���̕ό`�s��擾
	const XMMATRIX& GetModelTransform() { return meshNode->gloabalTransform; }
	//�{�[��getter
	std::vector<Bone>& GetBones() { return bones; }
	//fbxgetter
	FbxScene* GetFbxScene() { return fbxScene; }

private:
	//���f����
	std::string name;
	//�m�[�h�z��
	std::vector<Node> nodes;
	//���b�V�������m�[�h
	Node* meshNode = nullptr;
	//���_�f�[�^�z��
	std::vector<VertexPosNormalUvSkin> vertices;
	//���_�C���f�b�N�X�z��
	std::vector<unsigned short> indices;
	//�A���r�G���g�W��
	XMFLOAT3 ambient = { 1,1,1 };
	//�f�B�t���[�Y�W��
	XMFLOAT3 diffuse = { 1,1,1 };
	//�e�N�X�`�����^�f�[�^
	TexMetadata metadata = {};
	//�X�N���b�`�C���[�W
	ScratchImage scratchImg = {};
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//�C���f�b�N�X�o�b�t�@
	ComPtr<ID3D12Resource> indexBuff;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView = {};
	//�C���f�b�N�X�o�b�t�@�r���[
	D3D12_INDEX_BUFFER_VIEW ibView = {};
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//�{�[���z��	
	std::vector<Bone> bones;
	//FBX�V�[��
	FbxScene* fbxScene = nullptr;
};