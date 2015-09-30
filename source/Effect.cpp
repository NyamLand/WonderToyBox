
#include	"iextreme.h"
#include	"system\System.h"
#include	"GlobalFunction.h"
#include	"Effect.h"
#include	"PlayerManager.h"
#include	"GameManager.h"

//*******************************************************************************
//
//	Effectクラス
//
//*******************************************************************************

	//　実体宣言
	Effect*		m_Effect = nullptr;

//-------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------

	//	コンストラクタ
	Effect::Effect(void) : aura(nullptr), isAura(false), _adjustV(0)
	{
	
	}

	//	デストラクタ
	Effect::~Effect( void )
	{
		SafeDelete(aura);
	}

	//	初期化
	void	Effect::Initialize()
	{
		aura = new iexMesh("DATA\\Effect\\aura.IMO");
	}

//-------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------

	//	更新
	void	Effect::Update( void )
	{
		if (GameManager::GetMode() == GAME_MODE::CLIMAX)	isAura = true;
		if (isAura)
		{
			Vector3 p = m_Player->GetPos(GameManager::GetWorst()) + Vector3(0, 1.5f, 0);
			aura->SetPos(p);
			aura->SetScale(0.02f);
			aura->SetAngle(0.0f);
			aura->Update();
		}
		_adjustV += 0.02f;
		shader3D->SetValue("adjustV", _adjustV);
	}

	//	描画
	void	Effect::Render( void )
	{
		if(isAura)	aura->Render(shader3D,"effect_add");
	}

//-------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------