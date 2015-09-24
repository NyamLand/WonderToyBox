
#include	"iextreme.h"
#include	"Random.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"Object.h"

//*******************************************************************************
//
//	Objectクラス
//
//*******************************************************************************

//-------------------------------------------------------------------------------
//	グローバル
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	初期化・解放
//-------------------------------------------------------------------------------

	//	コンストラクタ
	Object::Object()
	{
	
	}

	//	デストラクタ
	Object::~Object()
	{

	}

	//	初期化
	bool	Object::Initialize()
	{
		obj = nullptr;
		return	true;
	}
	
	//	解放
	void	Object::Release()
	{
		SafeDelete( obj );
	}

//-------------------------------------------------------------------------------
//	更新・描画
//-------------------------------------------------------------------------------

	//	更新
	void	Object::Update()
	{
		obj->Update();
	}

	//	描画
	void	Object::Render( iexShader* shader, LPSTR technique )
	{
		if ( shader == nullptr || technique == "" )
		{
			obj->Render();
		}
		else
		{
			obj->Render( shader, technique );
		}
	}

//-------------------------------------------------------------------------------
//	動作関数
//-------------------------------------------------------------------------------

//-------------------------------------------------------------------------------
//	情報取得
//-------------------------------------------------------------------------------