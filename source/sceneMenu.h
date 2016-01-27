
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
		int		life;
		int		minute;
		int		second;
		int		step;
	};

	struct BGInfo{
		Vector3 target;
		Vector3 start;
		Vector3 end;
		Vector3	cpos;
		Vector3 cspos;//カメラ移動用スタートポジション
		Vector3 cepos;//カメラ移動用エンドポジション
		float t;	//補間パラメータ
		float mt;	//移動用補間パラメータ
		bool moveflg;//移動用フラグ
	};
private:	
	//option設計中
	iex2DObj*				optionImage;
	iex2DObj*				optionTime;
	iex2DObj*				optionLife;
	iex2DObj*				optionMenu;
	unique_ptr<iexMesh>		BG;
	
public:
	//	オプション関数
	void	OptionInitialize(void);
	void	OptionUpdate(void);
	void	OptionRender(void);
	void	ArrowRender(void);
	void	TimerRender(void);
	void	OptionSelectRender(void);
	//void	OptionDUpdate(void);

private:
	//	画像・モデル
	unique_ptr<iex2DObj>	back;
	unique_ptr<iex2DObj>	frame;
	unique_ptr<iex2DObj>	checkBack;
	unique_ptr<iex2DObj>	checkCursor;
	unique_ptr<iex2DObj>	playerNumText;
	unique_ptr<iexMesh>		deskStage;
	unique_ptr<iexMesh>		toyStage;
	unique_ptr<iex2DObj>	playerNum;
	unique_ptr<iex3DObj>	org[4];
	unique_ptr<iexMesh>		orgCannon;
	iexMesh*					cannon[4];
	iex3DObj*				obj[4];
	iex2DObj*				face;
	iex2DObj*				cursor;
	iex2DObj*				cpuCursor;
	iex2DObj*				decidecursor;
	iex2DObj*				selectCheckCursor;
	iex2DObj*				triangleCursor;
	ImageObj				textImage;
	ImageObj				faceImage[CHARACTER_TYPE::MAX];
	ImageObj				cursorImage[4];
	ImageObj				decidecursorImage[4];


	//	各モード情報
	PlayerNumSelectInfo		playerNumSelectInfo;
	CharacterSelectInfo		characterSelectInfo;
	StageSelectInfo			stageSelectInfo;
	CheckSelectInfo			checkSelectInfo;
	OptionInfo				optionInfo;
	BGInfo					bgInfo;

	//	共通変数
	int		mode;
	int		tempmode;
	bool	changeScene;
	Vector3 dir;
	Vector3 modelPos[4];
	Vector3 modelAngle[4];
	Vector3 modelScale[4];
	
public:
	//	初期化・解放
	sceneMenu( void );
	~sceneMenu( void );
	bool	Initialize( void );
	void	Release( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );
	void	CameraUpdate( void );
	void	ChangeToOption( void );
	void	ModelUpdate( void );
	void	CannonRender( void );

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


	//	情報設定
	void	SetMode( int mode );
	void SetCannonPos( void );

};
