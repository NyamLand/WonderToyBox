//*****************************************************************************************************************************
//
//		メインシーン
//
//*****************************************************************************************************************************

class	sceneMain : public Scene
{
private:
	//	定数
	static	const		int		SHADOW_SIZE = 2048;

private:
	//	オブジェクト
	iexMesh*	m_CollisionStage;
	iexMesh*	m_Stage;

	//	カメラパラメータ
	Vector3	ViewPos;

	//	テクスチャ
	iex2DObj*	ShadowTex;

	//	バッファ
	Surface*	backBuffer;
	Surface*	ShadowZ;

public:
	//	初期化・解放
	sceneMain( void );
	~sceneMain( void );
	bool Initialize( void );
	void	PlayerInitialize( void );

	//	更新
	void Update( void );	//	更新

	//	描画
	void	Render( void );
	void	RenderShadowBuffer( void );
};


