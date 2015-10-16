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
	static	const		int		GAME_START_COIN_NUM = 100;	//	百枚

	//	変数
	int		timer;
	int		playerNum;
	int		stageType;
	int		gameStartCoinNum;

private:
	//	オブジェクト
	iexMesh*	m_CollisionStage;
	iexMesh*	m_Stage;

	//	カメラパラメータ
	Vector3		ViewPos;

	//	テクスチャ
	iex2DObj*	ShadowTex;
	iex2DObj*	RefTex;

	//	バッファ
	iex2DObj*	hdr;
	Surface*	backBuffer;
	Surface*	ShadowZ;

	//	ディファード用
	//iex2DObj*	diffuse;
	//iex2DObj*	specular;
	//iex2DObj*	depth;
	//iex2DObj*	normal;
	//iex2DObj*	light;
	//iex2DObj*	light_s;

public:
	//	初期化・解放
	sceneMain( void );
	~sceneMain( void );
	bool Initialize( void );
	void	PlayerInitialize( void );
	void	StageInitialize( void );
	void	DifferedInitialize( void );

	//	更新
	void	Update( void );	//	更新
	void	StartUpdate( void );
	void	MainGameUpdate( void );
	void	DonketsuUpdate( void );
	void	ClimaxUpdate( void );
	void	FinishUpdate( void );
	void	AllUpdate( void );

	//	描画
	void	Render( void );
	void	RenderShadowBuffer( void );
	void	RenderDiffered( void );
	void	RenderRef( void );
	void	RenderHDR( void );

	//	動作関数
	void	PointLight( const Vector3& pos, const Vector3& color, float range );
	void	DirLight( const Vector3& dir, const Vector3& color );
};

