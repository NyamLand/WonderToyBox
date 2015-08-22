
#ifndef __SCENETITLE_H__
#define	__SCENETITLE_H__

//*****************************************************************************
//
//	sceneTitleクラス
//
//*****************************************************************************

class sceneTitle : public	Scene
{
private:
	//	定数
	enum Title_Mode
	{
		TITLE,
		MENU,
		SELECT_PLAYERNUM,
		SELECT_CHARACTER,
		SELECT_STAGE,
		SELECT_CHECK,
		OPTION,
		CREDIT
	};

	//	構造体
	struct CharacterInfo
	{
		LPSTR name;
		bool	select;
	};

private:
	//	オブジェクト
	iexMesh*	m_CollisionStage;
	iexMesh*	m_Stage;	

	//	カメラ用パラメータ
	int			testpos;			//	仮のカメラ移動先のポジションを指し示す値
	float		t;					//	補間パラメータ
	Vector3		c_pos;		//	カメラポジション
	Vector3		t_pos;		//	カメラのターゲットポジション
	Vector3		s_pos;		//	ラープスタートポジション
	Quaternion orientation;	//	クォータニオン
	
	//	ゲーム設定用パラメータ
	int		mode;
	CharacterInfo	characterInfo[PlayerData::CHARACTER_MAX];
	int 	playerNum;	//　何人で遊ぶ？
	int		stageType;	//　どのステージ？

public:
	//	初期化・解放
	sceneTitle( void );
	~sceneTitle( void );
	bool	Initialize( void );
	void	InitSound( void );

	//	更新・描画
	void	Update(void);
	void	Render(void);

	//-------------------------------------
	//　各画面ごとのメソッド
	//-------------------------------------
	//　タイトル
	void	TitleUpdate(void);
	void	TitleRender(void);
	//　メニュー画面
	void	MenuUpdate(void);
	void	MenuRender(void);

	//　人数選択
	void	SelectPlayerNumUpdate(void);
	void	SelectPlayerNumRender(void);
	//　キャラ選択
	void	SelectCharacterUpdate(void);
	void	SelectCharacterRender(void);
	//　ステージ選択
	void	SelectStageUpdate(void);
	void	SelectStageRender(void);
	//　最終確認
	void	SelectCheckUpdate(void);
	void	SelectCheckRender(void);

	//　オプション
	void	OptionUpdate(void);
	void	OptionRender(void);

	//　クレジット
	void	CreditUpdate(void);
	void	CreditRender(void);
};

//*****************************************************************************
#endif // !__SCENETITLE_H__
