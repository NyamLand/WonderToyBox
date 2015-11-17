
#pragma once

//*******************************************************************************
//
//	sceneMenuクラス
//
//*******************************************************************************

//	include
#include	<memory>
#include	"BaseChara.h"
using namespace std;

//	class
class sceneMenu	:	public	Scene
{
private:
	struct PlayerNumSelectInfo
	{
		float	t;
		int		num;
		int		saveY;
		int		sy;
	};

	struct CharacterSelectInfo
	{
		int		step;
		int		timer;
		int		character[4];
		int		imagePos;
		int		playerNum;
		bool	select[4];
	};

	struct StageSelectInfo
	{
		float	angle;
		int		stage;
	};

	struct CheckSelectInfo
	{
		bool	select;
		bool	check;
		int		step;
	};

	struct OptionInfo
	{
		bool	itemflg;
	};
private:	
	//	画像・モデル
	unique_ptr<iex2DObj>	back;
	unique_ptr<iex2DObj>	frame;
	unique_ptr<iex2DObj>	checkBack;
	unique_ptr<iex2DObj>	checkCursor;
	unique_ptr<iex2DObj>	playerNumText;
	unique_ptr<iexMesh>	deskStage;
	unique_ptr<iexMesh>	forestStage;	
	unique_ptr<iex2DObj>	playerNum;
	unique_ptr<iex3DObj>	org[4];
	iex3DObj*						obj[4];
	iex2DObj*						face;
	iex2DObj*						cursor;
	ImageObj						textImage;
	ImageObj						faceImage[CHARACTER_TYPE::MAX];
	ImageObj						cursorImage[4];

	//	各モード情報
	PlayerNumSelectInfo		playerNumSelectInfo;
	CharacterSelectInfo		characterSelectInfo;
	StageSelectInfo				stageSelectInfo;
	CheckSelectInfo				checkSelectInfo;
	OptionInfo						 optionInfo;

	//	共通変数
	int		mode;
	
public:
	//	初期化・解放
	sceneMenu( void );
	~sceneMenu( void );
	bool	Initialize( void );
	void	Release( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	プレイヤー人数選択関数
	void	SelectPlayerNumInitialize( void );
	void	SelectPlayerNumUpdate( void );
	void	SelectPlayerNumRender( void );

	//	キャラ選択関数
	void	SelectCharacterInitialize( void );
	void	SelectCharacterUpdate( void );
	void	SelectCharacterRender( void );

	//	ステージ選択関数
	void	SelectStageInitialize( void );
	void	SelectStageUpdate( void );
	void	SelectStageRender( void );

	//	チェック関数
	void	SelectCheckInitialize( void );
	void	SelectCheckUpdate( void );
	void	SelectCheckRender( void );

	//	メイン移動関数
	void	MoveMainInitialize( void );
	void	MoveMainUpdate( void );
	void	MoveMainRender( void );

	//	タイトル移動関数
	void	MoveTitleInitialize( void );
	void	MoveTitleUpdate( void );
	void	MoveTitleRender( void );

	//	オプション関数
	void	OptionInitialize( void );
	void	OptionUpdate( void );
	void	OptionRender( void );

	//	情報設定
	void	SetMode( int mode );

};
