
#ifndef		__SCENETITLE_H__
#define	__SCENETITLE_H__

//*****************************************************************************
//
//	sceneTitleクラス
//
//*****************************************************************************

//	タイトル情報
namespace
{
	//	タイトルモード
	namespace TITLE_MODE
	{
		enum
		{
			TITLE,
			MENU,
			PLAY,
			OPTION,
			CREDIT,
			MOVE_MAIN,
		};
	}
}

//	include
#include	"Curtain.h"

//	class
class sceneTitle : public	Scene
{
private:	//	構造体

	//	各キャラクター情報
	struct CharacterInfo
	{
		LPSTR	name;		//	キャラクター名
		bool	select;		//	選択されているか
	};

	//	カメラ用パラメータ構造体
	struct CameraInfo
	{
		int				posNum;				//	仮のカメラ移動先のポジションを指し示す値
		float				t;					//	補間パラメータ
		int				state;			//	状態
		Vector3		pos;				//	カメラポジション
		Vector3		target;				//	カメラのターゲットポジション
		Vector3		lerpStartPos;		//	ラープスタートポジション
	};

	//	カーテン用構造体
	struct CurtainInfo
	{
		iex2DObj*	obj;
		TLVERTEX	tlv[4];
		float			t;
	};

	//	タイトル用パラメータ
	struct TITLE_INFO
	{
		int		step;
		int		nextMode;
		CurtainInfo	curtainR;
		CurtainInfo	curtainL;
		iex2DObj*	titleText;
		ImageObj		pressSpace;
		ImageObj		titleImage;
		ImageObj		gameStartImage;
		ImageObj		creditTextImage;
	};

	//	メニュー用パラメータ
	struct MENU_INFO
	{
		int	 menu_num;
	};

	//	情報選択用パラメータ
	struct SELECT_INFO
	{
		int		mode;
		int		select_mode;
		int		menu_num;
	};

	struct CREDIT_INFO
	{
		iex2DObj*	credit;
		int				sy;
		float				t;
	};

private:
	//	カーテン
	Curtain*	curtain;

	//	背景
	iexMesh*	stage;

	//	カメラ用パラメータ
	CameraInfo	cameraInfo;

	//	ゲーム設定用パラメータ
	CharacterInfo	characterInfo[CHARACTER_TYPE::MAX];
	int				mode;

	//	各モード用パラメータ
	TITLE_INFO		titleInfo;
	MENU_INFO		menuInfo;
	SELECT_INFO	selectInfo;
	CREDIT_INFO	creditInfo;

public:
	//	初期化・解放
	sceneTitle( void );
	~sceneTitle( void );
	bool	Initialize( void );
	void	TitleInitialize( void );
	void	MenuInitialize( void );
	void	CreditInitialize( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//	動作関数
	void	CurtainInitialize( void );

	//-------------------------------------
	//　各画面ごとのメソッド
	//-------------------------------------
	//　タイトル
	void	TitleUpdate( void );
	void	TitleRender( void );

	//　メニュー画面
	void	MenuUpdate( void );
	void	MenuRender( void );

	//　選択画面へ
	void	MoveSelectUpdate( void );
	void	MoveSelectRender( void );

	//　オプション
	void	OptionUpdate( void );
	void	OptionRender( void );

	//　クレジット
	void	CreditUpdate( void );
	void	CreditRender( void );
};

//*****************************************************************************
#endif // !__SCENETITLE_H__
