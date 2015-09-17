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
	static	const		int		SECOND = 60;			//	１秒
	static	const		int		MINUTE = 60 * 60;	//	１分
	static	const		int		TIMELIMIT = 10 * SECOND;		//	秒数×秒

	//	変数
	int		timer;
	int		playerNum;
	int		stageType;

private:
	//	オブジェクト
	iexMesh*	m_CollisionStage;
	iexMesh*	m_Stage;

	//	カメラパラメータ
	Vector3		ViewPos;

	//	テクスチャ
	iex2DObj*	ShadowTex;
	iex2DObj*	screen;

	//	ディファード用
	iex2DObj*	diffuse;
	iex2DObj*	specular;
	iex2DObj*	depth;
	iex2DObj*	normal;
	iex2DObj*	light;
	iex2DObj*	light_s;

	//	バッファ
	Surface*	backBuffer;
	Surface*	ShadowZ;

public:
	//	初期化・解放
	sceneMain( void );
	~sceneMain( void );
	bool Initialize( void );
	void	PlayerInitialize( void );
	void	DifferedInitialize( void );

	//	更新
	void	Update( void );	//	更新

	//	描画
	void	Render( void );
	void	RenderShadowBuffer( void );
	void	RenderDiffered( void );

	//	動作関数
	void	PointLight( const Vector3& pos, const Vector3& color, float range );
	void	DirLight( const Vector3& dir, const Vector3& color );
};

