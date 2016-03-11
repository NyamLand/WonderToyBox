
#pragma once

//**********************************************************************
//
//	NumberUI�N���X
//
//**********************************************************************

//	include

//-------------------------------------------------------------------------------------
//	�O���[�o��
//-------------------------------------------------------------------------------------

#define	NUMBER_SCALE	100

//	class
class NumberUI
{
private:
	//	���l�摜�\����
	struct NumberImageInfo
	{
		int				scale;
		POINT			pos;
		ImageObj		hundred;			//	�O����
		ImageObj		ten;					//	�񌅖�
		ImageObj		one;					//	�ꌅ��
		bool				hundredRenderFlag;	//	�S�̈ʃ����_�[�p�t���O
	};

	//	�������
	struct NumberInfo
	{
		int		hundred;	//�R�C���O����
		int		ten;			//�R�C���񌅖�
		int		one;			//�R�C���ꌅ��
		bool		H_flg;		//�S�̈ʃ����_�[�p�t���O
	};

private:
	iex2DObj*	orgNumber;
	NumberImageInfo	numImageInfo;
	NumberInfo				numInfo;
	bool				secretflag;
	int				number;	//	���l

public:
	//	�������E���
	NumberUI( void );
	~NumberUI( void );
	bool	Initialize( int x, int y, int scale, int num );
	void	Release( void );
	void	Load( void );

	//	�X�V�E�`��
	void	Update( void );
	void	Render( void );

	//	����֐�
	void	SetNumberInfo( NumberInfo& numberInfo, int num );
	void	SetNumberImageInfo( NumberImageInfo& numImageInfo, NumberInfo& numInfo, const int& num );

	//	���ݒ�
	void	SetPos( int x, int y );
	void	SetScale( int scale );
	void	SetColor( const Vector3& color );
	void	SetNumber( int num );
	void	SetSecretFlag( bool state );
	void	SetImage( void );

	//	���擾
	int	GetPosX( void )const;
	int	GetPosY( void )const;
};
