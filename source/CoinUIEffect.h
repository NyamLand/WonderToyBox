
#pragma once

//**********************************************************************
//
//	CoinUIEffect�N���X
//
//**********************************************************************

//	include

//	�N���X�O���錾
class Camera;

//	class
class CoinUIEffect
{
private:
	//	�p�[�e�B�N���p
	Vector3	parPos;
	bool		coinFlag[PLAYER_MAX];
	int			coinTimer[PLAYER_MAX];
	int			posX[PLAYER_MAX];
	int			posY;
	int			scale;

	//	�p�[�e�B�N���p�o�b�N�o�b�t�@
	iex2DObj*	targetPar;

	//	�p�[�e�B�N���p�J����
	Camera*		camera;

public:
	//	�������E���
	CoinUIEffect( void );
	~CoinUIEffect( void );
	bool	Initialize( void );
	void	Release( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( int player );
	void	RenderToBackBaffer( void );
	
	//	���ݒ�
	void	SetEffectFlag( int playerNum, bool state );
	void	SetPos( int playerNum, int x, int y );
	void	SetScale( int scale );
};
