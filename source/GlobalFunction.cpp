
#include	"iextreme.h"
#include	"GlobalFunction.h"

//*****************************************************************************
//
//	�O���[�o���֐�
//
//*****************************************************************************

//----------------------------------------------------------------------------
//	�ϐ�
//----------------------------------------------------------------------------

#define	PERCENTAGE_MAX	1.0f

//----------------------------------------------------------------------------
//	�V�X�e��
//----------------------------------------------------------------------------

	//	�ϐ��ϊ�
	POINT	GetPoint( int x, int y )
	{
		POINT	p;
		p.x = x;
		p.y = y;
		return p;
	}

	DWORD GetColor( float r, float g, float b, float a )
	{
		DWORD	ret;
		ret = ARGB( ( DWORD )( 0xFF * a ), ( DWORD )( 0xFF * r ), ( DWORD )( 0xFF * g ), ( DWORD )( 0xFF * b ) );
		return	ret;
	}

	DWORD GetColor( Vector3 color )
	{
		DWORD	ret;
		ret = ARGB( 0xFF, ( DWORD )( 0xFF * color.x ), ( DWORD )( 0xFF * color.y ), ( DWORD )( 0xFF * color.z ) );
		return ret;
	}

	DWORD GetColor( Vector3 color, float alpha )
	{
		DWORD	ret;
		ret = ARGB( ( DWORD )( 0xFF * alpha ), ( DWORD )( 0xFF * color.x ), ( DWORD )( 0xFF * color.y ), ( DWORD )( 0xFF * color.z ) );
		return ret;
	}

	//	���[���h���W����X�N���[�����W�ɕϊ�
	bool	WorldToClient( const Vector3& pos, Vector3& out, const Matrix& mat )
	{
		//	���_�~�s��Ŏˉe����
		Vector3	temp;
		temp.x = pos.x *mat._11 + pos.y * mat._21 + pos.z * mat._31 + 1.0f * mat._41;
		temp.y = pos.x *mat._12 + pos.y * mat._22 + pos.z * mat._32 + 1.0f * mat._42;
		temp.z = pos.x *mat._13 + pos.y * mat._23 + pos.z * mat._33 + 1.0f * mat._43;
		float w = pos.x *mat._14 + pos.y * mat._24 + pos.z * mat._34 + 1.0f * mat._44;
		temp.x /= w;
		temp.y /= w;
		temp.z /= w;

		//	��ʍ��W�Ɋ��Z(�r���[�|�[�g�ϊ�)
		long WinWide, WinHeight;
		WinWide = iexSystem::ScreenWidth;
		WinWide /= 2;
		WinHeight = iexSystem::ScreenHeight;
		WinHeight /= 2;

		out.x = ( temp.x *WinWide + WinWide );
		out.y = ( -temp.y*WinHeight + WinHeight );
		out.z = temp.z;

		if ( temp.x >  1.0f ) return false;
		if ( temp.x < -1.0f ) return false;
		if ( temp.y >  1.0f ) return false;
		if ( temp.y < -1.0f ) return false;
		if ( temp.z <  0 )	return false;
		return true;
	}

	//	�N���C�A���g���W���烏�[���h���W��
	void	ClientToWorld( const Vector3& screenPos, Vector3& out )
	{
		D3DXVECTOR3	d3dxScreenCoord( screenPos.x, screenPos.y, screenPos.z );

		D3DVIEWPORT9	viewPort;
		iexSystem::Device->GetViewport( &viewPort );

		D3DXMATRIX	worldMatrix;
		D3DXMatrixIdentity( &worldMatrix );
		D3DXVECTOR3	result;
		D3DXVec3Unproject( &result, &d3dxScreenCoord, &viewPort, &matProjection, &matView, &worldMatrix );

		out = Vector3( result.x, result.y, result.z );
	}

	//	���[�V�����ԍ��o�^
	void	SetMotionNum( int& motionData, int motionNum )
	{
		motionData = motionNum;
	}

	//	�f�o�b�O����
	void	DrawString( LPSTR string, int x, int y, DWORD color )
	{
		IEX_DrawText( string, x, y, 300, 300, color );
	}
	
	void	DrawString( LPSTR string, int x, int y, float r, float g, float b )
	{
		IEX_DrawText(string, x, y, 300, 300, ARGB(0xFF, ( DWORD )( 0xFF * r ), ( DWORD )( 0xFF * g ), ( DWORD )( 0xFF * b ) ) );
	}
	
	void	DrawString( LPSTR string, int x, int y, Vector3 color )
	{
		IEX_DrawText( string, x, y, 300, 300, ARGB( 0xFF, ( DWORD )( 0xFF * color.x ), ( DWORD )( 0xFF * color.y ), ( DWORD )( 0xFF * color.z ) ) );
	}

//----------------------------------------------------------------------------
//	�摜�֘A
//----------------------------------------------------------------------------

	//	������
	void	ImageInitialize(ImageObj& image, int x, int y, int w, int h, int sx, int sy, int sw, int sh)
	{
		image.x = x;
		image.y = y;
		image.w = w;
		image.h = h;
		image.sx = sx;
		image.sy = sy;
		image.sw = sw;
		image.sh = sh;
		image.plusScaleX = 0;
		image.plusScaleY = 0;
		image.t = 0.0f;
		image.alpha = 1.0f;
		image.angle = 0.0f;
		image.color = Vector3(1.0f, 1.0f, 1.0f);
		image.p = GetPoint(image.x, image.y);
		image.renderflag = true;

		//	wave�p�p�����[�^
		image.waveAlpha = 1.0f;
		image.waverenderflag = false;
		image.wavespeed = 0.0f;
		image.waveState = false;

		//	flashing�p�p�����[�^
		image.flashingSpeed = 0.0f;
		image.flashingAlpha = 0.0f;
		image.flashingParam = 0.0f;
		image.flashingRenderflag = true;

		//	scaling�p�p�����[�^
		image.scalingspeed = 0.0f;
		image.scalingAlpha = 0.0f;
		image.scalingState = true;
		image.scalingFlag = false;
		image.scalingrenderflag = true;
	}

	//	�摜(��scalingrenderflag;�u�̓C���[�W�̒l)
	void	RenderImage( ImageObj image, int sx, int sy, int sw, int sh, int mode )
	{
		int		width = image.w;
		int		height = image.h;
		int		posx = image.x - width / 2;
		int		posy = image.y - height / 2;

		switch ( mode )
		{
		case IMAGE_MODE::NORMAL:
			if ( image.renderflag )
			image.obj->Render( posx, posy, width, height, sx, sy, sw, sh );
			break;

		case IMAGE_MODE::ADOPTPARAM:
			if ( image.renderflag )
				image.obj->Render( posx, posy, width, height, sx, sy, sw, sh, image.p, image.angle, RS_COPY, GetColor( image.color, image.alpha ) );
			break;

		case IMAGE_MODE::ADD:
			if (image.renderflag)
				image.obj->Render(posx, posy, width, height, sx, sy, sw, sh, image.p, image.angle, RS_ADD, GetColor(image.color, image.alpha));
			break;

		case IMAGE_MODE::WAVE:
			width = image.w + image.plusScaleX;
			height = image.h + image.plusScaleY;
			posx = image.x - width / 2;
			posy = image.y - height / 2;

			if ( image.waverenderflag )
			image.obj->Render( posx, posy, width, height, sx, sy, sw, sh, image.p, image.angle, RS_COPY, GetColor( image.color, image.waveAlpha ) );
			break;

		case IMAGE_MODE::FLASH:
			if ( image.flashingRenderflag )
				image.obj->Render( posx, posy, width, height, sx, sy, sw, sh, RS_COPY, GetColor( image.color, image.flashingAlpha ) );
			break;

		case IMAGE_MODE::SCALING:
			width = image.w + image.plusScaleX;
			height = image.h + image.plusScaleY;
			posx = image.x - width / 2;
			posy = image.y - height / 2;

			if ( image.scalingrenderflag )
				image.obj->Render( posx, posy, width, height, sx, sy, sw, sh, image.p, image.angle, RS_COPY, GetColor( image.color, image.alpha ) );
			break;
		}
	}

	//	�摜(�ʒu���C���[�W�Ƃ͈Ⴄ�����̒l��^����)
	void	RenderImage(ImageObj image, int sx, int sy, int sw, int sh, int mode, int x, int y)
	{
		int		width = image.w;
		int		height = image.h;
		int		posx = x - width / 2;
		int		posy = y - height / 2;

		switch (mode)
		{
		case IMAGE_MODE::NORMAL:
			if (image.renderflag)
				image.obj->Render(posx, posy, width, height, sx, sy, sw, sh);
			break;

		case IMAGE_MODE::ADOPTPARAM:
			if (image.renderflag)
				image.obj->Render(posx, posy, width, height, sx, sy, sw, sh, image.p, image.angle, RS_COPY, GetColor(image.color, image.alpha));
			break;

		case IMAGE_MODE::ADD:
			if (image.renderflag)
				image.obj->Render(posx, posy, width, height, sx, sy, sw, sh, image.p, image.angle, RS_ADD, GetColor(image.color, image.alpha));
			break;

		case IMAGE_MODE::WAVE:
			width = image.w + image.plusScaleX;
			height = image.h + image.plusScaleY;
			posx = x - width / 2;
			posy = y - height / 2;

			if (image.waverenderflag)
				image.obj->Render(posx, posy, width, height, sx, sy, sw, sh, RS_COPY, GetColor(image.color, image.waveAlpha));
			break;

		case IMAGE_MODE::FLASH:
			if (image.flashingRenderflag)
				image.obj->Render(posx, posy, width, height, sx, sy, sw, sh, RS_COPY, GetColor(image.color, image.flashingAlpha));
			break;

		case IMAGE_MODE::SCALING:
			width = image.w + image.plusScaleX;
			height = image.h + image.plusScaleY;
			posx = x - width / 2;
			posy = y - height / 2;

			if ( image.scalingrenderflag )
				image.obj->Render( posx, posy, width, height, sx, sy, sw, sh, image.p, image.angle, RS_COPY, GetColor( image.color, image.alpha ) );
			break;
		}
	}

	//	�g��ݒ�
	void	SetWave( ImageObj& image, float speed )
	{
		image.plusScaleX = 0;
		image.plusScaleY = 0;
		image.t = 0;
		image.waveAlpha = 1.0f;
		image.waveState = true;
		image.wavespeed = speed;
		image.waverenderflag = true;
	}

	//	�g��X�V
	bool	WaveUpdate( ImageObj& image, int max_scale, float max_alpha )
	{
		if ( image.waveState )
		{
			//	�p�����[�^���Z
			image.t += D3DX_PI / 180 * image.wavespeed;

			//	�p�����[�^����ݒ�
			if ( image.t >= 1.0f )
			{
				image.t = 1.0f;
				image.waveState = false;
				image.waverenderflag = false;
			}

			Lerp( image.waveAlpha, max_alpha, 0.0f, image.t );
			Lerp( image.plusScaleX, 0, max_scale, image.t );
			Lerp( image.plusScaleY, 0, max_scale, image.t );

		}

		if ( image.t >= 1.0f )		return	true;
		return	false;
	}

	//	�g��k���I��
	void	StopScaling( ImageObj& image )
	{
		image.plusScaleX = 0;
		image.plusScaleY = 0;
		image.t = 0;
		image.alpha = 1.0f;
		image.scalingState = true;
		image.scalingspeed = 0.0f;
		image.scalingFlag = false;
		image.scalingrenderflag = true;
	}

	//	�g��k���ݒ�
	void	SetScaling(ImageObj& image, float speed, bool state)
	{
		image.plusScaleX = 0;
		image.plusScaleY = 0;
		image.t = 0;
		if (state)	image.alpha = 1.0f;
		if (!state)	image.alpha = 0.0f;
		image.scalingAlphaFlag = state;
		image.scalingState = true;
		image.scalingspeed = speed;
		image.scalingFlag = true;
		image.scalingrenderflag = true;
	}

	//	�g��k���X�V
	void	ScalingUpdate(ImageObj& image, int max_scale)
	{
		if (!image.scalingFlag) return;

		//	�p�����[�^���Z
		image.t += D3DX_PI / 180 * image.scalingspeed;
	
		//-------------------------
		//	�g��
		//-------------------------
		if (image.scalingState)
		{
			//	�p�����[�^����ݒ�
			if (image.t >= 1.0f)
			{
				image.t = 1.0f;
				image.scalingState = false;
			}

			Lerp(image.plusScaleX, 0, max_scale, image.t);
			Lerp(image.plusScaleY, 0, max_scale, image.t);

		}

		//-------------------------
		//	�k��
		//-------------------------
		else
		{
			//	�p�����[�^����ݒ�
			if (image.t >= 1.0f)
			{
				image.t = 1.0f;
				image.scalingState = true;
			}

			Lerp(image.plusScaleX, max_scale, 0, image.t);
			Lerp(image.plusScaleY, max_scale, 0, image.t);

		}

		if (image.t >= 1.0f)		image.t = 0.0f;
	}

	//	�g��k���X�V
	bool	ScalingAlphaUpdate(ImageObj& image, int max_scale)
	{
		if (!image.scalingFlag) return false;

		switch ( image.scalingAlphaFlag )
		{
		case true:
			//	�p�����[�^���Z
			image.t += D3DX_PI / 180 * image.scalingspeed;
			image.alpha -= D3DX_PI / 180 * image.scalingspeed;

			//-------------------------
			//	�g��
			//-------------------------
			//	�p�����[�^����ݒ�
			if (image.t >= 1.0f)
			{
				image.t = 0.0f;
				image.alpha = 0.0f;
				image.scalingFlag = false;
				return true;
			}

			Lerp(image.plusScaleX, 0, max_scale, image.t);
			Lerp(image.plusScaleY, 0, max_scale, image.t);

			if (image.alpha <= 0.0f) image.alpha = 0.0f;
			return false;

		case false:
			//	�p�����[�^���Z
			image.t += D3DX_PI / 180 * image.scalingspeed;
			image.alpha += D3DX_PI / 180 * image.scalingspeed;

			//-------------------------
			//	�k��
			//-------------------------
			//	�p�����[�^����ݒ�
			if (image.t >= 1.0f)
			{
				image.t = 0.0f;
				image.alpha = 1.0f;
				image.scalingFlag = false;
				return true;
			}

			Lerp(image.plusScaleX, max_scale, 0, image.t);
			Lerp(image.plusScaleY, max_scale, 0, image.t);

			if (image.alpha >= 1.0f) image.alpha = 1.0f;
			return false;
			break;
		}
		return false;
	}

	//	���g��c�k���X�V
	void	ScalingLandingUpdate( ImageObj& image, int max_scale )
	{
		if (!image.scalingFlag) return;

		//	�p�����[�^���Z
		image.t += D3DX_PI / 180 * image.scalingspeed;
	
		//-------------------------
		//	�g��
		//-------------------------
		if (image.scalingState)
		{
			//	�p�����[�^����ݒ�
			if (image.t >= 1.0f)
			{
				image.t = 1.0f;
				image.scalingState = false;
			}

			Lerp(image.plusScaleX, 0, max_scale, image.t);
			Lerp(image.plusScaleY, 0, -max_scale, image.t);

		}

		//-------------------------
		//	�k��
		//-------------------------
		else
		{
			//	�p�����[�^����ݒ�
			if (image.t >= 1.0f)
			{
				image.t = 1.0f;
				image.scalingState = true;
				image.scalingFlag = false;
			}

			Lerp(image.plusScaleX, max_scale, 0, image.t);
			Lerp(image.plusScaleY, -max_scale, 0, image.t);

		}

		if (image.t >= 1.0f)		image.t = 0.0f;
	}


	//	�_�ŏ���
	void	FlashingUpdate( ImageObj& image, float speed )
	{
		if ( speed > 0.0f )
			image.flashingParam += speed;
		else
			image.flashingParam += image.flashingSpeed;

		if ( image.flashingParam >= D3DX_PI * 2.0f )	image.flashingParam = 0.0f;

		//	�����x�X�V
		image.flashingAlpha = 0.5f + 0.5f * sinf( image.flashingParam );
	}

//----------------------------------------------------------------------------
//	�x�W�F�Ȑ�( �o�́A�n�_�A����_�A�I�_�A���݂̊���( 0.0f ~ 1.0f ) )
//----------------------------------------------------------------------------

	//	�Q���x�W�F�Ȑ�
	bool	BezierCurve( Vector3& out, Vector3 start, Vector3 p1, Vector3 end, float t )
	{
		if ( t >= 1.0f )	return true;

		out.x = ( ( 1 - t ) * ( 1 - t ) ) * start.x + ( ( 2 * ( 1 - t ) * t * p1.x ) + ( ( t * t ) * end.x ) );
		out.y = ( ( 1 - t ) * ( 1 - t ) ) * start.y + ( ( 2 * ( 1 - t ) * t * p1.y ) + ( ( t * t ) * end.y ) );
		out.z = ( ( 1 - t ) * ( 1 - t ) ) * start.z + ( ( 2 * ( 1 - t ) * t * p1.z ) + ( ( t * t ) * end.z ) );

		return	false;
	}
	
	//	�R���x�W�F�Ȑ�
	bool	BezierCurve( Vector3& out, Vector3 start, Vector3 p1, Vector3 p2, Vector3 end, float t )
	{
		if ( t >= 1.0f )	return true;

		out.x = ( ( 1 - t ) * ( 1 - t ) * ( 1 - t ) ) * start.x + ( 3 * ( ( 1 - t ) * ( 1 - t ) ) * t * p1.x ) + ( 3 * ( 1 - t ) * ( t * t ) * p2.x ) + ( t * t * t ) * end.x;
		out.y = ( ( 1 - t ) * ( 1 - t ) * ( 1 - t ) ) * start.y + ( 3 * ( ( 1 - t ) * ( 1 - t ) ) * t * p1.y ) + ( 3 * ( 1 - t ) * ( t * t ) * p2.y ) + ( t * t * t ) * end.y;
		out.z = ( ( 1 - t ) * ( 1 - t ) * ( 1 - t ) ) * start.z + ( 3 * ( ( 1 - t ) * ( 1 - t ) ) * t * p1.z ) + ( 3 * ( 1 - t ) * ( t * t ) * p2.z ) + ( t * t * t ) * end.z;

		return	false;
	}

//----------------------------------------------------------------------------
//	�������x�W�F�Ȑ��p�����[�^�擾( 0.0f ~ 1.0f )
//----------------------------------------------------------------------------

	namespace
	{
		const static int nPasTgl[12][12] = {
			{ 1 },
			{ 1, 1 },
			{ 1, 2, 1 },
			{ 1, 3, 3, 1 },
			{ 1, 4, 6, 4, 1 },
			{ 1, 5, 10, 10, 5, 1 },
			{ 1, 6, 15, 20, 15, 6, 1 },
			{ 1, 7, 21, 35, 35, 21, 7, 1 },
			{ 1, 8, 28, 56, 70, 56, 28, 8, 1 },
			{ 1, 9, 36, 84, 126, 126, 84, 36, 9, 1 },
			{ 1, 10, 45, 120, 210, 252, 210, 120, 45, 10, 1 },
			{ 1, 11, 55, 165, 330, 464, 464, 330, 165, 55, 11, 1 }
		};
	}

	inline float Get( const float &y1, const float &y2, const float &t, const int &n ){
		float b = t > 1 ? 1 : (t < 0 ? 0 : t);
		float a = 1 - b;
		float ay = 0;
		float y[4] = { 0, y1, y2, 1 };
		int m;
		for (int i = 0; i <= n; i++){
			m = i == 0 ? 0 : (i == n ? 3 : (i <= n / 2 ? 1 : 2));//y�̓Y��������
			ay += nPasTgl[n][i] * pow(a, n - i)*pow(b, i)*y[m];//��1
		}
		return ay;
	}

	//	�x�W�F�Ȑ��p�����[�^�擾�i 0.0f ~ 1.0f �ŕԂ� �j�@
	float GetBezier( int ePrm1, int ePrm2, float fRate ){
		int n = 3;                //n�����w��
		float y1, y2;
		switch (ePrm1){
		case ePrm_t::eSlow_Lv5: y1 = 0;						n = 11; break;//11����
		case ePrm_t::eSlow_Lv4: y1 = 0;                        n = 9; break;//9����
		case ePrm_t::eSlow_Lv3: y1 = 0;                        n = 7; break;//7����
		case ePrm_t::eSlow_Lv2: y1 = 0;                        n = 5; break;//5����
		case ePrm_t::eSlow_Lv1: y1 = 0;                        n = 3; break;//3����
		case ePrm_t::eNoAccel: y1 = 0.333333f;				n = 3; break;//�����̏ꍇ��3������1/3�̓_
		case ePrm_t::eRapid_Lv1: y1 = 1;						n = 3; break;//3����
		case ePrm_t::eRapid_Lv2: y1 = 1;						n = 5; break;//5����
		case ePrm_t::eRapid_Lv3: y1 = 1;						n = 7; break;//7����
		case ePrm_t::eRapid_Lv4: y1 = 1;						n = 9; break;//9����
		case ePrm_t::eRapid_Lv5: y1 = 1;						n = 11; break;//11����
		}
		switch (ePrm2){
		case ePrm_t::eSlow_Lv5: y2 = 1;                       n = 11; break;//11����
		case ePrm_t::eSlow_Lv4: y2 = 1;                       n = 9; break;//9����
		case ePrm_t::eSlow_Lv3: y2 = 1;                       n = 7; break;//7����
		case ePrm_t::eSlow_Lv2: y2 = 1;                       n = 5; break;//5����
		case ePrm_t::eSlow_Lv1: y2 = 1;                       n = 3; break;//3����
		case ePrm_t::eNoAccel: y2 = 0.6666667f;              n = 3; break;//�����̏ꍇ��3������2/3�̓_
		case ePrm_t::eRapid_Lv1: y2 = 0;                       n = 3; break;//3����
		case ePrm_t::eRapid_Lv2: y2 = 0;                       n = 5; break;//5����
		case ePrm_t::eRapid_Lv3: y2 = 0;                       n = 7; break;//7����
		case ePrm_t::eRapid_Lv4: y2 = 0;                       n = 9; break;//9����
		case ePrm_t::eRapid_Lv5: y2 = 0;                       n = 11; break;//11����
		}

		float	out = Get( y1, y2, fRate, n );
		return out;
	}

//----------------------------------------------------------------------
//	��Ԋ֐��p�p�����[�^�X�V
//----------------------------------------------------------------------

	//	�����X�V( �X�V�������p�����[�^, ���Z�l ) : �X�V�������p�����[�^��1.0f�ȏ�̏ꍇtrue��������
	bool	PercentageUpdate( float& param, float addParam )
	{
		//	�p�����[�^���Z
		param += addParam;

		if ( param >= PERCENTAGE_MAX )
		{
			param = PERCENTAGE_MAX;
			return	true;
		}

		return	false;
	}

//-------------------------------------------------------------------------
//	�}�`�`��
//-------------------------------------------------------------------------

	//	���`��
	void	DrawSphere( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p0, FLOAT r, D3DCOLOR c )
	{
		CONST INT N = 36;

		static _VB vb( d3dd, sizeof( D3DXVECTOR3 ) * ( N + 1 ) );
		for ( int n = 0; n < 3; n++ )
		{
			D3DXVECTOR3 *p;
			vb->Lock( 0, 0, ( void ** )&p, 0 );
			for ( int i = 0; i < N; i++ )
			{
				switch (  n )
				{
				case 0: p[i] = r * D3DXVECTOR3( sinf( 360.0f / N * 0.01745f * i ), cosf( 360.0f / N * 0.01745f * i ), 0 ) + p0; break;
				case 1: p[i] = r * D3DXVECTOR3( sinf( 360.0f / N * 0.01745f * i ), 0, cosf( 360.0f / N * 0.01745f * i ) ) + p0; break;
				case 2: p[i] = r * D3DXVECTOR3( 0, sinf( 360.0f / N * 0.01745f * i ), cosf( 360.0f / N * 0.01745f * i ) ) + p0; break;
				}
			}
			p[N] = p[0];
			vb->Unlock();

			D3DMATERIAL9 m = { 0 };
			m.Diffuse = m.Ambient = D3DXCOLOR( c );
			d3dd->SetMaterial( &m );

			d3dd->SetFVF( D3DFVF_XYZ );
			d3dd->SetStreamSource( 0, vb, 0, sizeof( D3DXVECTOR3 ) );
			d3dd->SetTransform( D3DTS_WORLD, &D3DXMATRIX( 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1 ) );
			d3dd->DrawPrimitive( D3DPT_LINESTRIP, 0, N );
		}
	}

	void	DrawSphere( const Vector3& pos, float r, DWORD color )
	{
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		DrawSphere( iexSystem::GetDevice(), D3DXVECTOR3( pos.x, pos.y, pos.z ), r, color );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	}

	//	�J�v�Z���`��
	void	DrawCapsule( LPDIRECT3DDEVICE9 d3dd, CONST D3DXVECTOR3 &p0, CONST D3DXVECTOR3 &p1, FLOAT r, D3DCOLOR c )
	{
		CONST INT N = 36;
		CONST INT L0 = 0;
		CONST INT L1 = N + 1;
		CONST INT L2 = L1 + N + 1;

		CONST INT L3 = L2 + N * 2;
		CONST INT L4 = L3 + N + 1;
		CONST INT L5 = L4 + N + 1;
		CONST INT L6 = L5 + N + 1;

		static _VB vb(d3dd, sizeof(D3DXVECTOR3)* ((N + 1) * 2 + N * 2 + (N + 1) * 4));
		D3DXVECTOR3 *p;
		vb->Lock(0, 0, (void **)&p, 0);
		D3DXVECTOR3 up = p0 - p1;
		CONST FLOAT L = D3DXVec3Length(&up);
		for (int i = 0; i < N; i++)
		{
			p[L0 + i] = D3DXVECTOR3(r * sinf(360.0f / N * 0.01745f * i), L / 2.0f, r * cosf(360.0f / N * 0.01745f * i));
			p[L1 + i] = D3DXVECTOR3(r * sinf(360.0f / N * 0.01745f * i), -L / 2.0f, r * cosf(360.0f / N * 0.01745f * i));
			if (i % (N / 4) == 0)
			{
				p[L2 + 2 * i] = p[L0 + i];
				p[L2 + 2 * i + 1] = p[L1 + i];
			}
			p[L3 + i] = D3DXVECTOR3(r * cosf(180.0f / N * 0.01745f * i), r * sinf(180.0f / N * 0.01745f * i) + L / 2.0f, 0);
			p[L4 + i] = D3DXVECTOR3(0, r * sinf(180.0f / N * 0.01745f * i) + L / 2.0f, r * cosf(180.0f / N * 0.01745f * i));
			p[L5 + i] = D3DXVECTOR3(r * cosf(180.0f / N * 0.01745f * i), -r * sinf(180.0f / N * 0.01745f * i) - L / 2.0f, 0);
			p[L6 + i] = D3DXVECTOR3(0, -r * sinf(180.0f / N * 0.01745f * i) - L / 2.0f, r * cosf(180.0f / N * 0.01745f * i));
		}
		p[L0 + N] = p[L0];
		p[L1 + N] = p[L1];
		p[L3 + N] = D3DXVECTOR3(-r, L / 2.0f, 0);
		p[L4 + N] = D3DXVECTOR3(0, L / 2.0f, -r);
		p[L5 + N] = D3DXVECTOR3(-r, L / 2.0f, 0);
		p[L6 + N] = D3DXVECTOR3(0, L / 2.0f, -r);

		vb->Unlock();

		D3DXVECTOR3 axis;
		D3DXVec3Cross(&axis, &D3DXVECTOR3(0, 1, 0), &up);
		D3DXVec3Normalize(&axis, &axis);
		FLOAT angle = acosf(D3DXVec3Dot(&up, &D3DXVECTOR3(0, 1, 0)) / D3DXVec3Length(&up));
		D3DXMATRIX M;
		D3DXMatrixRotationAxis(&M, &axis, angle);
		D3DXVECTOR3 g = (p0 + p1) / 2;
		M._41 = g.x;
		M._42 = g.y;
		M._43 = g.z;

		D3DMATERIAL9 m = { 0 };
		m.Diffuse = m.Ambient = D3DXCOLOR(c);
		d3dd->SetMaterial(&m);

		d3dd->SetFVF(D3DFVF_XYZ);
		d3dd->SetStreamSource(0, vb, 0, sizeof(D3DXVECTOR3));
		d3dd->SetTransform(D3DTS_WORLD, &M);
		d3dd->DrawPrimitive(D3DPT_LINESTRIP, L0, N);
		d3dd->DrawPrimitive(D3DPT_LINESTRIP, L1, N);
		d3dd->DrawPrimitive(D3DPT_LINELIST, L2, N);
		d3dd->DrawPrimitive(D3DPT_LINESTRIP, L3, N);
		d3dd->DrawPrimitive(D3DPT_LINESTRIP, L4, N);
		d3dd->DrawPrimitive(D3DPT_LINESTRIP, L5, N);
		d3dd->DrawPrimitive(D3DPT_LINESTRIP, L6, N);
	}

	void	DrawCapsule( const Vector3& p1, const Vector3& p2, float r, DWORD color )
	{
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_FALSE );
		DrawCapsule( iexSystem::GetDevice(), D3DXVECTOR3( p1.x, p1.y, p1.z ), D3DXVECTOR3( p2.x, p2.y, p2.z ), r, color );
		iexSystem::GetDevice()->SetRenderState( D3DRS_ZENABLE, D3DZB_TRUE );
	}

//----------------------------------------------------------------------------
//	�}�`�ݒ�
//----------------------------------------------------------------------------

	//	���_�ݒ�
	void	SetVertex( LVERTEX& v, float x, float y, float z, float tu, float tv, DWORD color )
	{
		v.x = x;
		v.y = y;
		v.z = z;
		v.tu = tu;
		v.tv = tv;
		v.color = color;
	}

	//	���_������
	void	SetVertex( TLVERTEX& v, float x, float y, float z, float tu, float tv, DWORD color )
	{
		v.sx = x;
		v.sy = y;
		v.sz = z;
		v.tu = tu;
		v.tv = tv;
		v.color = color;
		v.rhw = 1.0f;
	}

//----------------------------------------------------------------------
//	������(�o�́A�J�n�_�A�ڕW�n�_�A�����A�����镉��(�d��))
//----------------------------------------------------------------------
	bool Parabola(Vector3& out, Vector3 start, Vector3 end, float speed, float mass)
	{
		float angleTan;
		float elevation;	//�p
		float height = start.y - end.y;	//���፷
		float tx = end.x - start.x;	//�@X���W��
		float tz = end.z - start.z;	//�@Z���W��

		float txz = sqrtf(tx*tx + tz*tz);

		// y = v0 * sin��*t + 0.5f*G*t*t
		// x = v0 * cos��*t
		// 2�������� a*T*T + b*T + c = 0
		float A = (mass*txz*txz) / (2.0f*speed*speed);

		// T�̌W��(aT*T + b*T + c = 0)	
		float a, b, c;
		a = 1.0f;
		if (A != 0){
			b = txz / A;
			c = 1.0f + height / A;
		}
		// ���̌���(������)
		float D = b * b - 4 * a * c;
		if (D < 0)
		{
			out = Vector3(0, 0, 0);
			angleTan = 0;
			return false;
		}

		// ���̌���
		elevation = (-b + sqrtf(D)) / (2.0f*a);

		// �p
		angleTan = atanf(elevation);

		// �ړ�����
		D3DXVECTOR3 tt;
		tt.x = end.x - start.x;
		tt.y = 0;
		tt.z = end.z - start.z;
		D3DXVec3Normalize(&tt, &tt);

		out.x = speed * cosf(angleTan)*tt.x;
		out.y = speed * sinf(angleTan);
		out.z = speed * cosf(angleTan)*tt.z;

		return true;
	}

//-------------------------------------------------------------------------
//	���ݕϊ�DirectX<->IEX
//-------------------------------------------------------------------------

	void	QuaternionDtoI( Quaternion *pOut, D3DXQUATERNION *pQ )
	{
		pOut->x = pQ->x;
		pOut->y = pQ->y;
		pOut->z = pQ->z;
		pOut->w = pQ->w;
	}

	void	QuaternionItoD( D3DXQUATERNION  *pOut, Quaternion *pQ )
	{
		pOut->x = pQ->x;
		pOut->y = pQ->y;
		pOut->z = pQ->z;
		pOut->w = pQ->w;
	}

	void	Vector3DtoI( Vector3 *pOut, D3DXVECTOR3 *pV )
	{
		pOut->x = pV->x;
		pOut->y = pV->y;
		pOut->z = pV->z;
	}

	void	Vector3ItoD( D3DXVECTOR3 *pOut, Vector3 *pV )
	{
		pOut->x = pV->x;
		pOut->y = pV->y;
		pOut->z = pV->z;
	}

//-------------------------------------------------------------------------
//	���Z�n
//-------------------------------------------------------------------------
	
	//�@( min <= value && value < max )
	bool isMiddle(const int& value, const int& min, const int& max)
	{
		return	(min <= value && value < max);
	}
	bool isMiddle(const float& value, const float& min, const float& max)
	{
		return	(min <= value && value < max);
	}