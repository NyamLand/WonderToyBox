
#ifndef __PRINCESS_H__
#define	__PRINCESS_H__

//*********************************************************************************
//
//	Princess�N���X
//
//*********************************************************************************
class Princess : public		Player
{
private:


public:
	//	�������E���
	Princess( void );
	~Princess( void );
	void	SetMotionData( void );
	
	//	�X�V�E�`��
	void	Render( iexShader* shader, LPSTR technique );

	//	����֐�
	bool	QuickArts( void );
	bool	PowerArts( void );
	bool	HyperArts( void );

	//	���ݒ�
	void	SetAttackParam( int attackKind );
};

//*********************************************************************************
#endif // !__PRINCESS_H__
