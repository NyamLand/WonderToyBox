
#pragma once

//**********************************************************************
//
//	Curtain�N���X
//
//**********************************************************************

//	class
class Curtain
{
private:	//	�\����
	struct CurtainInfo
	{
		iex2DObj*	obj;
		TLVERTEX	tlv[4];
		float			t;
	};

public:		//	�萔
	enum CURTAIN_MODE
	{
		OPEN,
		CLOSE,
	};

private:	//	�p�����[�^
	CURTAIN_MODE	mode;
	CurtainInfo	curtainR, curtainL;
	float startPosL[2][4];		//	�S���_�Q�p�^�[�����J�[�e��
	float	startPosR[2][4];		//	�S���_�Q�p�^�[���E�J�[�e��
	float	endPosL[2][4];		//	�S���_�Q�p�^�[�����J�[�e��
	float	endPosR[2][4];		//	�S���_�Q�p�^�[���E�J�[�e��
	float speed;
	bool	isEnd;

public:		//	�֐�
	//	�������E�J��
	Curtain( void );
	~Curtain( void );
	void	StartPosInitialize( void );
	void	EndPosInitialize( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	void	Open( void );
	void	Close( void );

	//	���ݒ�
	void	SetMode( CURTAIN_MODE mode, float speed = 0.01f );

	//	���擾
	bool	GetIsEnd( void )const;
};
