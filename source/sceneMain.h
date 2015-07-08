//*****************************************************************************************************************************
//
//		メインシーン
//
//*****************************************************************************************************************************

class	sceneMain : public Scene
{
private:

private:
	//	オブジェクト
	iexMesh*	m_Stage;

public:
	//	初期化・解放
	sceneMain( void );
	~sceneMain( void );
	bool Initialize( void );
	void	PlayerInitialize( void );

	//	更新・描画
	void Update( void );	//	更新
	void Render( void );	//	描画
};


