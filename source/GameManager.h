
#ifndef __GAMEMANAGER_H__
#define	__GAMEMANAGER_H__

//*******************************************************************************
//
//	GameManager�N���X
//
//*******************************************************************************
class GameManager
{
private:
	int		timer;


public:
	//	�������E���
	GameManager( void );
	~GameManager( void );
	bool	Initialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );
};


//*******************************************************************************
#endif // !__GAMEMANAGER_H__
