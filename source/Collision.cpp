
#include	"iextreme.h"
#include	"GlobalFunction.h"
#include	"Collision.h"

//****************************************************************************************
//
//	Collision�N���X
//
//****************************************************************************************

//--------------------------------------------------------------------------------------------
//	�O���[�o��
//--------------------------------------------------------------------------------------------
iexMesh*	Collision::obj = NULL;

//--------------------------------------------------------------------------------------------
//	�������E���
//--------------------------------------------------------------------------------------------

	//	�R���X�g���N�^
	Collision::Collision( void )
	{
		
	}
	
	//	�f�X�g���N�^
	Collision::~Collision( void )
	{

	}

	//	������
	bool	Collision::Initiallize( iexMesh* org )
	{
		obj = org;
		obj->Update();
		return	true;
	}

//--------------------------------------------------------------------------------------------
//	�X�e�[�W����
//--------------------------------------------------------------------------------------------

	//	�X�e�[�W�����擾
	float	Collision::GetHeight( const Vector3	pos )
	{
		Vector3	p_pos = Vector3( pos.x, pos.y + 3.0f, pos.z );
		Vector3	vec = Vector3(0.0f, -1.0f, 0.0f);
		Vector3	out;
		float	dist = 50.0f;

		if ( obj->RayPick( &out, &p_pos, &vec, &dist ) == -1 )
			return	pos.y;
		else
			return	out.y;

	}

	//�@�X�e�[�W�����擾�i���苗���w��j
	float	Collision::GetHeight( const Vector3	pos, float dist )
	{
		Vector3	p_pos = Vector3( pos.x, pos.y + 3.0f, pos.z );
		Vector3	vec = Vector3( 0.0f, -1.0f, 0.0f );
		Vector3	out;

		//	�I�u�W�F�N�g�̋t�s����Z�o
		obj->Update();
		Matrix	mat = obj->TransMatrix;
		Matrix	invMat;		//	�t�s��
		D3DXMatrixInverse( &invMat, null, &mat );

		//	�t�s��ŗ�����[�J����
		Vector3	invVec;
		invVec.x = invMat._11 * vec.x + invMat._21 * vec.y + invMat._31 * vec.z;
		invVec.y = invMat._12 * vec.x + invMat._22 * vec.y + invMat._32 * vec.z;
		invVec.z = invMat._13 * vec.x + invMat._23 * vec.y + invMat._33 * vec.z;

		Vector3	invPos;
		invPos.x = invMat._11 * p_pos.x + invMat._21 * p_pos.y + invMat._31 * p_pos.z + invMat._41;
		invPos.y = invMat._12 * p_pos.x + invMat._22 * p_pos.y + invMat._32 * p_pos.z + invMat._42;
		invPos.z = invMat._13 * p_pos.x + invMat._23 * p_pos.y + invMat._33 * p_pos.z + invMat._43;
		if ( obj->RayPick( &out, &p_pos, &vec, &dist ) == -1 )
			return	pos.y;
		else
		{
			Vector3	resultPos;
			resultPos.x = mat._11 * out.x + mat._21 * out.y + mat._31 * out.z + mat._41;
			resultPos.x = mat._12 * out.x + mat._22 * out.y + mat._32 * out.z + mat._42;
			resultPos.x = mat._13 * out.x + mat._23 * out.y + mat._33 * out.z + mat._43;
			return	resultPos.y;
		}
	}

	//	�ǂƂ̓����蔻��
	void	Collision::CheckWall( const Vector3 pos, Vector3& p_move )
	{
		const	float	DIST = 2.0f;	//	�ǂƂ̋���
		Vector3	p_pos = Vector3( pos.x, pos.y + 1.0f, pos.z );
		Vector3	vec = Vector3( p_move.x, 0.0f, p_move.z );
		vec.Normalize();
		Vector3	takePos;
		float	dist = 100.0f;

		if ( obj->RayPick( &takePos, &p_pos, &vec, &dist ) != -1 ){
			float	disToWall = Vector3( Vector3( takePos.x, 0.0f, takePos.z ) - Vector3( p_pos.x, 0.0f, p_pos.z ) ).Length();
			if ( disToWall <= DIST ){
				
				//	�ړ���
				float	move = Vector3( p_move.x, 0.0f, p_move.z ).Length();
				
				//	�v���C���[���烌�C�̌����_�ւ̃x�N�g��
				Vector3	vPtoWall( takePos - p_pos );
				vPtoWall.y = 0.0f;	vPtoWall.Normalize();
				vec.y = 0.0f;	vec.Normalize();
				
				//	�@���̏�����i�H�j�����߂�
				Vector3	vCrossUp;
				Vector3Cross( vCrossUp, vec, vPtoWall );
				vCrossUp.Normalize();
				
				//	�@���̏�����i�H�j�Ɩ@���̊O�ς��犊��������v�Z
				Vector3	vCrossSide;
				Vector3Cross( vCrossSide, vCrossUp, vec );
				vCrossSide.Normalize();
				
				//	�@���ƃv���[���[���烌�C�̌����_�ւ̃x�N�g���̓���
				float	dotNP = Vector3Dot( vec, vPtoWall );

				//	�ړ��ʂ̒���
				p_move.x = vCrossSide.x * move * ( dotNP + 1.0f );
				p_move.z = vCrossSide.z * move * ( dotNP + 1.0f );
			}
		}
	}

//--------------------------------------------------------------------------------------------
//	����
//--------------------------------------------------------------------------------------------

	//	�I�u�W�F�N�g�̔���
	bool	Collision::GetReflect( Vector3& pos, Vector3& vec, float rate )
	{
		Vector3 p = pos;
		Vector3 v = vec;
		Vector3 out;
		float dist = 50.0f;
		obj->Update();

		if ( obj->RayPick( &out, &p, &v, &dist ) == -1 )
			return false;
		// �ړ��ʂ̒���
		float vec_len = vec.Length();
		// �ǂ܂ł̃x�N�g��
		Vector3 pos_out_vec = out - pos;
		// �ǂ܂ł̋���
		float pos_out_len = pos_out_vec.Length();
		if ( vec_len > pos_out_len )
		{
			// �@�����K��
			v.Normalize();
			// ���˃x�N�g����@���Ɏˉe
			float dot = Vector3Dot( -vec, v );
			// ���˃x�N�g���Ɣ��˃x�N�g����
			// �����x�N�g������
			// ���˃x�N�g���������B
			vec = v*2.0f*dot - ( -vec );
			// ���˗��̌v�Z
			vec *= rate;
			return true;
		}
		return false;
	}

	//	�I�u�W�F�N�g�̔���
	bool	Collision::GetReflect( iexMesh*	org, Vector3& pos, Vector3& vec, float rate ) 
	{
		// �I�u�W�F�N�g�̋t�s����Z�o
		org->Update();
		Matrix mat = org->TransMatrix;
		Matrix invMat;	// �t�s��
		D3DXMatrixInverse(&invMat, null, &mat);

		// �t�s��Ń��C�����[�J����
		Vector3 invVec;
		invVec.x = invMat._11 * vec.x + invMat._21 * vec.y + invMat._31 * vec.z;
		invVec.y = invMat._12 * vec.x + invMat._22 * vec.y + invMat._32 * vec.z;
		invVec.z = invMat._13 * vec.x + invMat._23 * vec.y + invMat._33 * vec.z;

		Vector3 invPos;
		invPos.x = invMat._11 * pos.x + invMat._21 * pos.y + invMat._31 * pos.z + invMat._41;
		invPos.y = invMat._12 * pos.x + invMat._22 * pos.y + invMat._32 * pos.z + invMat._42;
		invPos.z = invMat._13 * pos.x + invMat._23 * pos.y + invMat._33 * pos.z + invMat._43;

		Vector3 v = invVec;
		Vector3 p = invPos;
		Vector3 out;
		float d = 100.0f;
		if (org->RayPick(&out, &p, &v, &d) >= 0)
		{
			Vector3 vv = out - p;
			float dd = vv.Length();
			float dm = invVec.Length();
			if (dd < dm){
				v.Normalize();		//	�@���Z�o			
				float dot = Vector3Dot(-invVec, v);	//�@�@�������Ɏˉe
				invVec = v*dot*2.0f - (-invVec);


				Vector3 p;
				p.x = mat._11 * out.x + mat._21 * out.y + mat._31 * out.z + mat._41;
				p.y = mat._12 * out.x + mat._22 * out.y + mat._32 * out.z + mat._42;
				p.z = mat._13 * out.x + mat._23 * out.y + mat._33 * out.z + mat._43;
				pos = p;

				Vector3 v;
				v.x = mat._11 * invVec.x + mat._21 * invVec.y + mat._31 * invVec.z;
				v.y = mat._12 * invVec.x + mat._22 * invVec.y + mat._32 * invVec.z;
				v.z = mat._13 * invVec.x + mat._23 * invVec.y + mat._33 * invVec.z;
				vec = v*rate;

				return true;
			}
		}
		return false;
	}

//--------------------------------------------------------------------------------------------
//	�����蔻��
//--------------------------------------------------------------------------------------------
	
	//	��������
	bool	Collision::DistCheck( const Vector3 p1, const Vector3 p2, float dist )
	{
		Vector3 len = p1 - p2;
		float length = len.Length();

		if ( length < dist )	return true;
		return	false;
	}

	// �_�Ɛ����̋����̕�����Ԃ�(�J�v�Z���p)
	float	Collision::DistancePointSegmentSq( const Vector3& p1, const Vector3& l1p1, const Vector3& l1p2 )
	{
		Vector3 v1 = p1 - l1p1;
		Vector3 v2 = l1p2 - l1p1;
		float v1v2 = Vector3Dot( v1, v2 );
		if ( v1v2 <= 0.0f ) return v1.LengthSq();
		float v2v2 = v2.LengthSq();
		if ( v2v2 <= v1v2 ) {
			Vector3 v3 = p1 - l1p2;
			return v3.LengthSq();
		}
		return v1.LengthSq() - v1v2 * v1v2 / v2v2;
	}

	// �����Ɛ����̋����̕�����Ԃ�(�J�v�Z���p)
	float	Collision::DistanceSegmentSegmentSq( const Vector3& l1p1, const Vector3& l1p2, const Vector3& l2p1, const Vector3& l2p2 )
	{
		// �˂���̈ʒu�̔���
		Vector3 v1 = l1p2 - l1p1;
		Vector3 v2 = l2p2 - l2p1;
		Vector3 n;
		Vector3Cross( n, v1, v2 );
		float nn = n.LengthSq();
		if ( nn ) {
			// ���s�ł͂Ȃ�
			Vector3 v12 = l2p1 - l1p1;
			float nv12 = Vector3Dot( n, v12 );
			Vector3 vd = n * ( nv12 / nn );
			Vector3 q1 = l2p1 - vd;
			Vector3 q2 = l2p2 - vd;
			Vector3 p1q1 = q1 - l1p1;
			Vector3 p1q2 = q2 - l1p1;
			Vector3 r1, r2;
			Vector3Cross( r1, v1, p1q1 );
			Vector3Cross( r2, v1, p1q2 );
			if ( Vector3Dot( r1, r2 ) < 0 ) {
				Vector3 v3 = q2 - q1;
				Vector3 q1p1 = l1p1 - q1;
				Vector3 q1p2 = l1p2 - q1;
				Vector3Cross( r1, v3, q1p1 );
				Vector3Cross( r2, v3, q1p2 );
				if (Vector3Dot( r1, r2 ) < 0 ) {
					// �˂���̈ʒu
					return nv12 * nv12 / nn;
				}
			}
		}
		// �˂��ꂶ��Ȃ��ʒu
		return min(
			min( DistancePointSegmentSq( l1p1, l2p1, l2p2 ),
			DistancePointSegmentSq( l1p2, l2p1, l2p2 ) ),
			min(DistancePointSegmentSq( l2p1, l1p1, l1p2 ),
			DistancePointSegmentSq( l2p2, l1p1, l1p2 ) )
			);
	}

	//	�J�v�Z�����m�̓����蔻��
	bool	Collision::CapsuleVSCapsule( Vector3 p1, Vector3 p2, float r1, Vector3 p3, Vector3 p4, float r2 )
	{
		float length = DistanceSegmentSegmentSq( p1, p2, p3, p4 );

		if ( length <= r1 + r2 )	return	true;
		return	false;
	}

	//	�J�v�Z���Ƌ��̓����蔻��
	bool	Collision::CapsuleVSSphere( Vector3 p1, Vector3 p2, float r1, Vector3 c, float r2 )
	{
		float length = DistancePointSegmentSq( c, p1, p2 );

		if ( length <= r1 + r2 )	return	true;
		return	false;
	}

	//	���Ƌ��̓����蔻��
	bool	Collision::SphereVSSphere( Vector3 c1, float r1, Vector3 c2, float r2 )
	{
		Vector3 dist = c2 - c1;
		float length = dist.Length();

		if ( length <= r1 + r2 )	return true;
		return	false;
	}




