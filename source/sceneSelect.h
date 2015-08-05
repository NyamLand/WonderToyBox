
#ifndef __SCENESELECT_H__
#define	__SCENESELECT_H__

//******************************************************************************
//
//	sceneSelectクラス
//
//******************************************************************************
class sceneSelect : public Scene
{
private:
	enum 
	{
		SELECT_PLAYERNUM,
		SELECT_CHARACTER,
		SELECT_STAGE,
		SELECT_CHECK
	};

	iexView*	view;
	int mode;

public:
	//	初期化・解放
	sceneSelect( void );
	~sceneSelect( void );
	bool	Initialize( void );

	//	更新・描画
	void	Update( void );
	void	Render( void );

	//-------------------------------------
	//　各画面ごとのメソッド
	//-------------------------------------
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
};

//******************************************************************************
#endif // !__SCENESELECT_H__
