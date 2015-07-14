#include	"iExtreme.h"

//*****************************************************************************
//
//		行列関連
//
//*****************************************************************************

//*****************************************************************************
//		変換行列
//*****************************************************************************
//------------------------------------------------------
//	ＸＹＺ回転
//------------------------------------------------------
void	SetTransformMatrixXYZ( Matrix *Mat, float x, float y, float z, float ax, float ay, float az )
{
	float	sx, sy, sz, cx, cy, cz;

	sx = sinf(ax);	sy = sinf(ay);	sz = sinf(az);
	cx = cosf(ax);	cy = cosf(ay);	cz = cosf(az);

	Mat->_11 = (float)( cy*cz + sx*sy*sz );
	Mat->_12 = (float)( cx*sz );
	Mat->_13 = (float)(-sy*cz + sx*cy*sz );
	Mat->_14 = (float)(0.0f);

	Mat->_21 = (float)(-cy*sz + sx*sy*cz );
	Mat->_22 = (float)( cx*cz );
	Mat->_23 = (float)( sy*sz + sx*cy*cz );
	Mat->_24 = (float)(0.0f);

	Mat->_31 = (float)( cx*sy );
	Mat->_32 = (float)(-sx );
	Mat->_33 = (float)( cx*cy );
	Mat->_34 = (float)(0.0f);

	Mat->_41 = x;
	Mat->_42 = y;
	Mat->_43 = z;
	Mat->_44 = (float)(1.0f);
}

//------------------------------------------------------
//	ＺＸＹ回転
//------------------------------------------------------
void	SetTransformMatrixZXY( Matrix *Mat, float x, float y, float z, float ax, float ay, float az )
{
	float	sx, sy, sz, cx, cy, cz;

	sx = sinf(ax);	sy = sinf(ay);	sz = sinf(az);
	cx = cosf(ax);	cy = cosf(ay);	cz = cosf(az);
	
	Mat->_11 = cz*cy + sz*sx*sy;
	Mat->_12 = sz*cx;
	Mat->_13 = -cz*sy + sz*sx*cy;
	Mat->_14 = 0;

	Mat->_21 = -sz*cy + cz*sx*sy;
	Mat->_22 = cz*cx;
	Mat->_23 = sz*sy + cz*sx*cy;
	Mat->_24 = 0;

	Mat->_31 = cx*sy;
	Mat->_32 = -sx;
	Mat->_33 = cx*cy;
	Mat->_34 = 0;

	Mat->_41 = x;
	Mat->_42 = y;
	Mat->_43 = z;
	Mat->_44 = 1;
}

//*****************************************************************************
//		ビュー変換表列
//*****************************************************************************
//------------------------------------------------------
//	左手座標系ビュー変換
//------------------------------------------------------
void LookAtLH( Matrix& mat, Vector& Eye, Vector& At, Vector& Up )
{
	Vector3 zaxis;
	zaxis.x = At.x - Eye.x;
	zaxis.y = At.y - Eye.y;
	zaxis.z = At.z - Eye.z;
	Vector3 xaxis = Vector3( Up.y*zaxis.z - Up.z*zaxis.y, Up.z*zaxis.x - Up.x*zaxis.z, Up.x*zaxis.y - Up.y*zaxis.x ); 
	Vector3 yaxis = Vector3( zaxis.y*xaxis.z - zaxis.z*xaxis.y, zaxis.z*xaxis.x - zaxis.x*xaxis.z, zaxis.x*xaxis.y - zaxis.y*xaxis.x ); 
   
	xaxis.Normalize();
	yaxis.Normalize();
	zaxis.Normalize();

	mat._11 = xaxis.x;
	mat._21 = xaxis.y;
	mat._31 = xaxis.z;

	mat._12 = yaxis.x;
	mat._22 = yaxis.y;
	mat._32 = yaxis.z;

	mat._13 = zaxis.x;
	mat._23 = zaxis.y;
	mat._33 = zaxis.z;

	mat._14 = 0;
	mat._24 = 0;
	mat._34 = 0;

	mat._41 = -(xaxis.x*Eye.x + xaxis.y*Eye.y + xaxis.z*Eye.z);
	mat._42 = -(yaxis.x*Eye.x + yaxis.y*Eye.y + yaxis.z*Eye.z);
	mat._43 = -(zaxis.x*Eye.x + zaxis.y*Eye.y + zaxis.z*Eye.z);
	mat._44 = 1;
}

//*****************************************************************************
//		投影変換表列
//*****************************************************************************
//------------------------------------------------------
//	パース投影変換
//------------------------------------------------------
void PerspectiveLH( Matrix& mat, float fovY, float aspect, float zn, float zf )
{
	float yScale = 1 / tanf(fovY/2);
	float xScale = yScale / aspect;

	mat._11 = xScale;
	mat._12 = 0;
	mat._13 = 0;
	mat._14 = 0;

	mat._21 = 0;
	mat._22 = yScale;
	mat._23 = 0;
	mat._24 = 0;

	mat._31 = 0;
	mat._32 = 0;
	mat._33 = zf / (zf-zn);
	mat._34 = 1;

	mat._41 = 0;
	mat._42 = 0;
	mat._43 = (-zn*zf) / (zf-zn);
	mat._44 = 0;
}

//------------------------------------------------------
//	平行投影変換
//------------------------------------------------------
void OlthoLH( Matrix& mat, float w, float h, float zn, float zf )
{
	mat._11 = 2 / w;
	mat._12 = 0;
	mat._13 = 0;
	mat._14 = 0;

	mat._21 = 0;
	mat._22 = 2 / h;
	mat._23 = 0;
	mat._24 = 0;

	mat._31 = 0;
	mat._32 = 0;
	mat._33 = 1 / (zf-zn);
	mat._34 = 0;

	mat._41 = 0;
	mat._42 = 0;
	mat._43 = zn / (zn-zf);
	mat._44 = 1;
}





//*****************************************************************************
//
//		クォータニオン関連
//
//*****************************************************************************

//*****************************************************************************
//		変換
//*****************************************************************************
//------------------------------------------------------
//	行列への変換
//------------------------------------------------------
void Quaternion::toMatrix(Matrix& m)
{
	float s = sqrtf( x*x + y*y + z*z + w*w );
	s = 2.0f / (s * s);
	float vx = x * s;
	float vy = y * s;
	float vz = z * s;
	float wx = vx * w;
	float wy = vy * w;
	float wz = vz * w;
	float sx = x * vx;
	float sy = y * vy;
	float sz = z * vz;
	float cx = y * vz;
	float cy = z * vx;
	float cz = x * vy;

	m._11 = 1.0f - sy - sz;
	m._12 = cz + wz;
	m._13 = cy - wy;
	m._14 = 0.0f;
	m._21 = cz - wz;
	m._22 = 1.0f - sx - sz;
	m._23 = cx + wx;
	m._24 = 0.0f;
	m._31 = cy + wy;
	m._32 = cx - wx;
	m._33 = 1.0f - sx - sy;
	m._34 = 0.0f;
	m._41 = 0.0f;
	m._42 = 0.0f;
	m._43 = 0.0f;
	m._44 = 1.0f;
}

//------------------------------------------------------
//		球面線形補間
//------------------------------------------------------
Quaternion QuaternionSlerp(Quaternion& q, Quaternion& r, float t )
{
	//	内積
	float dot = q.x*r.x + q.y*r.y + q.z*r.z + q.w*r.w;
	float ss = 1.0f - (dot * dot);
	if(ss <= 0.0f) return q;

	Quaternion Target = r;
	if( dot < 0.0f )
	{
		Target = -Target;
		dot = -dot;
	}
	//	内積値補正
	if(dot > 1.0f ) dot = 1.0f;

	//	補間割合計算
	float ph = acosf(dot);
	float s = 1 / sinf(ph);
	float s1 = sinf(ph * (1.0f-t)) * s;
	float s2 = sinf(ph *  t      ) * s;
	//	補間
	return q*s1 + Target*s2;
}
