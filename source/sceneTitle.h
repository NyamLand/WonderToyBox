
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
	//iexView*	view;

	//	オブジェクト
	iexMesh*	m_CollisionStage;
	iexMesh*	m_Stage;	
	int			testpos;	//	仮のカメラ移動先のポジションを指し示す値
	float		t;			//	ラープのｔ
	Vector3		c_pos;		//	カメラポジション
	Vector3		s_pos;		//	ラープスタートポジション
	Vector3		t_pos;		//	カメラのターゲットポジション

	int			state_step;	//	ステート管理用
	int			scene_change;	//	切り替え用ポジション番号

	iex2DObj*	screen;
	
	Quaternion orientation;	//	クォータニオン


public:
	//	初期化・解放
	sceneTitle( void );
	~sceneTitle( void );
	bool	Initialize( void );
	void	InitSound( void );

	//	更新・描画
	void	Update( void );
	void	SceneState( void );
	void	Render( void );
};

#define MAX_TARGET 5

//*****************************************************************************
#endif // !__SCENETITLE_H__
