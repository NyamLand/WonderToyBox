//*****************************************************************************************************************************
//
//		���C���V�[��
//
//*****************************************************************************************************************************

class	sceneMain : public Scene
{
private:

private:
	//	�I�u�W�F�N�g
	iexMesh*	m_Stage;

public:
	//	�������E���
	sceneMain( void );
	~sceneMain( void );
	bool Initialize( void );
	void	PlayerInitialize( void );

	//	�X�V�E�`��
	void Update( void );	//	�X�V
	void Render( void );	//	�`��
};


