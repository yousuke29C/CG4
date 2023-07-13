#include "GameScene.h"
#include <cassert>
#include <sstream>
#include <iomanip>
#include "FbxLoader.h"

using namespace DirectX;

GameScene::GameScene()
{
}

GameScene::~GameScene()
{
	safe_delete(spriteBG);
	safe_delete(lightGroup);
	safe_delete(object1);
	safe_delete(model1);
}

void GameScene::Initialize(DirectXCommon* dxCommon, Input* input, Audio* audio)
{
	// nullptrチェック
	assert(dxCommon);
	assert(input);
	assert(audio);

	this->dxCommon = dxCommon;
	this->input = input;
	this->audio = audio;

	// カメラ生成
	camera = new DebugCamera(WinApp::window_width, WinApp::window_height, input);
	//デバイスセット
	Object3d::SetDevice(dxCommon->GetDevice());
	//カメラをセット
	Object3d::SetCamera(camera);

	//グラフィックスパイプライン生成
	Object3d::CreateGraphicsPipeline();

	// デバッグテキスト用テクスチャ読み込み
	if (!Sprite::LoadTexture(debugTextTexNumber, L"Resources/debugfont.png")) {
		assert(0);
		return;
	}
	// デバッグテキスト初期化
	debugText = DebugText::GetInstance();
	debugText->Initialize(debugTextTexNumber);

	// テクスチャ読み込み
	if (!Sprite::LoadTexture(1, L"Resources/background.png")) {
		assert(0);
		return;
	}
	// 背景スプライト生成
	spriteBG = Sprite::Create(1, { 0.0f,0.0f });
	// パーティクルマネージャ生成
	particleMan = ParticleManager::GetInstance();
	particleMan->SetCamera(camera);

	// テクスチャ2番に読み込み
	Sprite::LoadTexture(2, L"Resources/tex1.png");

	//モデル名を指定してファイル読み込み
	model1 = FbxLoader::GetInstance()->LoadModelFromFile("boneTest");
	//model1 = FbxLoader::GetInstance()->LoadModelFromFile("cube");

	//3Dオブジェクト作成とモデルのセット
	object1 = new Object3d;
	object1->Initialize();
	object1->SetModel(model1);

	// ライト生成
	lightGroup = LightGroup::Create();

	// カメラ注視点をセット
	camera->SetTarget({ 0,3,0 });
	camera->SetDistance(10.0f);

}

void GameScene::Update()
{
	lightGroup->Update();
	camera->Update();
	object1->Update();
	particleMan->Update();

}

void GameScene::Draw()
{
	// コマンドリストの取得
	ID3D12GraphicsCommandList* cmdList = dxCommon->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(cmdList);
	// 背景スプライト描画
	spriteBG->Draw();

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon->ClearDepthBuffer();
#pragma endregion

#pragma region 3D描画
	//3Dオブジェクトの描画
	object1->Draw(cmdList);

	// パーティクルの描画
	particleMan->Draw(cmdList);
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(cmdList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>


	// デバッグテキストの描画
	debugText->DrawAll(cmdList);

	// スプライト描画後処理
	Sprite::PostDraw();
#pragma endregion
}