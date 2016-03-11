#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"GameManager.h"
#include	"CharacterManager.h"
#include	"NumberUI.h"
#include	"FaceUI.h"

//**********************************************************************
//
//	FaceUIクラス
//
//**********************************************************************

//------------------------------------------------------------------------------------
//	グローバル
//------------------------------------------------------------------------------------

#define FACEIMAGE_SCALE 256

//------------------------------------------------------------------------------------
//	初期化・解放
//------------------------------------------------------------------------------------
	
	//	コンストラクタ
	FaceUI::FaceUI( void )
	{
	}

	//	デストラクタ
	FaceUI::~FaceUI(void)
	{
		SafeDelete( face );
	}

	void	FaceUI::Initialize( void )
	{
		FOR( 0, PLAYER_MAX ){
			ImageInitialize( faceImage[value], posXTable[value],
				static_cast<int>( iexSystem::ScreenHeight * 0.1f ),
				NUMBER_SCALE + NUMBER_SCALE,		//	数字2文字分の横幅
				NUMBER_SCALE + NUMBER_SCALE,		//	数字2文字分の縦幅
				0, 0, FACEIMAGE_SCALE, FACEIMAGE_SCALE );
		}
	}

	void	FaceUI::TableInitialize( void )
	{
		//	座標テーブル初期化
		posXTable[0] = static_cast<int>( iexSystem::ScreenWidth * 0.1f );
		posXTable[1] = static_cast<int>( iexSystem::ScreenWidth * 0.3f );
		posXTable[2] = static_cast<int>( iexSystem::ScreenWidth * 0.7f );
		posXTable[3] = static_cast<int>( iexSystem::ScreenWidth * 0.9f );

	}
//------------------------------------------------------------------------------------
//	更新・描画
//------------------------------------------------------------------------------------
	//	更新
	void	FaceUI::Update( void )
	{
		FOR( 0, PLAYER_MAX ){
			FaceImageUpdate( value, gameManager->GetCharacterType( value ) );
		}
	}


	//	顔画像構造体更新
	void	FaceUI::FaceImageUpdate( int num, int mode )
	{
		faceImage[num].alpha = 0.5f;
		faceImage[num].sy = 256 * mode;
	}

	//	描画
	void	FaceUI::Render( void )
	{
		FOR( 0, PLAYER_MAX ){
			RenderImage( faceImage[value], faceImage[value].sx, faceImage[value].sy, faceImage[value].sw, faceImage[value].sh, IMAGE_MODE::ADOPTPARAM );
		}
	}