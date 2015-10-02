
#ifndef		__BULLET_MANAGER_H__
#define		__BULLET_MANAGER_H__

//******************************************************************************
//
//	BulletManager�N���X
//
//******************************************************************************
class BulletManager
{
private:

private:


	//	�I�u�W�F�N�g
	iexMesh*	org;			//	�I���W�i�����f��
	Bullet*		c_Bullet;

	//	�ϐ�
	int				bullet_num;	//	�e��
	int				limitTimer;

public:
	//	�������E���
	BulletManager(void);
	~BulletManager(void);
	bool	Initialize(void);

	//	�X�V�E�`��
	void	Update(void);
	void	Render(void);
	void	Render(iexShader* shader, LPSTR technique);

	//	����֐�
	void	Set( Vector3 pos, const Vector3 vec, float speed);
	void	Set(Vector3 pos, const Vector3 vec, const float& scale, float speed);
	void	Set(Vector3 pos, const Vector3 vec, float speed, int leanpower);
	void	DistCheck(int n);
};

extern	BulletManager*	m_BulletManager;

//******************************************************************************
#endif // !__BULLET_MANAGER_H__