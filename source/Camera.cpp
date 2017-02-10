#include"Camera.h"

//默认构造函数
Camera::Camera()
{
	_cameraType = AIRCRAFT;

	_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

//摄像机类型的构造函数
Camera::Camera(CameraType cameraType)
{
	_cameraType = cameraType;

	_pos = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	_right = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	_up = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	_look = D3DXVECTOR3(0.0f, 0.0f, 1.0f);
}

//析构函数
Camera::~Camera()
{

}

//获取摄像机位置
void Camera::getPosition(D3DXVECTOR3* pos)
{
	*pos = _pos;
}

//设置摄像机位置
void Camera::setPosition(D3DXVECTOR3* pos)
{
	_pos = *pos;
}

void Camera::getRight(D3DXVECTOR3* right)
{
	*right = _right;
}

void Camera::getUp(D3DXVECTOR3* up)
{
	*up = _up;
}

void Camera::getLook(D3DXVECTOR3* look)
{
	*look = _look;
}


//----------------------------------------------------------------------
//下面三个函数都是将_pos这个点移来移去，而且是沿着right、look、up方向
//----------------------------------------------------------------------
void Camera::walk(float units)
{
	if (_cameraType == LANDOBJECT)
	{
		_pos += D3DXVECTOR3(_look.x, 0.0f, _look.z) * units;
	}
	if (_cameraType == AIRCRAFT)
		_pos += _look * units;
}

void Camera::strafe(float units)
{
	if (_cameraType == LANDOBJECT)
	{
		_pos += D3DXVECTOR3(_right.x, 0.0f, _right.z) * units;
	}

	if (_cameraType == AIRCRAFT)
		_pos += _right * units;
}

void Camera::fly(float units)
{
	if (_cameraType == LANDOBJECT)
		_pos.y += units;

	if (_cameraType == AIRCRAFT)
		_pos += _up * units;
}



//----------------------------------------------------------------------
//下面三个函数都是在_pos这个点转来转去，这时_pos是个定点
//----------------------------------------------------------------------
void Camera::pitch(float angle)
{
	D3DXMATRIX T;
	D3DXMatrixRotationAxis(&T, &_right, angle);

	D3DXVec3TransformCoord(&_up, &_up, &T);
	D3DXVec3TransformCoord(&_look, &_look, &T);
}

void Camera::yaw(float angle)
{
	D3DXMATRIX T;

	if (_cameraType == LANDOBJECT)
		D3DXMatrixRotationY(&T, angle);

	if (_cameraType == AIRCRAFT)
		D3DXMatrixRotationAxis(&T, &_up, angle);

	D3DXVec3TransformCoord(&_right, &_right, &T);
	D3DXVec3TransformCoord(&_look, &_look, &T);
}

void Camera::roll(float angle)
{
	if (_cameraType == AIRCRAFT)
	{
		D3DXMATRIX T;
		D3DXMatrixRotationAxis(&T, &_look, angle);

		D3DXVec3TransformCoord(&_right, &_right, &T);
		D3DXVec3TransformCoord(&_up, &_up, &T);
	}
}



//----------------------------------------------------------------------
//获取取景矩阵
//----------------------------------------------------------------------
void Camera::getViewMatrix(D3DXMATRIX* V)
{
	// 使三个方向向量相互垂直
	D3DXVec3Normalize(&_look, &_look);

	D3DXVec3Cross(&_up, &_look, &_right);
	D3DXVec3Normalize(&_up, &_up);

	D3DXVec3Cross(&_right, &_up, &_look);
	D3DXVec3Normalize(&_right, &_right);

	// 创建取景矩阵
	float x = -D3DXVec3Dot(&_right, &_pos);
	float y = -D3DXVec3Dot(&_up, &_pos);
	float z = -D3DXVec3Dot(&_look, &_pos);

	//这里是那个4*4矩阵的数据，返回的就是这个矩阵！
	// | Rx,    Ux,    Dx,  0 |
	// | Ry,    Uy,    Dy,  0 |
	// | Rz,    Uz,    Dz,  0 |
	// | -R*p, -U*p,  -D*p, 1 |
	(*V)(0, 0) = _right.x; (*V)(0, 1) = _up.x; (*V)(0, 2) = _look.x; (*V)(0, 3) = 0.0f;
	(*V)(1, 0) = _right.y; (*V)(1, 1) = _up.y; (*V)(1, 2) = _look.y; (*V)(1, 3) = 0.0f;
	(*V)(2, 0) = _right.z; (*V)(2, 1) = _up.z; (*V)(2, 2) = _look.z; (*V)(2, 3) = 0.0f;
	(*V)(3, 0) = x;        (*V)(3, 1) = y;     (*V)(3, 2) = z;       (*V)(3, 3) = 1.0f;
}

//设置摄像机类型
void Camera::setCameraType(CameraType cameraType)
{
	_cameraType = cameraType;
}

