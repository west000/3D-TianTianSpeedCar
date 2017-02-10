#ifndef __CameraH__
#define __CameraH__

#include<d3dx9.h>

class Camera
{
public:
	enum CameraType { LANDOBJECT, AIRCRAFT };

	Camera();
	Camera(CameraType cameraType);
	~Camera();

	void strafe(float units); 
	void fly(float units);    
	void walk(float units);  

	void pitch(float angle);
	void yaw(float angle);   
	void roll(float angle); 

	void getViewMatrix(D3DXMATRIX* V);
	void setCameraType(CameraType cameraType);
	void getPosition(D3DXVECTOR3* pos);
	void setPosition(D3DXVECTOR3* pos);

	void getRight(D3DXVECTOR3* right);
	void getUp(D3DXVECTOR3* up);
	void getLook(D3DXVECTOR3* look);

private:
	CameraType  _cameraType;   //摄像机的类型
	D3DXVECTOR3 _right;        //摄像机的right分量
	D3DXVECTOR3 _up;           //摄像机的up分量
	D3DXVECTOR3 _look;         //摄像机的look分量
	D3DXVECTOR3 _pos;          //摄像机的位置
};


#endif //__CameraH__