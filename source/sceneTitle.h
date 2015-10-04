
#ifndef __SCENETITLE_H__
#define	__SCENETITLE_H__

//*****************************************************************************
//
//	sceneTitleクラス
//
//*****************************************************************************

class sceneTitle : public	Scene
{
private:	//	定数
	static const int STAGE_MAX = 8;

private:	//	構造体

	//	各キャラクター情報
	struct CharacterInfo
	{
		LPSTR	name;		//	キャラクター名
		bool	select;		//	選択されているか
	};

	//	選択情報
	struct SelectInfo
	{
		int		characterType[4];
		int 	playerNum;		//　何人で遊ぶ？
		int		stageType;		//　どのステージ？
		bool	ready;			//	準備OKか？
		int		step_cs;
	};

	//	カメラ用パラメータ構造体
	struct CameraInfo
	{
		int			posNum;				//	仮のカメラ移動先のポジションを指し示す値
		float		t;					//	補間パラメータ
		Vector3		pos;				//	カメラポジション
		Vector3		target;				//	カメラのターゲットポジション
		Vector3		lerpStartPos;		//	ラープスタートポジション
	};

private:
	//	カメラ用パラメータ
	CameraInfo	cameraInfo;

	//	ゲーム設定用パラメータ
	CharacterInfo	characterInfo[CHARACTER_TYPE::MAX];
	SelectInfo		selectInfo;
	int				mode;

	//	タイトル用パラメータ
	iex2DObj*	curtain;
	iexMesh*	stage;


public:
	//	初期化・解放
	sceneTitle( void );
	~sceneTitle( void );
	bool	Initialize( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//-------------------------------------
	//　各画面ごとのメソッド
	//-------------------------------------
	//　タイトル
	void	TitleUpdate( void );
	void	TitleRender( void );

	//　メニュー画面
	void	MenuUpdate( void );
	void	MenuRender( void );

	//　人数選択
	void	SelectPlayerNumUpdate( void );
	void	SelectPlayerNumRender( void );

	//　キャラ選択
	void	SelectCharacterUpdate( void );
	void	SelectCharacterRender( void );
	
	//　ステージ選択
	void	SelectStageUpdate( void );
	void	SelectStageRender( void );
	
	//　最終確認
	void	SelectCheckUpdate( void );
	void	SelectCheckRender( void );

	//　オプション
	void	OptionUpdate( void );
	void	OptionRender( void );

	//　クレジット
	void	CreditUpdate( void );
	void	CreditRender( void );
};

//*****************************************************************************
#endif // !__SCENETITLE_H__
